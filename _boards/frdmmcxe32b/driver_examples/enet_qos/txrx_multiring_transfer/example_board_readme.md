Hardware requirements
===================
- Type-C USB cable
- Loopback network cable RJ45 standard
- FRDM-MCXE32B board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Insert loopback network cable to Ethernet RJ45 port (J16).
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Make loopback cable:

        568B standard 	Unknowed standard
    J1	orange+white    green+white
    J2	orange          green
    J3	green+white     orange+white
    J4	blue            brown+white
    J5	blue+white      brown
    J6	green           orange
    J7	brown+white     blue
    J8	brown           blue+white

Connect J1 => J3, J2 => J6, J4 => J7, J5 => J8. 10/100M transfer only requires J1, J2, J3, J6, and 1G transfer requires all 8 pins.
Check your net cable color order and refer to 568B standard or the other standard. If your cable's color order is not showed in the list,
please connect J1~J8 based on your situation.

Running the demo
===============
When the demo runs, the log would be seen on the terminal like:

    ENET multi-ring txrx example start.
    Wait for PHY init...
    Wait for PHY link up...
    
    20 frames will be sent in 2 queue(s), and frames will be received in 2 queue(s).
    The number of frames transmitted from the ring 0, 1 is 10, 10!
    20 frames transmitted successfully!
    The number of frames successfully received from the ring 0, 1 is 10, 10!
