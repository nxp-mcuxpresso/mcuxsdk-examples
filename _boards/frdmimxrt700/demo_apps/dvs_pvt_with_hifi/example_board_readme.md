Hardware requirements
=====================
- Type-C USB cable
- FRDM-IMXRT700 board
- Personal Computer

Board settings
============
JP7(2-3) connected. 

The demo use PMIC as power supply.
   Jumper setting - JP1(1-2), JP2(2-3), JP3(1-2), JP4(1-2)

NOTE,
1. To rebuild the HiFi cores' binary, use the main_dsp.c provided in the project root folder to replace the file in
   dsp_examples/dsp_naturedsp/<core_id> and build dsp_naturedsp examples to get the binaries for HiFi cores.

Prepare the Demo
===============
1.  Connect a USB cable between the PC host and the MCU-LINK on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Build cm33_core1 project first, then the cm33_core0 project.
4.  Download the core0 program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to
    begin running the demo.

Running the demo
================
The log below shows the output of the demo in the terminal window:
For cm33_core0:
```
PVT Application Demo on Core0

This application uses the PMIC for power supply.
SILICON_REV_ID = B0
Build Time: Aug 26 2026--18:08:50
PVTS delay = 0x1111
Core Clock: 192000000Hz 
Input any key to start
PVT Task: Current VDD2 = 0.843750 V
```
For cm33_core1:
```
PVT Application Demo on Core1

Build Time: Aug 26 2026--16:40:55
Core Clock: 160000000Hz 
PVTS delay = 0xf0e
PVT Task: Current VDD1 = 0.818750 V
 Workload Task: Current VDD1 = 0.818750 V

```
