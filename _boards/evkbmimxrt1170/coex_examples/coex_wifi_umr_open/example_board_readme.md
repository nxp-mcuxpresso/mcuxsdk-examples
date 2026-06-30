# coex_wifi_umr_open

## Hardware Requirements
- MIMXRT1170-EVKB board
- Murata 2EL M.2 module (IW612)
- USB Type-C cable
- Headphones or speaker connected to audio jack

## Board Settings
- Connect the Murata 2EL M.2 module to the M.2 slot on the EVKB board.
- Connect USB Type-C cable to J11 for debug console.
- Connect headphones/speaker to J33 audio jack for audio output.

## Build Configuration
Use flexspi_nor_release or flexspi_nor_debug configuration for armgcc toolchain.

## CAAM DMA Workaround
This app includes the `--wrap=bt_encrypt_le` linker workaround to prevent CAAM DMA
from corrupting BT UART DMA when CSIP RSI generation is triggered.
