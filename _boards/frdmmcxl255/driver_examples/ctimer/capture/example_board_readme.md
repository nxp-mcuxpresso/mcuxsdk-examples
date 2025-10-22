Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXL255 board
- Personal Computer

Board settings
============
Connect following two pins on Arduino header together:

|                | FRDM pin | Chip pin |
|----------------|----------|----------|
| GPIO output    | J4-1     | P2_2     |
| CTIMER capture | J4-2     | P2_17    |

Prepare the Demo
===============
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port (J16) on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The example periodically toggles GPIO pin, CTIMER then captures its value (time).

The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CTimer capture example
Rising edge triggered and CTimer capture the edge periodically

Timer value is 2005593637 when rising edge captured
Timer value is 938662930 when rising edge captured
Timer value is 1975236405 when rising edge captured
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~