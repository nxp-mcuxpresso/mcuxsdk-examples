Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXE32B board
- Personal Computer

Board settings
============

## **ELE HSEB Firmware Installation Guide**

Before using **ELE HSEB**, you must first install the **ELE HSEB Firmware** to
your device.

For detailed instructions on firmware installation, please refer to the firmware
[README](../../../../../firmware/edgelock/ELE_HSEB/README.md) file.
The relative path points to the edgelock firmware release repository that should
be available in your SDK at `<sdk-root>/firmware/edgelock/`.

Prepare the Demo
===============
1.  Connect a type-c USB cable between the host PC and the MCU-Link USB port (J13) on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
