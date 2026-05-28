Hardware requirements
=====================
- Micro USB cable
- two IMX952-EVK boards
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer

Board settings
==============
Connection as below:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MASTER(LPI2C6)                connect to        SLAVE(LPI2C6)
Pin Name    Board Location                      Pin Name    Board Location
SCL         J18 pin 5                           SCL         J18 pin 5
SDA         J18 pin 3                           SDA         J18 pin 3
GND         J18 pin 2                           GND         J18 pin 2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note: J18 is the 40-PIN GPIO HDR expansion connector.
      - J18-Pin5: LPI2C6_SCL (GPIO_IO03)
      - J18-Pin3: LPI2C6_SDA (GPIO_IO02)
      - J18-Pin2: Common Ground (any GND pin on J18 can be used)
      - Pull-up resistors: 2.2kΩ @ 3.3V (built-in, no external pull-ups needed)
      - Each board must be powered independently
      - No IO expander configuration required for this example




#### Please note this application can't support running with Linux BSP! ####

Prepare the Demo
================
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW4 to power on the board.
2.  Connect a micro USB cable between the host PC and the J31 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either re-power up your board or launch the debugger in your IDE to begin running the example

Running the demo
================
When the example runs successfully, the following message is displayed in the terminal:

~~~~~~~~~~~~~~~~~~~~~
LPI2C board2board interrupt example -- Master transfer.
Master will send data :
0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7  
0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f  
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17  
0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f  

Receive sent data from slave :
0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7  
0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f  
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17  
0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f  


End of LPI2C example .
~~~~~~~~~~~~~~~~~~~~~
