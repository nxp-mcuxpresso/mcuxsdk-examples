# lpi2c_vlps_peripheral

## Overview
This example describes how to use kinetis SDK drivers to implement LPI2C transmit and receive in the VLPS (very low power STOP mode) with async DMA.
The LPI2C module is designed to have ability to work under low power module like STOP, VLPW and VLPS. It can use DMA to transmit the data from or to application user buffer without CPU interaction.

It uses LPI2C to access the on board accelerometer sensor to read the Accelerometer X, Y, Z data every 500ms. CPU would keep in VLPS low power mode, except for some trigger events and data output to LPUART0.

## Supported Boards
- [FRDM-KE15Z](../../../_boards/frdmke15z/demo_apps/lpi2c_vlps_peripheral/example_board_readme.md)
- [FRDM-KE17Z](../../../_boards/frdmke17z/demo_apps/lpi2c_vlps_peripheral/example_board_readme.md)
- [FRDM-KE17Z512](../../../_boards/frdmke17z512/demo_apps/lpi2c_vlps_peripheral/example_board_readme.md)
