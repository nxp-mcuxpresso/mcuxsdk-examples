Hardware requirements
=====================
- Mini USB cable
- TWR-KM34Z75M board
- Personal Computer

Board settings
============
Using two TWR-KM34Z75M boards:
Connection between two boards as follow:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER_BOARD        CONNECTS TO          SLAVE_BOARD
Pin Name   Board Location     Pin Name   Board Location
I2C0_SCL       J25-12         I2C0_SCL      J25-12
I2C0_SDA       J25-11         I2C0_SDA      J25-11
GND            J25-10         GND           J25-10
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  In file freertos_i2c.c in folder: boards\twrkm34z75m\rtos_examples\freertos_i2c, do following definition:
           For master, use default definition
                Build project.
                Download the program to one target board (used as master board).
           For slave, use following definition
                #define I2C_MASTER_SLAVE isSLAVE
                Build project.
                Download the program to one target board (used as slave board).
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully,
For 2 TWR-KM34Z75M boards, you can see the similar information from the terminal associated with master board and slave board as below.

For master:

==FreeRTOS I2C example start.==

This example use two boards to connect with one as master and another as slave.

Master will send data :

0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7

0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f

0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17

0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f



Master received data :

0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7

0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f

0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17

0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f





End of FreeRTOS I2C example.


For slave:


==FreeRTOS I2C example start.==

This example use two boards to connect with one as master and another as slave.

I2C slave transfer completed successfully.



Slave received data :

0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7

0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f

0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17

0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f





End of FreeRTOS I2C example.
