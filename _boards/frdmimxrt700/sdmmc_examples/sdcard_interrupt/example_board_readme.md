Hardware requirements
=====================
- Type-C USB cable
- FRDM-IMXRT700 board
- Personal Computer
- SD card

Board settings
============
Recommend SD Cards used to run the example
| Brand      | Mode          | Size  | Speed Class | Video Speed Class | UHS | A1  | A2  |
|------------|---------------|-------|-------------|-------------------|-----|-----|-----|
| SanDisk    | Ultra         | 16GB  | 10          | -                 | -   | YES | -   |
| SanDisk    | Extreme       | 64GB  | -           | V30               | 3   | -   | YES |
| SanDisk    | Ultra Plus    | 32GB  | 10          | V10               | -   | YES | -   |
| SanDisk    | ImageMate PRO | 32GB  | -           | V30               | 3   | YES | -   |
| Kingston   | -             | 16GB  | 10          | -                 | 1   | -   | -   |

Prepare the Demo
===============
1.  Connect a USB cable between the PC host and the MCU-LINK USB port on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.
Please insert the SDCARD into card slot(J47)

Running the demo
===============
When the demo runs successfully, the log would be seen on the terminal like:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SDCARD interrupt example.

Please insert a card into board.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
After the SDCARD inserted into J47, the log like below would be seen:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Card size xxxxx bytes

Working condition:

  Voltage : xxxV

  Timing mode: xxx

  Freq : xxx HZ

Read/Write/Erase the card continuously until encounter error......

Write/read one data block......
Compare the read/write content......
The read/write content is consistent.
Write/read multiple data blocks......
Compare the read/write content......
The read/write content is consistent.
Erase multiple data blocks......

Input 'q' to quit read/write/erase process.
Input other char to read/write/erase data blocks again.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
