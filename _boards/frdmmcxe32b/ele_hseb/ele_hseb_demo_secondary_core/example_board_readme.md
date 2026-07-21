Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXE32B board
- Personal Computer

Board settings
============
No special settings are required.

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

Note:
The secondary core (M7_1) image is embedded into the primary core (M7_0) image
and programmed to flash at address 0x005C0000. The primary core releases the
secondary core through the MC_ME interface and the secondary core runs in place
directly from flash. No copy-to-RAM step is performed.
