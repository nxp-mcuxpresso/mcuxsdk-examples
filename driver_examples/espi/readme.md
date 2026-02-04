# espi_device

## Overview

The espi_device example demonstrates how to use the eSPI (Enhanced Serial Peripheral Interface) driver to communicate between an eSPI controller and an eSPI device.

1. This example shows how to initialize the eSPI device peripheral.
2. How to configure eSPI channels (Virtual Wire, OOB, Flash Access, and Peripheral channels).
3. How to handle eSPI communication and data transfer between controller and device.

The example performs the following operations:
1. Initializes the eSPI device with default configuration.
2. Enables and configures the supported eSPI channels.
3. Demonstrates Virtual Wire communication for system events.
4. Shows how to handle peripheral channel I/O cycles (memory/IO read/write).
5. Demonstrates OOB (Out-of-Band) message transfer if supported.
6. Handles flash access channel operations if supported.

## Key Features
- eSPI device initialization and configuration
- Virtual Wire channel for platform signals
- Peripheral channel for memory and I/O access
- OOB channel for side-band communication
- Flash access channel for SPI flash sharing

## eSPI ProMira Test Guide
Use `espi_host.py` with ProMira host and analyzer devices to exercise eSPI transactions.

Refer to https://www.totalphase.com/support/articles/213706108-promira-serial-platform-espi-analyzer-user-manual
Download promact_is-api-windows-x86_64-v1.60 from https://www.totalphase.com/products/promira-software-api-espi
Follow the official manual to prepare the python environment.

```bash
python espi_host.py <IP_ADDRESS> <SIM_MODE> <COMMAND1> [COMMAND2] [...]
```

- IP: Promira device address. SIM_MODE: `0`=transaction, `1`=master-only TAR/response clocks.
- COMMAND: one or more of the grouped options below.

### Command Groups
- Virtual Wire: `mcux_sts_get`, `mcux_vw_put`, `mcux_vw_get` — status, write/read VWs.
- OOB: `mcux_oob_put`, `mcux_oob_get` — send/read OOB messages.
- Flash (SAF): `mcux_saf_erase`, `mcux_saf_write`, `mcux_saf_read`, `mcux_saf_wrd` — erase/write/read/verify (addr 0x100 by default).
- Peripheral I/O — Endpoint: `mcux_ep_write_p80`, `mcux_ep_write_data`, `mcux_ep_write_cmd`, `mcux_ep_read_data` — POST/endpoint data-cmd R/W.
- Peripheral I/O — Index-Data: `mcux_idx_write_data`, `mcux_idx_write_cmd` — write data/command.
- Mailbox (Memory): `mcux_mb_write`, `mcux_mb_read`, `mcux_mb_wrd` — write/read/verify (addr 0x0400 by default).

## Supported Boards
- [FRDM-MCXA577](../../_boards/frdmmcxa577/driver_examples/espi/example_board_readme.md)
