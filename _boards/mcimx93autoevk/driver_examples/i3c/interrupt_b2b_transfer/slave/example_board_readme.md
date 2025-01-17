Hardware requirements
=====================
- USB Type-C cable
- MCIMX93AUTO-EVK board
- Personal Computer

Board settings
============
To make the example work, connections needed to be as follows:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER                        connect to        SLAVE
Pin Name    Board Location                      Pin Name    Board Location
SCL         J17 pin 16                          SCL         J17 pin 16
SDA         J17 pin 18                          SDA         J17 pin 18
GND         J17 pin 29                          GND         J17 pin 29
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note
====
Please run the application in Low Power boot mode (without Linux BSP).
The IP module resource of the application is also used by Linux BSP.
Or, run with Single Boot mode by changing Linux BSP to avoid resource
conflict.

Prepare the Demo
===============
1.  Connect 12V~20V power supply and JLink Plus to the board, switch SW2 to power on the board
2.  Connect a USB Type-C cable between the host PC and the J26 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either re-power up your board or launch the debugger in your IDE to begin running the demo.

Tips: The I3C use the same PHY pin as LPI2C, Acore already use the i2c resource. Please test the function in "LOW POWER MODE" only.

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
