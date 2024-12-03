Hardware requirements
=====================
- Micro USB cable
- MC56F83000-EVK board
- Personal Computer

Board hardware common setting
=============================
1. Power setting:
   > Connect J19 pin2 and pin3
   > Connect J12 pin1 and pin2
   > Connect J13 pin1 and pin2
   > Connect J15 pin1 and pin2
   > Connect J16 pin2 and pin3
   > Connect J17 pin1 and pin2
2. Onboard OSJTAG debugger setting:
   > Leave J5 open
   > Leave J6 open
     Note: J6 is only used(connected) when firmware update for onboard debugger OSJTAG.
   > Connect J11 pin 1-2, 3-4, 5-6, 7-8, to setup onboard debugger OSJTAG.
     Note: when use external debugger(connected to J10), leave all J11 pins open.

Board settings
==============
- Connect J23-1 (CAN_H) of the two boards
- Connect J23-3 (CAN_L) of the two boards
- Connect J23-2 (GND) of the two boards
- Short J18 1-2, 3-4 for each board

Prepare the Demo
================
1.  Connect USB cable between the host PC and the JM60 USB(J8) port on the target board. It setups OSJTAG and COM port in PC device manager.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board with OSJTAG debug configuration.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo. 
    Please be noted that default FCF(flash configuration field) setting makes MCU boot from bootloader. So after reset button press,
    it will wait 5s(wait in bootloader) to run the application code.

Running the demo
================
After connecting the two boards, these instructions display on each terminal window.
One board must be chosen as node A and the other board as node B. (Note: Node B should start first)
Data is sent continuously between the node A and the node B.

This message displays on the node A terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

********* FLEXCAN Interrupt EXAMPLE *********

    Message format: Standard (11 bit id)

    Message buffer 1 used for Rx.

    Message buffer 0 used for Tx.

    Interrupt Mode: Enabled

    Operation Mode: TX and RX --> Normal

*********************************************

Please select local node as A or B:

Note: Node B should start first.

Node:a

Press any key to trigger one-shot transmission

Rx MB ID: 0x123, Rx MB data: 0x0, Time stamp: 8877

Press any key to trigger the next transmission!

Rx MB ID: 0x123, Rx MB data: 0x1, Time stamp: 32459

Press any key to trigger the next transmission!
~~~~~~~~~~~~~~~~~~~~~

This message displays on the node B terminal:

********* FLEXCAN Interrupt EXAMPLE *********

    Message format: Standard (11 bit id)

    Message buffer 1 used for Rx.

    Message buffer 0 used for Tx.

    Interrupt Mode: Enabled

    Operation Mode: TX and RX --> Normal

*********************************************

Please select local node as A or B:

Note: Node B should start first.

Node:b

Start to Wait data from Node A

Rx MB ID: 0x321, Rx MB data: 0x0, Time stamp: 5759

Wait Node A to trigger the next transmission!

Rx MB ID: 0x321, Rx MB data: 0x1, Time stamp: 57276

Wait Node A to trigger the next transmission!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
