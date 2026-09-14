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

## Known issue: host filename may pick up trailing garbage

The target passes the `SYS_OPEN` name length as `strlen(path)`, with no
terminator, per ARM DUI 0471. Some debug hosts round that length up to a word
before creating the file, so the file can appear on the host as `semihost.txt`
followed by a few junk bytes instead of under the documented name. This has been
observed with `monitor semihosting IOClient 2`, where gdb - not the GDB server -
performs the file open. If the demo reports success but no `semihost.txt` is
present, list the debugger's working directory and look for a name with that
prefix. The default IOClient routing, where the GDB server services `SYS_OPEN`
itself, is not affected.

## Supported Boards
- [FRDM-MCXW23](../../_boards/frdmmcxw23/component_examples/semihost/example_board_readme.md)
