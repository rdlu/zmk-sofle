# Sofle

- [Chinese](README.md)
- [English](README_EN.md)

## Update List

- 2024/12/21
  1. Added support for zmk-studio (just refresh the left hand to use).
- 2024/10/24
  1. Modified power supply mode to reduce power consumption.
  2. Fixed the automatic shut-off feature for RGB power supply.
- 2025/8/22
  1. update the soft off.When you press the keys Q, S and Z simultaneously and hold them for 2 seconds, the keyboard will enter a deep sleep state and cannot be awakened by pressing the keys. This function can be used when carrying it outside. The activation method is to press the reset switch once.
  2. This month, I also updated the ultra-thin versions of the corne and sofle cases. The frame and base plate have been thickened, and the opening of the reset switch has been adjusted, so that the reset switch can be easily pressed. At present, we are still conceptualizing how to design the shell with an inclined bracket.If you have carefully examined a PCB, you will notice that there are reserved interfaces for expansion IO. I wonder if anyone has been able to utilize them,I will try it！
  3. The GIF animations on the right-hand keyboard screen have been removed, which will significantly reduce the power consumption of the right-hand keyboard.

> If your  sofle was updated before 2025/8/22, please update to the latest firmware.
>

## Contact Me

For 3D printed model files or any issues and malfunctions with the keyboard, please contact [380465425@qq.com](mailto:380465425@qq.com)

## Sofle Keymap

![Sofle键位图](keymap-drawer/eyelash_sofle.svg)

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

```bash
mise run build-left    # left half (ZMK Studio enabled)
mise run build-right   # right half
mise run build-reset   # settings reset (clears BT bonds)
```

To flash: double-tap the reset button on the half you want to flash — a `NICENANO` USB drive appears — then:

```bash
mise run flash-left    # or flash-right / flash-reset
```

The keyboard reboots automatically once the file is copied.

### 5. ZMK Studio (live keymap editing)

With the Studio firmware on the left half, you can edit keymaps live without rebuilding:

1. Connect the left half via USB.
2. Open [studio.zmk.dev](https://studio.zmk.dev) in Chrome/Edge (WebSerial required).
3. Changes apply immediately; click **Save** to persist to flash.

> Rebuilding and reflashing will overwrite Studio changes — export your keymap first if needed.
