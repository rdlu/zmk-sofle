# ZMK Eyelash Sofle — Agent Context

This repo is a ZMK firmware configuration for an **Eyelash Sofle** split keyboard (NRF52840, nice!nano v2). The owner actively maintains and tunes the keymap — expect frequent changes to keys, layers, and hold-tap timing.

## Repo layout

```
config/
  eyelash_sofle.keymap   # main keymap — the file you'll edit most
  eyelash_sofle.conf     # ZMK Kconfig options
  eyelash_sofle.json     # physical key layout (used by keymap-drawer)
  west.yml               # west manifest pinning ZMK version
keymap-drawer/
  eyelash_sofle.svg      # rendered keymap image — regenerate after every edit
  eyelash_sofle.yaml     # intermediate parsed representation
KEYMAP.md                # human-readable layer reference + hold-tap timing docs
.mise.toml               # tasks: setup / build-* / flash-* / draw
keymap_drawer.config.yaml # keymap-drawer render config (icons, colors, labels)
```

## Toolchain (already set up on the owner's machine)

- **Python env**: mise + uv, auto-activates `.venv` on `cd`
- **west workspace**: initialized, ZMK source at `zmk/`, Zephyr at `zephyr/`
- **Zephyr SDK**: 0.17.0 at `tools/zephyr-sdk-0.17.0` — do NOT use AUR `zephyr-sdk` (requires Zephyr 4.2+, incompatible with ZMK v0.3.0)
- **Serial access**: user is in `uucp` group (required for ZMK Studio USB)

On a **new machine**, run `mise run setup` after cloning.

## Common workflows

```bash
mise run draw          # re-parse keymap + render SVG (do this after every keymap edit)
mise run build-left    # build left half (ZMK Studio enabled)
mise run build-right   # build right half
mise run flash-left    # copy .uf2 to mounted NICENANO (double-tap reset first)
```

Alternatively, push to the branch and download `.uf2` artifacts from GitHub Actions
(`.github/workflows/build.yml` triggers on `config/**` changes).

## Keymap structure

Five layers, accessed as follows:

| # | Name    | Access              |
|---|---------|---------------------|
| 0 | BASE    | always on           |
| 1 | NAV     | hold `mo1` (left thumb) |
| 2 | CODE    | hold `lt(2, DEL)` (right thumb) |
| 3 | MEDIA   | hold `mo3` (right thumb) |
| 4 | SYS\|NUM | hold `mo4` (left thumb) |

Right thumb cluster (BASE): `SPACE · lt(CODE,DEL) · mo(MEDIA) · F12 · CapsWord`
Left thumb cluster (BASE): `mo(SYS|NUM) · F11 · LGUI · mo(NAV) · BSPC`

## Key position numbering

The keymap has 64 positions (used in `hold-trigger-key-positions`):

```
Rows 0–3: 13 keys each (6 left + 1 encoder-click + 6 right), positions 0–51
Row 4:    12 keys      (5 left + 1 encoder-click + 5 right, + 1 encoder), positions 52–63

Left half:  cols 0–5 of each row
Right half: cols 7–12 of each row (col 6 = encoder click)
```

Left-side positions:  `0-5, 13-18, 26-31, 39-44, 52-57`
Right-side positions: `7-12, 20-25, 33-38, 46-51, 58-63`

## Hold-tap behaviors

Three behaviors defined in the `behaviors {}` block — all share the same core timing.
See **KEYMAP.md** for the full timing reference and tuning tips.

| Behavior | Used on | Positional restriction |
|----------|---------|------------------------|
| `hml` | D(LCTRL) F(LSHFT) G(LALT) | triggers only when right-side keys pressed |
| `hmr` | H(LALT) J(RSHFT) K(RCTRL) | triggers only when left-side keys pressed |
| `ralt_mt` | T(RALT) Y(RALT) | none — AltGr combos span both hands |

Current timing: `tapping-term-ms=280`, `require-prior-idle-ms=150`, `quick-tap-ms=175`, `flavor=balanced`.

## After every keymap edit

1. Run `mise run draw` to regenerate the SVG
2. Stage `config/eyelash_sofle.keymap`, `keymap-drawer/eyelash_sofle.yaml`, `keymap-drawer/eyelash_sofle.svg` together
3. Commit and push — GitHub Actions will build the firmware automatically

## Branch / PR conventions

Active development happens on feature branches. Current: `feat/home-row-mods` (PR #1).
Main branch is `main`.

## Important gotchas

- **Do not change the Zephyr SDK version** without verifying ZMK compatibility
- `keymap-drawer` CLI is `keymap` (not `keymap-drawer`) when invoked via `uvx --from keymap-drawer`
- The `eyelash_sofle/` directory is the vendor source — do not edit it; edit `config/` only
- Left half gets ZMK Studio snippet (`studio-rpc-usb-uart`) for live editing; right half does not
- `soft_off` hold time is 2000 ms (combo: Q+A+Z simultaneously)
