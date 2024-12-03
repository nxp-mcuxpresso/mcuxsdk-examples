Hardware requirements
=====================
- FRDM-MCXN947 board
- Mini/micro USB cable
- Personal computer
- OV7670 camera
- NXP LCD-PAR-S035 display 320x480 (default)
- MikroElektronika TFT Proto 5" display 800x480

Board settings
==============
In order to route the camera data signals to chip,
some Solder Jumpers (SJ) on the back of the board must be changed:
move SJ16, SJ26, SJ27 from A side to B side,
else the captured image would appear redish.

Connect the OV7670 module to camera pins (match 3V3 pins)
Connect the display to FLEXIO LCD pins (panel pins from GND up to D15)

Prepare the Demo
================
1. Connect a USB cable between the host PC and FRDM-MCXN947 board.
2. Debug Console:
   - available with TTY device over USB MCU-Link. 
3. For console over UART: Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
4. Build the project using the MCUX IDE (axf image) or armgcc (elf image).
5. Download the program to the target board using the MCUX IDE.
6. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
