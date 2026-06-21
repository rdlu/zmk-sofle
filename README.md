# Eyelash Sofle — ZMK Firmware

> [!WARNING]
> **Personal configuration — use at your own risk.**
> Releases are experimental and built for one specific keyboard. Key layout, timing, and layer assignments are tuned to personal preference and may change without notice. If you have the same hardware, feel free to use this as a starting point, but do not expect a stable or general-purpose firmware.

> Original hardware by [380465425@qq.com](mailto:380465425@qq.com) · Original README (Chinese): [README_JP.md](README_JP.md)

ZMK firmware configuration for the **Eyelash Sofle** split keyboard (NRF52840, nice!nano v2, nice!view display). Features home row mods, a full 5-layer layout, and ZMK Studio support on the left half.

## Keymap

![Keymap](keymap-drawer/eyelash_sofle.svg)

See [KEYMAP.md](KEYMAP.md) for the full layer reference and hold-tap timing documentation.

See [WM_KEYBINDS.md](WM_KEYBINDS.md) for suggested window manager keybind configuration (niri, GNOME, COSMIC) to match the NAV layer workspace and monitor navigation keys.

---

## Layers

| # | Name | Access |
|---|------|--------|
| 0 | BASE | always on |
| 1 | NAV | hold `mo1` — left thumb |
| 2 | CODE | hold `lt(2,DEL)` — right thumb |
| 3 | MEDIA | hold `mo3` — right thumb |
| 4 | SYS\|NUM | hold `mo4` — left thumb |
| 5 | GAME? (picker) | `&to 5` — SYS right-thumb outer key |
| 6 | MINECRFT | picker joystick ↑ |
| 7 | GAME (generic) | picker joystick ↓ |

### BASE

QWERTY with home row mods and AltGr mod-tap:

| Key | Tap | Hold |
|-----|-----|------|
| S | S | LALT |
| D | D | LCTRL |
| F | F | LSHFT |
| J | J | RSHFT |
| K | K | RCTRL |
| L | L | LALT |
| F12 (right thumb) | F12 | RALT (AltGr) |

**Left thumb:** `mo(SYS|NUM)` · `F11` · `LGUI` · `mo(NAV)` · `BSPC`

**Right thumb:** `SPACE` · `lt(CODE,DEL)` · `CapsWord` · `F12 / RALT` · `mo(MEDIA)`

**Central joystick column (top→bottom):** Vol↑ · Vol↓ · Play/Pause · Next · Mute — direct media access without holding `mo(MEDIA)`.

Hold-tap timing: `tapping-term=280ms` · `require-prior-idle=230ms` · `quick-tap=175ms` · `flavor=balanced`

### NAV (1)

- **Top row:** F12 · F1–F11
- **Row 2:** Home · PgDn · PgUp · End · Ins · Print (mouse buttons removed — see SYS|NUM)
- **Home row:** WM monitor/workspace navigation (Super+key combos) · **Left · Down · Up · Right** (HJKL) · Del
- **Bottom row:** WM move-to-workspace/monitor combos · workspace shortcuts (ws1 · next · prev · last) · WM mon-left · WM mon-right
- **Central joystick column (top→bottom):** ↑ · ↓ · ← · → · Enter
- **Thumb:** Del · App menu · LCTRL · Left · Right

### CODE (2)

Left home + bottom rows, right side transparent.

**Home row:** `` ` `` · `{` · `}`(LALT) · `[`(LCTRL) · `]`(LSHFT) · `+`

**Bottom row:** `-` · `_` · `(` · `)` · `=`

### MEDIA (3)

**Home row:** Vol↓ · Vol↑ · Play/Pause · Stop

**Bottom row:** F20 (mic mute) · Mute · Prev · Next

**Central joystick column** mirrors the BASE central column (Vol↑/Vol↓/Play-Pause/Next/Mute) — kept duplicated for clarity.

### SYS|NUM (4)

**Left — Bluetooth:** CLR · BT0–BT4

**Left — RGB:** Brightness · Saturation · Hue · Effect (increase row 1 / decrease row 2) · RGB Toggle

**Mouse — emergency fallback** (only on this layer; NAV no longer carries mouse keys):

| Key | Action |
|-----|--------|
| `R` / `T` | Mouse 4 / Mouse 5 (back / forward) |
| `F` / `G` / `V` | Mouse 1 / 2 / 3 (left / right / middle click) |
| Central joystick column | Pointer move (↑/↓/←/→) |
| Central joystick (row 4) | Left click |

**Right — Numpad** (aligned with base layer 7/8/9):

```
NUM  7   8   9   -
 /   4   5   6   +
 *   1   2   3  Ent
 =   0   ,   .   %
