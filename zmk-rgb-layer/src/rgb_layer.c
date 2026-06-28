/*
 * zmk-rgb-layer — event-driven per-layer RGB underglow indicator.
 *
 * Listens for layer-state changes and paints the underglow the highest active
 * layer's colour by invoking the GLOBAL &rgb_ug behaviour (ZMK forwards it to
 * both split halves). Central-only (uses the keymap API).
 *
 * Phase 2: the BASE layer's appearance is a runtime-selectable "mood"
 *   DARK   — underglow off
 *   TEAL   — solid dim teal (default)
 *   SWIRL  — animated rainbow swirl
 * cycled by pressing the mood key (a &none key) while the SYS layer is held.
 * Non-base layers always light their solid colour regardless of mood.
 *
 * RGB writes are minimised: a small state cache (on/effect/colour) means each
 * layer change usually emits a SINGLE &rgb_ug call, which keeps the split-bus
 * traffic — the dominant latency source — low.
 *
 * Not yet persisted across reboots (mood resets to TEAL) — planned follow-up.
 */

#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/logging/log.h>

#include <zmk/behavior.h>
#include <zmk/event_manager.h>
#include <zmk/keymap.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/events/position_state_changed.h>

#include <dt-bindings/zmk/rgb.h>

LOG_MODULE_REGISTER(zmk_rgb_layer, CONFIG_ZMK_LOG_LEVEL);

/* Keymap coupling: three dedicated mood keys on the SYS layer (each a &none key
 * so it has no other effect). Pressed while SYS is the highest active layer:
 *   pos 40 -> DARK, pos 41 -> DIM (teal), pos 42 -> cycle the animations. */
#define SYS_LAYER 4
#define MOOD_KEY_DARK 40
#define MOOD_KEY_DIM 41
#define MOOD_KEY_FX 42

/* ZMK underglow effect ids (see rgb_underglow.c UNDERGLOW_EFFECT_*). */
#define EFFECT_SOLID 0
#define EFFECT_BREATHE 1
#define EFFECT_SPECTRUM 2
#define EFFECT_SWIRL 3

#define NO_COLOR 0xFFFFFFFFu /* impossible packed HSB — cache "unknown" sentinel */

/* BASE moods: DARK and TEAL are set directly by their keys; the three
 * animations are reached by the effects key, which steps through them. */
enum base_mood {
    MOOD_DARK = 0,
    MOOD_TEAL = 1,
    MOOD_BREATHE = 2,
    MOOD_SPECTRUM = 3,
    MOOD_SWIRL = 4,
};

static enum base_mood base_mood = MOOD_TEAL;

struct layer_color {
    uint16_t h;
    uint8_t s;
    uint8_t v;
};

/* Solid colour for non-base layers, indexed by layer. */
static const struct layer_color layer_colors[] = {
    [1] = {250, 100, 50}, /* NAV     — indigo  */
    [2] = {290, 100, 50}, /* CODE    — magenta */
    [3] = {35, 100, 50},  /* MEDIA   — amber   */
    [4] = {0, 100, 50},   /* SYS|NUM — red     */
    [5] = {280, 100, 50}, /* GAME?   — violet  */
    [6] = {120, 100, 50}, /* MINECRFT— green   */
    [7] = {220, 100, 50}, /* GAME    — blue    */
};

#define NUM_LAYER_COLORS (sizeof(layer_colors) / sizeof(layer_colors[0]))

/* Desired underglow state for a layer. `color` applies when has_color (solid
 * and breathe use a base colour; spectrum/swirl cycle hues and ignore it). */
struct desired {
    bool on;
    int effect;
    bool has_color;
    uint32_t color;
};

/* Cache of what we last told the strip, so we only emit the diff. */
static bool strip_on;
static int cur_effect = -1;
static uint32_t cur_color = NO_COLOR;

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

static struct desired desired_for(uint8_t layer) {
    if (layer != 0) {
        struct layer_color c =
            (layer < NUM_LAYER_COLORS) ? layer_colors[layer] : (struct layer_color){0, 0, 0};
        return (struct desired){true, EFFECT_SOLID, true, RGB_COLOR_HSB_VAL(c.h, c.s, c.v)};
    }

    switch (base_mood) {
    case MOOD_DARK:
        return (struct desired){false, EFFECT_SOLID, false, 0};
    case MOOD_BREATHE:
        return (struct desired){true, EFFECT_BREATHE, true, RGB_COLOR_HSB_VAL(160, 100, 50)};
    case MOOD_SPECTRUM:
        return (struct desired){true, EFFECT_SPECTRUM, false, 0};
    case MOOD_SWIRL:
        return (struct desired){true, EFFECT_SWIRL, false, 0};
    case MOOD_TEAL:
    default:
        return (struct desired){true, EFFECT_SOLID, true, RGB_COLOR_HSB_VAL(160, 100, 15)};
    }
}

static void apply_desired(struct desired d) {
    if (!d.on) {
        if (strip_on) {
            invoke_rgb(RGB_OFF_CMD, 0);
            strip_on = false;
        }
        return;
    }

    if (!strip_on) {
        invoke_rgb(RGB_ON_CMD, 0);
        strip_on = true;
    }
    if (d.effect != cur_effect) {
        invoke_rgb(RGB_EFS_CMD, d.effect);
        cur_effect = d.effect;
    }
    if (d.has_color && d.color != cur_color) {
        invoke_rgb(RGB_COLOR_HSB_CMD, d.color);
        cur_color = d.color;
    }
}

static void apply_layer(uint8_t layer) {
    apply_desired(desired_for(layer));
}

static void set_mood(enum base_mood m) {
    base_mood = m;
    LOG_INF("base mood -> %d", m);
    /* Live preview: paint the chosen base mood now even though SYS is held, so
     * the change is visible immediately. Releasing SYS repaints base anyway. */
    apply_desired(desired_for(0));
}

/* The effects key steps through the animations; from DARK/TEAL it enters at the
 * first animation. */
static void cycle_effect(void) {
    switch (base_mood) {
    case MOOD_BREATHE:
        set_mood(MOOD_SPECTRUM);
        break;
    case MOOD_SPECTRUM:
        set_mood(MOOD_SWIRL);
        break;
    case MOOD_SWIRL:
        set_mood(MOOD_BREATHE);
        break;
    default:
        set_mood(MOOD_BREATHE);
        break;
    }
}

static int rgb_layer_listener(const zmk_event_t *eh) {
    if (as_zmk_layer_state_changed(eh) != NULL) {
        apply_layer(zmk_keymap_highest_layer_active());
        return 0;
    }

    const struct zmk_position_state_changed *psc = as_zmk_position_state_changed(eh);
    if (psc != NULL && psc->state && zmk_keymap_highest_layer_active() == SYS_LAYER) {
        switch (psc->position) {
        case MOOD_KEY_DARK:
            set_mood(MOOD_DARK);
            break;
        case MOOD_KEY_DIM:
            set_mood(MOOD_TEAL);
            break;
        case MOOD_KEY_FX:
            cycle_effect();
            break;
        default:
            break;
        }
    }

    return 0;
}

ZMK_LISTENER(rgb_layer, rgb_layer_listener);
ZMK_SUBSCRIPTION(rgb_layer, zmk_layer_state_changed);
ZMK_SUBSCRIPTION(rgb_layer, zmk_position_state_changed);

/* Paint the resting base mood at boot (no layer-state event fires for the
 * always-on default layer). */
static int rgb_layer_init(void) {
    apply_layer(0);
    return 0;
}

SYS_INIT(rgb_layer_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
