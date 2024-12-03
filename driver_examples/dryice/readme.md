# dryice

## Overview
The DryIce Example project is a demonstration program that uses the KSDK software to initialize
DryIce peripheral and store a secure key in it. Then it forces tamper event and verifies that the
secure key is clear.

## Prepare the Demo
Connect a serial cable from the debug UART port of the board to the PC. Start Tera Term
(http://ttssh2.osdn.jp) and make a connection to the virtual serial port.

1. Start Tera Term
2. New connection -> Serial
3. Set apropriate COMx port (x is port number) in Port context menu. Number is provided by operation
   system and could be different from computer to computer. Select COM number related to virtual
   serial port. Confirm selected port by OK button.
4. Set following connection parameters in menu Setup->Serial port...
        Baud rate:    115200
        Data:         8
        Parity:       none
        Stop:         1
        Flow control: one
5.  Confirm selected parameters by OK button.

## Running the demo
Example output on terminal:

DryIce Peripheral Driver Example

writing Secure Key = This is my password! Very secret
Secure Key erased upon Tamper Detection
