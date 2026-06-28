# zmk-rgb-layer — event-driven per-layer RGB underglow indicator.
# Gated on the Kconfig option AND the central role: the listener uses the
# keymap / layer-state APIs, which ZMK only compiles on the central (or a
# non-split board) — see zmk/app/CMakeLists.txt's
# `if ((NOT CONFIG_ZMK_SPLIT) OR CONFIG_ZMK_SPLIT_ROLE_CENTRAL)`. The peripheral
# half's strip is still painted: the module invokes the GLOBAL &rgb_ug behaviour
# on the central, which ZMK forwards across the split.
if(CONFIG_ZMK_RGB_LAYER_INDICATOR AND ((NOT CONFIG_ZMK_SPLIT) OR CONFIG_ZMK_SPLIT_ROLE_CENTRAL))
    target_sources(app PRIVATE src/rgb_layer.c)
endif()
