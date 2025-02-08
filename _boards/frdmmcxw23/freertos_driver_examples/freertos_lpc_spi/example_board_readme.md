Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW23 Board
- Personal Computer

Board settings
============
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER_BOARD(SPI1)           CONNECTS TO         SLAVE_BOARD(SPI2)
Pin Name      Board Location            Pin Name      Board Location 
SPI_SSEL1     J1-5                      SPI_SSEL1     J2-8          
SPI_MOSI      J4-2                      SPI_MOSI      J2-7          
SPI_MISO      J4-1                      SPI_MISO      J2-6          
SPI_SCK       J4-6                      SPI_SCK       J2-5                
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Use the default jumper settings.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the FRDM board J10.
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
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FreeRTOS SPI example start.
This example use one SPI instance as master and another as slave on one board.
Master and slave are both use interrupt way.
Please make sure you make the correct line connection. Basically, the connection is: 
 SPI_master -- SPI_slave   
   SCK      --    SCK  
   PCS0     --    PCS0 
   SOUT     --    SIN  
   SIN      --    SOUT 
Master transmited:
0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7  
0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f  
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17  
0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f  

Slave received:
0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7  
0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f  
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17  
0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f  

Slave transmited:
0xff  0xfe  0xfd  0xfc  0xfb  0xfa  0xf9  0xf8  
0xf7  0xf6  0xf5  0xf4  0xf3  0xf2  0xf1  0xf0  
0xef  0xee  0xed  0xec  0xeb  0xea  0xe9  0xe8  
0xe7  0xe6  0xe5  0xe4  0xe3  0xe2  0xe1  0xe0  

Master received:
0xff  0xfe  0xfd  0xfc  0xfb  0xfa  0xf9  0xf8  
0xf7  0xf6  0xf5  0xf4  0xf3  0xf2  0xf1  0xf0  
0xef  0xee  0xed  0xec  0xeb  0xea  0xe9  0xe8  
0xe7  0xe6  0xe5  0xe4  0xe3  0xe2  0xe1  0xe0  

SPI master transfer completed successfully.
Slave-to-master data verified ok.
SPI slave transfer completed successfully. 
Master-to-slave data verified ok.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
