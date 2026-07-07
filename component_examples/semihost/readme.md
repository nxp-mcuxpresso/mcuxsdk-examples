# semihost

## Overview
The semihost demo exercises the semihost utility component. Standard `printf` is
routed to the debugger console by `semihost_console`
(`CONFIG_MCUX_COMPONENT_utility.semihost_console=y`), and the direct host
file-I/O API (`SEMIHOST_Open` / `SEMIHOST_Write` / `SEMIHOST_Read` /
`SEMIHOST_Close`) creates a file on the debugger host over ARM semihosting,
writes a payload, reads it back, and verifies the contents. The direct API
touches no C library, so it needs no special library configuration.

This example issues the semihosting trap itself (via `semihost_console` and the
direct API), so it runs on armgcc, IAR and Keil MDK - MicroLib included. Only
the separate `semihost_file` unit (libc `fopen` retarget) is unavailable under
MicroLib, and this example does not use it.

## Supported Boards
- [FRDM-MCXW23](../../_boards/frdmmcxw23/component_examples/semihost/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/component_examples/semihost/example_board_readme.md)
