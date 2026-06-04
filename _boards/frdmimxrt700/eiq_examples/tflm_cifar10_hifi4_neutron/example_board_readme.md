Hardware requirements
=====================
- Mini/micro USB cable
- FRDM-IMXRT700 board
- Personal computer

Board settings
==============
1. Example running at 325MHz which need PMIC power supply.
2. PMIC is required to drive 1.1V on VDD2, then JP1 and JP3 on the RT700 EVK need to be shorted for proper operation
3. By default, the example is running at 325MHz. If you want to change to 250MHz, please remove Macro definition "EIQ_EXAMPLE_HSRUN_CLOCK" in compiler flags
Prepare the Demo
================
1. Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.