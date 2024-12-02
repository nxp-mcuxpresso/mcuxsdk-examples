Hardware requirements
=====================
- Micro USB cable
- Two LPCXpresso55S28 boards
- Personal Computer

Board settings
============
Connect SPI master on board to SPI slave on other board
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Master_board(SPI7)           Slave_board(SPI7)                          
Pin Name   Board Location     Pin Name   Board Location                     
MISO       P17  pin 12          MISO      P17  pin 12
MOSI       P17  pin 10          MOSI      P17  pin 10
SCK        P17  pin 14           SCK      P17  pin 14
SSEL1      P17  pin 1          SSEL1      P17  pin 1
GND        P17  pin 7           GND       P17  pin 7
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
===============
Note: MCUXpresso IDE project default debug console is semihost
1. Connect a micro USB cable between the PC host and the LPC-Link USB port (P6) on the board.
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

~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SPI CMSIS driver board to board interrupt example.
This example use one board as master and another as slave.
Master and slave uses interrupt way. Slave should start first. 
Please make sure you make the correct line connection. Basically, the connection is: 
SPI_master -- SPI_slave   
   CLK      --    CLK  
   PCS      --    PCS 
   MOSI     --    MOSI 
   MISO     --    MISO 
   GND      --    GND 

 Master transmit:

  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F
 30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F
 
SPI transfer all data matched! 

 Master received:

  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F
 30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
