/*
 * zmk-rgb-layer — event-driven per-layer RGB underglow indicator.
 *
 * Listens for layer-state changes and paints the underglow a per-layer colour
 * by invoking the GLOBAL &rgb_ug behaviour (which ZMK forwards to both split
 * halves). This replaces the keymap-macro approach: one source of truth, no
 * release-restore ordering bugs, and correct handling of stacked layers (it
 * always paints the highest active layer).
 *
 * MVP: a fixed palette; base (layer 0) rests on a dim teal. Runtime-selectable
 * base moods + settings persistence land in a later phase.
 */

#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/logging/log.h>

#include <zmk/behavior.h>
#include <zmk/event_manager.h>
#include <zmk/keymap.h>
#include <zmk/events/layer_state_changed.h>

#include <dt-bindings/zmk/rgb.h>

LOG_MODULE_REGISTER(zmk_rgb_layer, CONFIG_ZMK_LOG_LEVEL);

struct layer_color {
    bool on;       /* false => underglow off for this layer */
    uint16_t h;    /* hue 0-360 */
    uint8_t s;     /* saturation 0-100 */
    uint8_t v;     /* brightness 0-100 (capped by BRT_MAX) */
};

/* Indexed by layer index (0 = BASE). Unmapped/over-range layers fall back to base. */
static const struct layer_color layer_colors[] = {
    [0] = {true, 160, 100, 15}, /* BASE    — dim teal */
    [1] = {true, 250, 100, 50}, /* NAV     — indigo   */
    [2] = {true, 290, 100, 50}, /* CODE    — magenta  */
    [3] = {true, 35, 100, 50},  /* MEDIA   — amber    */
    [4] = {true, 0, 100, 50},   /* SYS|NUM — red      */
    [5] = {true, 280, 100, 50}, /* GAME?   — violet   */
    [6] = {true, 120, 100, 50}, /* MINECRFT— green    */
    [7] = {true, 220, 100, 50}, /* GAME    — blue     */
};

#define NUM_LAYER_COLORS (sizeof(layer_colors) / sizeof(layer_colors[0]))

static void invoke_rgb(uint32_t cmd, uint32_t val) {
    struct zmk_behavior_binding binding = {
        .behavior_dev = "rgb_ug",
        .param1 = cmd,
        .param2 = val,
    };
    struct zmk_behavior_binding_event event = {
        .layer = 0,
        .position = 0,
        .timestamp = k_uptime_get(),
#if IS_ENABLED(CONFIG_ZMK_SPLIT)
        .source = 0,
#endif
    };
    zmk_behavior_invoke_binding(&binding, event, true);
}

/* Full setup: turn the strip on and force the SOLID effect. Done once at boot
 * (and only needs repeating if something turns the strip off). Per-layer changes
 * only need to set the colour — keeping each change to a SINGLE global &rgb_ug
 * invocation minimises split-bus traffic, which is the dominant source of the
 * right-half colour-change lag. */
static void apply_setup(void) {
    invoke_rgb(RGB_ON_CMD, 0);
    invoke_rgb(RGB_EFS_CMD, 0); /* SOLID effect so static colours read */
}

static int last_layer = -1;

static void apply_layer(uint8_t layer) {
    if (layer == last_layer) {
        return; /* dedupe: layer events can fire repeatedly for the same top layer */
    }
    last_layer = layer;

    struct layer_color c = (layer < NUM_LAYER_COLORS) ? layer_colors[layer] : layer_colors[0];

    if (!c.on) {
        invoke_rgb(RGB_OFF_CMD, 0);
        return;
    }

    /* Single call in steady state (strip already on + solid from apply_setup). */
    invoke_rgb(RGB_COLOR_HSB_CMD, RGB_COLOR_HSB_VAL(c.h, c.s, c.v));
}

static int rgb_layer_listener(const zmk_event_t *eh) {
    if (as_zmk_layer_state_changed(eh) == NULL) {
        return 0;
    }

    uint8_t highest = zmk_keymap_highest_layer_active();
    LOG_DBG("layer change -> highest active layer %d", highest);
    apply_layer(highest);
    return 0;
}

ZMK_LISTENER(rgb_layer, rgb_layer_listener);
ZMK_SUBSCRIPTION(rgb_layer, zmk_layer_state_changed);

/* Paint the resting (base) colour at boot — no layer-state event fires for the
 * always-on default layer, so without this the base stays dark until the first
 * layer change. */
static int rgb_layer_init(void) {
    apply_setup();
    apply_layer(0);
    return 0;
}

SYS_INIT(rgb_layer_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
