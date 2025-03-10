Hardware requirements
=====================
- Micro USB cable
- IMX943-EVK board
- JLink Plus
- 12V~20V power supply
- Personal Computer

Board settings
==============
The example requires 2 sets of boards.
Connect below pins:
- Board1(Manager) <-> Board2(Device)
- J17-2(CAN3_H)   <-> J17-2(CAN3_H)
- J17-3(CAN3_L)   <-> J17-3(CAN3_L)
- J17-4(GND)      <-> J17-4(GND)

Prepare the Demo
================
1.  Connect 12V~20V power supply and JLink Plus to the board, switch SW1 to power on the board
2.  Connect a micro USB cable between the host PC and the J15 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either re-power up your board or launch the debugger in your IDE to begin running the demo.

