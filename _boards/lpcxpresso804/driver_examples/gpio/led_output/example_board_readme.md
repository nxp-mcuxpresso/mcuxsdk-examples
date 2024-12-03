Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso804 board
- Personal Computer

Board settings
==============
This example project does not call for any special hardware configurations.
Although not required, the recommendation is to leave the development board's jumper settings
and configurations in default state when running this example.

Prepare the Demo
================
Connect a serial cable from the debug UART port of the board to the PC. Start Tera Term
(http://ttssh2.osdn.jp) and make a connection to the virtual serial port.

1. Start Tera Term
2. New connection -> Serial
3. Set appropriate COMx port (x is port number) in Port context menu. Number is provided by operation
   system and could be different from computer to computer. Select COM number related to virtual
   serial port. Confirm selected port by OK button.
4. Set following connection parameters in menu Setup->Serial port...
        Baud rate:    9600
        Data:         8
        Parity:       none
        Stop:         1
        Flow control: none
5.  Confirm selected parameters by OK button.


Running the demo
================
After the board is flashed the Tera Term will print message on terminal.

Example output:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 GPIO Driver example

 The LED is taking turns to shine.

 Standard port read: 7ffffbb7

 Masked port read: 7fff0000

 Port state: 7ffffbb7

 Port state: 7ffffbb7
 ...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
- When press sw1 the LED blue will turn to shine.
