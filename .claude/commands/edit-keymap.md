Edit the keymap based on the user's request, then regenerate the SVG and commit.

## Steps

1. Read `config/eyelash_sofle.keymap` to understand the current state before making any changes.
2. Make the requested edits. Refer to the layer/behavior context below.
3. Run `mise run draw` to regenerate the SVG (always do this — never commit a stale SVG).
4. Stage and commit `config/eyelash_sofle.keymap`, `keymap-drawer/eyelash_sofle.yaml`, and `keymap-drawer/eyelash_sofle.svg` together.
5. Push to the current branch.

## Layer index

| # | Name     | Access key                     |
|---|----------|--------------------------------|
| 0 | BASE     | always on                      |
| 1 | NAV      | hold `mo1` — left thumb        |
| 2 | CODE     | hold `lt(2,DEL)` — right thumb |
| 3 | MEDIA    | hold `mo3` — right thumb       |
| 4 | SYS\|NUM | hold `mo4` — left thumb        |

## Key position numbering

Rows 0–3 have 13 positions each (cols 0–12); row 4 has 12.
Col 6 in every row = encoder click.

```
Left-side positions:  0-5, 13-18, 26-31, 39-44, 52-57
Right-side positions: 7-12, 20-25, 33-38, 46-51, 58-63
```

Row/col mapping for common keys:
```
Row 0: ` 1 2 3 4 5 [enc] 6 7 8 9 0 -
Row 1: Tab Q W E R T [enc] Y U I O P =
Row 2: Esc A S D F G [enc] H J K L ; '
Row 3: \   Z X C V B [enc] N M , . / Ret
Row 4: Mute [mo4] F11 GUI [mo1] Bspc [enc] Spc [lt2] [mo3] F12 CW
```

## Hold-tap behaviors

- `&hml MOD KEY` — left home row mod (positional: fires only on right-side key combos)
- `&hmr MOD KEY` — right home row mod (positional: fires only on left-side key combos)
- `&ralt_mt RALT KEY` — AltGr mod-tap (no positional restriction; for T and Y only)
- `&mt MOD KEY` — standard mod-tap (avoid for new additions; prefer hml/hmr/ralt_mt)
- `&lt LAYER KEY` — layer-tap
- `&mo LAYER` — momentary layer

## Common ZMK key names

Modifiers: `LCTRL LSHFT LALT LGUI RCTRL RSHFT RALT`
Special: `BSPC DEL ESC TAB RET SPACE CAPS`
Symbols: `LBKT RBKT LBRC RBRC LPAR RPAR MINUS EQUAL PLUS GRAVE SEMI APOS FSLH BSLH`
Media: `C_VOL_UP C_VOL_DN C_MUTE C_PP C_PREV C_NEXT C_STOP`
Mouse: `&mkp LCLK` `&mkp RCLK` `&mkp MCLK` `&mmv MOVE_UP/DOWN/LEFT/RIGHT`
RGB: `&rgb_ug RGB_TOG/BRI/BRD/SAI/SAD/HUI/HUD/EFF/EFR`
BT: `&bt BT_SEL 0` … `&bt BT_CLR`

## After editing

Always read KEYMAP.md if the user asks about timing — it has the full parameter reference and tuning tips.
