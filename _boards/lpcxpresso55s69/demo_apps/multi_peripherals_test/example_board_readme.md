Hardware requirements
===================
- Micro USB cable
- LPCXpresso55S69 board
- Personal Computer
- SD card
- headphones with 3.5 mm stereo jack
- source of sound (line output to 3.5 mm stereo jack)

Board settings
============
Insert the card(FAT32 formatted with testfile.txt in the root) into the card slot.

Prepare the Demo
===============
Note: MCUXpresso IDE project default debug console is semihost
1.  Connect headphones to Audio HP / Line-Out connector.
2.  Connect source of sound(from PC or Smart phone) to Audio Line-In connector.
3.  Connect a micro USB cable between the PC host and the LPC-Link USB port (J8) on the board.
4.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
5.  Download the program to the target board.
6.  Insert sdcard into sdcard.
7.  Reset the SoC and run the project.

Running the demo
===============
When the demo runs successfully, the log would be seen on the terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Multi-peripheral Test!


Start datetime: 2020- 9-24 19:15: 0
AUDIO Loopback started!
Headphones will play what is input into Audio Line-In connector.
Accelerometer: Found Accelerometer!
USB device HID mouse test
Plug-in the device, which is running mouse test, into the PC.A HID-compliant mouse is enumerated in the Device Manager.
The mouse arrow is moving on PC screen in the rectangular rotation.

SDCARD: Write/read file ......

Write to testfile.txt.

Read from testfile.txt.
Compare the read/write content......
SDCARD Tested OK!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Press the WAKEUP button, the red LED will flash.
Press the ISP button, the blue LED will flash.
Press the USER button, the green LED will constant on.
