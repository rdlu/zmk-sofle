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

This repo uses [ZMK Firmware](https://zmk.dev/) with a west workspace. The keyboard is the **Eyelash Sofle** (split, NRF52840).

### 1. Install system dependencies and the Zephyr SDK

```bash
yay -S python-west zephyr-sdk
```

`zephyr-sdk` from AUR pulls in all required dependencies (`cmake`, `ninja`, `dtc`, `dfu-util`, `python-pyelftools`, etc.) and installs the full Zephyr toolchain including the ARM cross-compiler needed for the nRF52840.

> **SDK version note:** ZMK v0.3.0 targets Zephyr 3.5 and was validated against SDK 0.16/0.17. The AUR package installs SDK 1.0.0. If you hit build errors related to the toolchain, fall back to the manual install:
> ```bash
> cd ~
> wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.17.0/zephyr-sdk-0.17.0_linux-x86_64_minimal.tar.xz
> tar xf zephyr-sdk-0.17.0_linux-x86_64_minimal.tar.xz
> cd zephyr-sdk-0.17.0 && ./setup.sh -t arm-zephyr-eabi -h -c
> ```

### 3. Initialize the west workspace

Run this **once** from the project root:

```bash
cd ~/Projects/zmk-sofle
west init -l config/
west update
west zephyr-export
pip install --user -r zmk/zephyr/scripts/requirements.txt
```

This will fetch ZMK v0.3.0 and the custom board definitions into `zmk/` and `modules/` directories (gitignored).

### 4. Build the firmware

Run from the project root. Each build outputs `build/zephyr/zmk.uf2`.

**Left half (with ZMK Studio support) — flash this when editing the keymap:**

```bash
west build -s zmk/app -b eyelash_sofle_left --pristine -- \
  -DZMK_CONFIG="$(pwd)/config" \
  -DSHIELD=nice_view \
  -DCONFIG_ZMK_STUDIO=y \
  -DCONFIG_ZMK_STUDIO_LOCKING=n \
  -Dsnippet=studio-rpc-usb-uart
cp build/zephyr/zmk.uf2 eyelash_sofle_studio_left.uf2
```

**Right half:**

```bash
west build -s zmk/app -b eyelash_sofle_right --pristine -- \
  -DZMK_CONFIG="$(pwd)/config" \
  -DSHIELD=nice_view
cp build/zephyr/zmk.uf2 eyelash_sofle_right.uf2
```

**Settings reset** (use once to clear Bluetooth bonds if pairing breaks):

```bash
west build -s zmk/app -b nice_nano_v2 --pristine -- \
  -DZMK_CONFIG="$(pwd)/config" \
  -DSHIELD=settings_reset
cp build/zephyr/zmk.uf2 settings_reset.uf2
```

### 5. Flash the firmware

Each half has a nice!nano v2 controller with a UF2 bootloader.

1. **Enter bootloader:** double-tap the reset button on the half you want to flash. A USB drive named `NICENANO` will appear.
2. **Copy the `.uf2` file** to the drive:

```bash
# Example for left half (adjust mount path as needed)
cp eyelash_sofle_studio_left.uf2 /run/media/$USER/NICENANO/
```

The keyboard reboots automatically after the file is copied. Flash the right half the same way.

### 6. ZMK Studio (live keymap editing)

With the Studio firmware on the left half, you can edit keymaps live without rebuilding:

1. Connect the left half via USB.
2. Open [studio.zmk.dev](https://studio.zmk.dev) in a browser (Chrome/Edge required for WebSerial).
3. Changes are applied immediately; click **Save** to persist them to flash.

> Note: ZMK Studio changes are stored on the keyboard. Rebuilding and reflashing the left half will overwrite them unless you export the keymap first.
