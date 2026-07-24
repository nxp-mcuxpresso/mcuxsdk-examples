# pf9453

## Overview

The pf9453 driver example demonstrates the usage of the PF9453 PMIC SDK component driver
(`components/pmic/pf9453`). The PF9453 is controlled over I2C; the example supplies the I2C
transport (send/receive function pointers) and the slave address to the driver, then exercises the
public API through a console menu:

1. **Read device ID** — reads `DEV_ID` and checks the `CHIP_ID` nibble to confirm a PF9453 is on the
   bus.
2. **Set BUCK2 run voltage** — uses `PF9453_SetBuck2RunVoltage()` (the DVS hot path) to switch
   VDD_CORE between 0.8 V and 0.9 V.
3. **Configure BUCK2** — uses `PF9453_ConfigBuck2()` to program run voltage, standby voltage, and the
   enable mode in one call.
4. **Dump BUCK2 registers** — reads back the BUCK2 control/output register block.

> **Caution:** BUCK2 supplies VDD_CORE on the board. Setting an out-of-range or unsafe voltage can
> damage the hardware. Only use the voltages offered by the menu.

## Supported Boards

- [MIMXRT2660-EVK](../../../_boards/mimxrt2660evk/component_examples/pf9453/example_board_readme.md)

## Running the demo

Build and flash the example, open a serial terminal (115200 8N1), and follow the on-screen menu.
On a healthy board, menu item 1 reports `DEV_ID = 0xB2 (PF9453 detected ...)`.
