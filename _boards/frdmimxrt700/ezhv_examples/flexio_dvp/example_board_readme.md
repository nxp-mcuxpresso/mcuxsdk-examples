Hardware requirements
=====================
- Type-C USB cable
- FRDM-IMXRT700 board
- Personal Computer
- RaspberryPi Panel
- OV7670 module
Board settings
============ 
JP7 2-3 connected.
To use Raspberry panel:
Connect the panel to J8. Then connect the panel's 5V pin to JP43-1, GND pin to JP43-2.

Prepare the Demo
===============
The demo use MIPI RaspberryPi panel by default. 
1.  Connect a micro USB cable between the PC host and the USB port on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo CM33
=====================
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
When the demo runs successfully, you will find the video is dislayed on the  
panel.
and the terminal will display the following:
    EZH-V flexIO DVP example started
    Frame Update Rate: 28 fps
    Frame Update Rate: 28 fps
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note:
=====================
Connect the OV7670 camera sensor to J53 connector
short 2-3 for JP7 

Note:
=====================
If the camera frame buffer is placed in SRAM P18~P29, and camera resolution
is set to 640x480, the EZHV firmware doesn't work at 30Hz frame rate, 15Hz
frame rate works.
Root cause is accessing to these RAM_ARBITER1 partitions needs more clock cycles,
so the EZHV can't write camera data to the buffers in time.
