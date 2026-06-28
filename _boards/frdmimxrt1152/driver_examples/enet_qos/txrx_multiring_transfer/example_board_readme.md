Hardware requirements
===================
- Type-C USB cable
- Loopback network cable RJ45 standard
- FRDM-IMXRT1152 board
- Personal Computer

Board settings
============
- Use the 10/100 Ethernet RJ45 port (J112).
- Set J134 to position 1-2 for the shared `ENET_MDIO` routing.
- Set J136 to position 1-2 for the `ENET_QOS_REF_CLK1` routing.

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Insert loopback network cable to the 10/100 Ethernet RJ45 port (J112).
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

Connect J1 => J3, J2 => J6. 10/100M transfer only requires J1, J2, J3, and J6.
Check your net cable color order and refer to 568B standard or the other standard. If your cable's color order is not showed in the list,
please connect J1~J8 based on your situation.

Running the demo
===============
When the demo runs, the log would be seen on the terminal like:

 ENET multi-ring txrx example start.

30 frames will be sent in 3 queues, and frames will be received in  queues.
The frame transmitted from the ring 0, 1, 2 is 10, 10, 10!
30 frames transmitted succeed!
The frames successfully received from the ring 0, 1, 2 is 10, 10, 10!
