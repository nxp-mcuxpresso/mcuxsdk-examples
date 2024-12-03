Hardware requirements
=====================
- Micro USB cable
- Two FRDM-K32L3A6 boards
- Personal Computer

Board settings
==============
To make lpi2c example work(M4), connections needed to be as follows:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER_BOARD        CONNECTS TO         SLAVE_BOARD
Pin Name     Board Location     Pin Name     Board Location
LPI2C1_SCL   J1-10(PTB14)       LPI2C1_SCL   J1-10(PTB14)
LPI2C1_SDA   J2-6(PTB6)         LPI2C1_SDA   J2-6(PTB6)
GND          J2-14              GND          J2-14
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To make lpi2c example work(M0), connections needed to be as follows:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER_BOARD        CONNECTS TO         SLAVE_BOARD
Pin Name     Board Location     Pin Name     Board Location
LPI2C3_SCL   J3-15(PTB15)       LPI2C3_SCL   J3-15(PTB15)
LPI2C3_SDA   J3-13(PTB16)       LPI2C3_SDA   J3-13(PTB16)
GND          J2-14              GND          J2-14
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1. Connect a micro USB cable between the PC host and the OpenSDA USB port on the board.
2. Open a serial terminal on PC for JLink serial device with these settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
The following message shows in the terminal if the example runs successfully.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CMSIS LPI2C board2board EDMA example -- Slave transfer.


End of LPI2C example .
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~