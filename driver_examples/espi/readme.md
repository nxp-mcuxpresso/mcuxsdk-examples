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

Key Features
- eSPI device initialization and configuration
- Virtual Wire channel for platform signals
- Peripheral channel for memory and I/O access
- OOB channel for side-band communication
- Flash access channel for SPI flash sharing

For eSPI test setup with Promira platform, see espi_promira_guide.md.

## Supported Boards
- [FRDM-MCXA577](../../_boards/frdmmcxa577/driver_examples/espi/example_board_readme.md)