```

### Gaming mode (5–7)

Locked gaming layers that bypass the home-row-mod / hold-tap machinery entirely, so WASD, SPACE and the modifiers behave as pure keys. Entered explicitly and held by a toggle — no key needs to stay pressed. (See [issue #11](https://github.com/rdlu/zmk-sofle/issues/11) for the design.)

**Enter:** hold `mo(SYS|NUM)` (left thumb) → tap the **right-thumb outer key** (`&to 5`) → the **GAME?** picker opens. Then push the **joystick** to choose:

| Joystick | Goes to | OLED | Underglow |
|----------|---------|------|-----------|
| ↑ | **MINECRFT** (layer 6) | `MINECRFT` | green |
| ↓ | **GAME** generic FPS/MOBA (layer 7) | `GAME` | blue |
| center | cancel → BASE | | |

Every other key on the picker is `&none`, so a stray press can't fall through to BASE.

**Inside a game layer:**
- WASD/QWER and the right half are plain `&kp` (chat/console work normally) — zero relearning, just no HRM.
- **LCTRL** on the bottom-left pinky (replaces `\`), **LSHIFT** on the left thumb, **SPACE** on *both* thumbs, **BSPC** + **LALT** on the right thumb.
- **F13** on the right thumb = push-to-talk (bind it in Discord/OBS). `mo(MEDIA)` is still reachable.
- Joystick = audio cluster (Vol↑ / Vol↓ / Mute / Play-Pause) + center Enter. Encoder = volume.
- **MINECRFT** has `F3` (debug) top-left; the generic **GAME** layer has `` ` `` (console) there.

**Exit / switch:**
- **Exit → BASE:** press the two **top corners** together (positions 0 + 12) — `GAME_EXIT` turns the underglow off and returns to BASE.
- **Re-pick:** press **top-left two keys** together (positions 0 + 5) to reopen the picker without bouncing through BASE.

> **RGB caveat:** the per-game underglow colour only shows on battery/BLE. `CONFIG_ZMK_RGB_UNDERGLOW_AUTO_OFF_USB=y` turns underglow off on USB power (typical wired desktop gaming), so the **OLED name** (`MINECRFT` / `GAME`) is the primary active-mode indicator. Set `AUTO_OFF_USB=n` if you want the colour while wired.

---

## Flashing

### Quick flash — recommended

One command downloads the latest firmware from GitHub Actions and walks you through flashing both halves:

```bash
mise run flash-release
```

Follow the on-screen prompts: double-tap reset on the left half when asked, then on the right half. The task handles everything else (artifact download, mount detection, `sync`, unmount wait).

Requires `mise` and `gh` (GitHub CLI, authenticated). If you don't have them, see [FLASHING_MANUAL.md](FLASHING_MANUAL.md) for the drag-and-drop flow.

### Clearing ZMK Studio overlay (when new firmware doesn't take effect)

If you've used ZMK Studio to edit the keymap live, the changes are stored on the left half's settings partition and **survive a normal firmware flash**. Symptom: you flash a new release and the keymap still behaves like the old version.

Fix — flash settings reset first, then the normal firmware on top:

```bash
mise run flash-release-reset   # double-tap the LEFT half when prompted
mise run flash-release          # double-tap left, then right
```

This also wipes the left half's Bluetooth bonds — see [Re-pairing the halves](#re-pairing-the-halves) below if the halves don't auto-pair afterwards.

### Firmware files

For reference — the `mise` task downloads these automatically, but if you're flashing manually they are:

| File | Target |
|------|--------|
| `eyelash_sofle_studio_left.uf2` | Left half (ZMK Studio enabled) |
| `nice_view-eyelash_sofle_right-zmk.uf2` | Right half |
| `settings_reset-nice_nano_v2-zmk.uf2` | Either half — clears Studio overlay + BT bonds |

For the drag-and-drop flashing flow without `mise`, see [FLASHING_MANUAL.md](FLASHING_MANUAL.md).

### Re-pairing the halves

If the halves fail to connect to each other, clear their Bluetooth bonds and let them re-discover:

**Left half — via the keymap (no reflash needed):**
1. Hold `mo(SYS|NUM)` (left thumb) and tap `BT_CLR` (top-left of the SYS layer, above `Q`).

**Right half — always requires settings reset:**
1. Flash `settings_reset-nice_nano_v2-zmk.uf2` to the right half (the right half has no independent access to the SYS layer when disconnected).

**After clearing bonds:**
1. Power off both halves.
2. Power on the **right** half first (peripheral — starts advertising).
3. Power on the **left** half (central — scans and connects).
4. They auto-pair within a few seconds.

---

## Firmware memory budget

Headroom available for future keymap growth, measured on the `feat/gaming-mode` build (10 layers incl. gaming, local build). The left half is the tightest because ZMK Studio ships on it.

| Build | Flash used | Flash free | RAM used | RAM free |
|-------|-----------:|-----------:|---------:|---------:|
| Left (Studio + nice_view) | 380 KB / 792 KB (48.0%) | **412 KB** | 100 KB / 256 KB (38.9%) | **156 KB** |
| Right (nice_view) | 269 KB / 792 KB (33.9%) | **523 KB** | 61 KB / 256 KB (24.0%) | **195 KB** |
| Settings reset | 45 KB / 792 KB (5.7%) | — | 11 KB / 256 KB (4.4%) | — |

