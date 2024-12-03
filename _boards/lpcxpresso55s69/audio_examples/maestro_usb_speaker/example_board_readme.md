Hardware requirements
=====================
- 2x Micro USB cable
- JTAG/SWD debugger
- LPCXpresso55s69 board
- Personal Computer
- Headphones with 3.5 mm stereo jack

Board settings
==============
1. Set the hardware jumpers (Tower system/base module) to default settings.

Prepare the Demo
================
1. Change the value of the USB_DEVICE_WORKAROUND_AUDIO_20_WINDOWS macro from (0U) to (1U) in
   the usb_device_descriptor.h file.
2. Connect the first micro USB cable between the PC host and the debug USB port on the board.
3. Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4. Download the program to the target board.
5. Connect the second micro USB cable between the PC host and the USB (full speed) port on the
   board.
6. Connect the earphone/headphone at Audio Line Out port of target board.
7. Either press the reset button on your board or launch the debugger in your IDE to begin
   running the demo.

