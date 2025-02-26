Toolchain supported
===================
- IAR Embedded Workbench (IDE version details are in the Release Notes)
- MCUXpresso IDE (IDE version details are in the Release Notes)
- GCC ARM Embedded (IDE version details are in the Release Notes)

Hardware requirements
=====================
- Type-C USB cable
- MCX-W72-EVK Board
- Personal Computer

Board settings
==============
No special board setting.

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB port on the board.
2. Download the program to the target board.
3. Press the reset button on your board to begin running the demo.

**Ensure that both the NCP FSCI Blackbox or NCP BLE Test App (for the NBU core) and the FSCI BRIDGE (for the Application core) have been flashed to the board.**

Running the demo
================
Interact with the device using the NXP TestTool or your custom application which supports the implemented BLE profile(s) over FSCI.
