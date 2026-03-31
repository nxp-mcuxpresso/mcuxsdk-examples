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

    ENET example start.
    Wait for PHY init...
    Wait for PHY link up...
     Get the 1-th time 0 second, 20040 nanosecond
     Get the 2-th time 0 second, 204715800 nanosecond
     Get the 3-th time 0 second, 409797960 nanosecond
     Get the 4-th time 0 second, 614871720 nanosecond
     Get the 5-th time 0 second, 819945360 nanosecond
     Get the 6-th time 1 second, 25019120 nanosecond
     Get the 7-th time 1 second, 229997240 nanosecond
     Get the 8-th time 1 second, 435071120 nanosecond
     Get the 9-th time 1 second, 640144760 nanosecond
     Get the 10-th time 1 second, 845218640 nanosecond

    Transmission start now!
    PHY link changed, link status = 1
    The 1 frame transmitted success!
     the timestamp is 2 second, 56362360 nanosecond
     One frame received. the length 1014
     the timestamp is 2 second, 56363600 nanosecond
    The 2 frame transmitted success!
     the timestamp is 2 second, 73146400 nanosecond
     One frame received. the length 1014
     the timestamp is 2 second, 73147640 nanosecond
    The 3 frame transmitted success!
     the timestamp is 2 second, 89926760 nanosecond
     One frame received. the length 1014
     the timestamp is 2 second, 89928000 nanosecond
    The 4 frame transmitted success!
     the timestamp is 2 second, 106712080 nanosecond
     One frame received. the length 1014
     the timestamp is 2 second, 106713320 nanosecond
    The 5 frame transmitted success!
     the timestamp is 2 second, 123683360 nanosecond
     One frame received. the length 1014
     the timestamp is 2 second, 123684600 nanosecond

    ......
    The 20 frame transmitted success!
     the timestamp is 2 second, 379255240 nanosecond
     One frame received. the length 1014
     the timestamp is 2 second, 379256480 nanosecond
