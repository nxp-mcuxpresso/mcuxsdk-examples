Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA153 board
- Personal Computer

Board settings
============
I3C one board:
  + Transfer data from MASTER_BOARD to SLAVE_BOARD of I3C interface, I3C0 pins of MASTER_BOARD are connected with
    I3C0 pins of SLAVE_BOARD
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER_BOARD        CONNECTS TO         SLAVE_BOARD
Pin Name   Board Location     Pin Name   Board Location
P0_16      J20                P0_16      J20
P0_17      J21                P0_17      J21
GND        J6-8               GND        J6-8
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
===============
1.  Connect a Type-C USB cable between the host PC and the MCU-Link port(J15) on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Remove jumper J43 3-4.
5.  Press reset button to begin running the demo.

Running the demo
================
The following message shows in the terminal if the example runs successfully.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
I3C board2board Interrupt example -- Slave transfer.


Check I3C master I2C transfer.
Slave received data :
0x 1  0x20  0x 0  0x 1  0x 2  0x 3  0x 4  0x 5
0x 6  0x 7  0x 8  0x 9  0x a  0x b  0x c  0x d
0x e  0x f  0x10  0x11  0x12  0x13  0x14  0x15
0x16  0x17  0x18  0x19  0x1a  0x1b  0x1c  0x1d
0x1e  0x1f


 I3C master I2C transfer finished .

Check I3C master I3C SDR transfer.
Slave received data :
0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7
0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17
0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f


 I3C master I3C SDR transfer finished .

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
