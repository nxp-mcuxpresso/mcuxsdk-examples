Hardware requirements
===================
- Type-C USB cable
- Loopback network cable RJ45 standard
- FRDM-IMXRT1152 board
- Personal Computer

Board settings
============
- Use the Gigabit Ethernet RJ45 port (J130).
- Set J134 to position 1-2 for the shared `ENET_MDIO` / `ETH0_RDATA3` routing.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Insert loopback network cable to the Gigabit Ethernet RJ45 port (J130).
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
The 0 frame transmitted from the ring 0 !
The frame received from the ring 0, 1, 2 is 1, 0, 0 now!
The 1 frame transmitted from the ring 1 !
The frame received from the ring 0, 1, 2 is 1, 0, 1 now!
The 1 frame transmitted from the ring 2 !
The frame received from the ring 0, 1, 2 is 1, 1, 1 now!
......
The 9 frame transmitted from the ring 0 !
The frame received from the ring 0, 1, 2 is 0, 0, 0 now!
The 10 frame transmitted from the ring 1 !
The frame received from the ring 0, 1, 2 is 0, 0, 0 now!
The 10 frame transmitted from the ring 2 !
The frame received from the ring 0, 1, 2 is 0, 0, 0 now!

30 frames transmitted succeed!
10 frames successfully received from the ring 0!
10 frames successfully received from the ring 1!
10 frames successfully received from the ring 2!
