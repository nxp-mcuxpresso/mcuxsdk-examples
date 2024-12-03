Hardware requirements
=====================
- Mini USB cable
- TWR-MC56F8200 board
- Personal Computer

Board hardware common setting
=============================
1. HW reworkd(workaround):
   > Connect J18(the mini USB port) pin4 and pin5.
     Note: This is due to HW design error, correct setting should be: J18 pin4 left empty and pin5 connected GND.
2. Power setting:
   > Connect J11 pin1 and pin2
   > Connect J6 pin1 and J7 pin2
3. UART setting: 
   > Connect J8 pin3 and pin4
   > Connect J9 pin3 and pin4
4. Onboard OSJTAG debugger setting:
   > Connect J20 pin1 and pin2.
   > Leave J17 open.
     Note: J17 is only used(connected) when firmware update for onboard debugger OSJTAG.
   > Connect J21 pin 1-2, 3-4, 5-6, 7-8, to setup onboard debugger OSJTAG.
     Note: when use external debugger(connected to J14), leave all J21 pins open

Board settings
==============
The example requires connecting between CAN pins of two TWR-MC56F8200 boards.
The connection should be set as follows:
On board:
- J15-1, J15-2 connected
- J16-1, J16-2 connected
- J16-3, J16-4 connected
Between TWR-MC56F8200 boards:
node A   connect to   node B
J13-3                 J13-3
J13-4                 J13-4
J13-5                 J13-5

Prepare the Demo
================
1.  Connect USB cable between the host PC and the JM60 USB(J18) port on the target board. It setups OSJTAG and COM port in PC device manager.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board with OSJTAG debug configuration.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
After connecting the two boards, these instructions display on each terminal window.
One board must be chosen as node A and the other board as node B. (Note: Node B should start first)
Data is sent continuously between the node A and the node B

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
This message displays on the node B terminal:
********* MSCAN Interrupt EXAMPLE *********
    Message format: Standard (11 bit id)
    Interrupt Mode: Enabled
    Operation Mode: TX and RX --> Normal
*********************************************

Please select local node as A or B:
Note: Node B should start first.
Node:B
Start to Wait data from Node A

Rx MB ID: 0x321, Rx MB data: 0x0
Wait Node A to trigger the next transmission!

Rx MB ID: 0x321, Rx MB data: 0x1
Wait Node A to trigger the next transmission!

Rx MB ID: 0x321, Rx MB data: 0x2
Wait Node A to trigger the next transmission!

This message displays on the node A terminal:
********* MSCAN Interrupt EXAMPLE *********
    Message format: Standard (11 bit id)
    Interrupt Mode: Enabled
    Operation Mode: TX and RX --> Normal
*********************************************

Please select local node as A or B:
Note: Node B should start first.
Node:A
Press any key to trigger one-shot transmission

Rx MB ID: 0x123, Tx MB data: 0x0
Press any key to trigger the next transmission!

Rx MB ID: 0x123, Tx MB data: 0x1
Press any key to trigger the next transmission!

Rx MB ID: 0x123, Tx MB data: 0x2
Press any key to trigger the next transmission!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