Over half the flash is still free on the left half and two-thirds on the right, so new layers, macros, and combos have plenty of room. Dropping ZMK Studio from the left half would recover a large chunk of flash if a future feature ever needs it.

Refresh the numbers after any significant keymap change by re-reading the `Memory region` block in the latest `Build ZMK firmware` run log.

---

## Building Locally on Arch Linux

This repo uses [ZMK Firmware](https://zmk.dev/) with a west workspace and [mise](https://mise.jdx.dev/) for environment management.

### 1. Install system dependencies

```bash
sudo pacman -S cmake ninja dtc git dfu-util
yay -S python-west mise
```

> **Why not `yay -S zephyr-sdk`?** The AUR package installs SDK 1.0.0 (March 2026), which requires Zephyr 4.2+. ZMK v0.3.0 targets Zephyr 3.5 — incompatible.

### 2. Install the Zephyr SDK 0.17.0 (ARM toolchain)

The SDK goes into `tools/` (already gitignored):

```bash
mkdir -p tools
wget -P tools https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.17.0/zephyr-sdk-0.17.0_linux-x86_64_minimal.tar.xz
tar xf tools/zephyr-sdk-0.17.0_linux-x86_64_minimal.tar.xz -C tools/
tools/zephyr-sdk-0.17.0/setup.sh -t arm-zephyr-eabi -h -c
rm tools/zephyr-sdk-0.17.0_linux-x86_64_minimal.tar.xz
```

### 3. Bootstrap the environment (run once)

mise provisions Python 3.12 via uv and auto-activates a `.venv` on `cd`:

```bash
mise install
mise run setup
```

`setup` runs `west init`, `west update`, `west zephyr-export`, and installs Python deps into the venv. ZMK source lands in `zmk/` and `modules/` (gitignored).

### 4. Build and flash

Build the firmware locally:

```bash
mise run build-left    # left half (ZMK Studio enabled)
mise run build-right   # right half
mise run build-reset   # settings reset (clears Studio overlay + BT bonds)
```

Flash the locally-built firmware:

```bash
mise run flash-left    # or flash-right / flash-reset
```

Each `flash-*` task prompts you to double-tap reset, waits up to 60 seconds for the `NICENANO` drive to mount, copies the `.uf2`, and syncs the write. The keyboard reboots automatically once the copy completes.

For a one-shot rebuild + interactive flash of both halves with a memory-budget summary at the end:

```bash
mise run build-flash
```

> If you just want to flash a release without building from source, use `mise run flash-release` instead — it downloads the latest main artifacts from GitHub Actions. See the [Flashing](#flashing) section above.

### 4b. Print the keymap layout (PDF)

The keymap-drawer SVG can be wrapped into a printable A4 PDF (header with version + footer with date and repo URL). Three sources are available depending on what you want to print:

```bash
mise run draw-pdf         # use the SVG already in keymap-drawer/ (no regen, no fetch)
mise run draw-pdf-fresh   # regenerate the SVG locally first (mise run draw), then build the PDF
mise run draw-pdf-ci      # fetch the CI-rendered SVG from origin/main, then build the PDF
mise run open-pdf         # generate from local SVG + open in default viewer (xdg-open)
```

The PDF is written to `keymap-drawer/eyelash_sofle.pdf` (gitignored — rebuild on demand). `weasyprint` is provisioned by mise via the `pipx:weasyprint` entry in `[tools]`; if it's missing, run `mise install`. To upgrade: `mise upgrade pipx:weasyprint`.

### 5. Keymap Editor (visual web editor + auto-build)

[keymap-editor](https://nickcoutsos.github.io/keymap-editor/) is a browser-based visual keymap editor that commits changes directly to this repo and triggers a firmware build automatically.

**Setup (one-time):**
1. Install the [keymap-editor GitHub App](https://github.com/apps/keymap-editor) and grant it access to this repo.
2. Open [nickcoutsos.github.io/keymap-editor](https://nickcoutsos.github.io/keymap-editor/) and sign in with GitHub.
3. Select this repo — it will detect `config/eyelash_sofle.keymap` and `config/eyelash_sofle.json` automatically.

**Workflow:**
1. Edit keybindings visually in the browser.
2. Click **Save** — keymap-editor commits the updated `.keymap` to `main`.
3. GitHub Actions runs automatically:
   - `build.yml` builds new firmware and uploads `.uf2` artifacts.
   - `draw.yml` regenerates the keymap SVG in `keymap-drawer/`.
4. Download the artifacts from the [Actions tab](../../actions) and flash.

> The `build.yml` and `draw.yml` workflows trigger on `main` pushes only. Use `workflow_dispatch` from the Actions tab to build manually from any branch.

### 6. ZMK Studio (live keymap editing)

With the Studio firmware on the left half, you can edit keymaps live without rebuilding:

1. Connect the left half via USB.
2. Open [studio.zmk.dev](https://studio.zmk.dev) in Chrome/Edge (WebSerial required).
3. Changes apply immediately; click **Save** to persist to flash.

> Rebuilding and reflashing will overwrite Studio changes — export your keymap first if needed.
