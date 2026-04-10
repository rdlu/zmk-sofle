Build and/or flash the Eyelash Sofle firmware.

## Option A — GitHub Actions (recommended, no local toolchain needed)

Push changes to the branch. The workflow at `.github/workflows/build.yml` triggers automatically on `config/**` changes and produces `.uf2` artifacts. Download from the Actions tab on GitHub.

To trigger manually without a commit:
```bash
gh workflow run build.yml
```

## Option B — Local build

Requires the west workspace to be initialized (`mise run setup` on first use).

```bash
mise run build-left    # left half with ZMK Studio (studio-rpc-usb-uart snippet)
mise run build-right   # right half
mise run build-reset   # settings reset (clears Bluetooth bonds — use when switching hosts)
```

Output files: `eyelash_sofle_studio_left.uf2`, `eyelash_sofle_right.uf2`, `settings_reset.uf2`

## Flashing

1. Double-tap the reset button on the half you want to flash → it mounts as `NICENANO`
2. Run the matching flash task:

```bash
mise run flash-left
mise run flash-right
mise run flash-reset
```

Or copy manually:
```bash
cp <file>.uf2 /run/media/$USER/NICENANO/
```

The drive unmounts automatically when flashing is complete.

## First-time pairing after flashing both halves

1. Flash reset firmware to **both** halves (clears old bonds)
2. Flash left half firmware, then right half firmware
3. Power cycle both halves — they will pair automatically
4. Pair with the host via Bluetooth (BT0–BT4 slots on SYS|NUM layer)

## ZMK Studio (live keymap editing over USB)

Only the left half has the `studio-rpc-usb-uart` snippet enabled.
Connect left half via USB → open ZMK Studio → select the serial port.

Requirements on Arch Linux: user must be in the `uucp` group.
```bash
sudo usermod -aG uucp $USER  # then log out and back in
```

## Toolchain notes

- Zephyr SDK **0.17.0** at `tools/zephyr-sdk-0.17.0` — do NOT upgrade without checking ZMK compatibility
- ZMK is pinned to v0.3.0 (Zephyr 3.5) via `config/west.yml`
- AUR `zephyr-sdk` package requires Zephyr 4.2+ and is incompatible with this ZMK version
