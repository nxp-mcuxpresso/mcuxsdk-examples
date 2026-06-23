# Board specific notes for FRDM-IMXRT700

## Hardware requirements
- FRDM-IMXRT700 board
- Murata 2EL M.2 module (IW612)
- USB-C cable

## Board settings
- Ensure the Murata 2EL M.2 module is properly seated in the M.2 connector.
- Connect the USB-C cable to J54 for debug console.

## Module configuration
This example uses the IW612 (Murata 2EL) Wi-Fi + Bluetooth combo module
connected via SDIO (Wi-Fi) and UART (BT HCI).

The combo firmware is downloaded over SDIO at boot. BT firmware download is
bypassed (CONFIG_BT_BYPASS_DOWNLOAD=y) since BT is included in the combo image.

## Build configurations
| Toolchain | Config | Command |
|-----------|--------|---------|
| armgcc | flash_release | `west build -b frdmimxrt700 examples/coex_examples/coex_wifi_edgefast_open -p always -d build_coex_edgefast_open_700 --config flash_release -- -Dcore_id=cm33_core0 -DCONFIG_TOOLCHAIN=armgcc` |
| armgcc | flash_debug | `west build -b frdmimxrt700 examples/coex_examples/coex_wifi_edgefast_open -p always -d build_coex_edgefast_open_700 --config flash_debug -- -Dcore_id=cm33_core0 -DCONFIG_TOOLCHAIN=armgcc` |

## Serial terminal settings
- Baud rate: 115200
- Data bits: 8
- Parity: None
- Stop bits: 1
- Flow control: None

## Jumper settings
- Default jumper settings apply.
