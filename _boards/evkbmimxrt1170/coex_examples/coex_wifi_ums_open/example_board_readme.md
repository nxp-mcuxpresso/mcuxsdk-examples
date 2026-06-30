# coex_wifi_ums_open

## Hardware Requirements
- MIMXRT1170-EVKB board
- Murata 2EL M.2 module (IW612)
- USB Type-C cable
- USB flash drive with WAV audio file

## Board Settings
- Connect the Murata 2EL M.2 module to the M.2 slot on the EVKB board.
- Connect USB Type-C cable to J11 for debug console.
- Insert USB flash drive with audio file to J5 (USB host port).

## Build Configuration
Use flexspi_nor_release or flexspi_nor_debug configuration for armgcc toolchain.
