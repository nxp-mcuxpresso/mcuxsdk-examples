Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW23 Board
- Personal Computer

Board settings
==============
Since the PLU_OUT0 has no external pins on the FRDM board, 
so only input sources 0 and 1 are supported in this example.
Therefore, the PLU_IN5 and PLU_OUT0 do not need to be connected.

PLU input pins connection:
- J2-7 (GPIO0_6) to J2-8 (PLU_IN3 PIO0_8).
- J2-6 (GPIO0_7) to J1-6 (PLU_IN4 PIO0_9).

PLU output pins connection:
- J2-2 (PLU_OUT1 PIO0_13) to J1-5 (RGB_GREEN PIO0_0).
- J2-1 (PLU_OUT2 PIO0_14) to J4-5 (LED_BLUE PIO0_19).

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the FRDM board J10.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Expected Result
=====================
Input source and LED status table:
 
 PLU_IN5 | PLU_IN4 | PLU_IN3 | LED_BLUE | LED_GREEN
---------|---------|---------|----------|-----------
 0       | 0       | 0       | off      | off
 0       | 0       | 1       | off      | on
 0       | 1       | 0       | on       | off
 0       | 1       | 1       | on       | on

The log below shows the output of the plu combination example in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PLU combination driver example.

Select the input source.
1. Input source 0
2. Input source 1
3. Input source 2
4. Set all three input sources.
0
Select the input value.
0. Low level.
1. High level.
1
Select the input source.
0. Input source 0
1. Input source 1
2. Input source 2
3. Set all three input sources.
3
Input the three values like 000.
0. Low level.
1. High level.
011
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The LED will change as the description in the table.
