Hardware requirements
=====================
- USB-C cable
- Loopback network cable RJ45 standard
- MIMXRT2660-EVK board
- Personal Computer

Board settings
==============
No board rework is required. The COMM_ENET MAC drives the on-board
YT8531 gigabit PHY (RGMII, PHY address 0x00) which terminates at RJ45
port J71. This example uses three TX/RX rings active at gigabit speed,
so the loopback cable goes on J71.

Prepare the Demo
================
1.  Connect a USB-C cable between the host PC and the debug USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Insert loopback network cable to Ethernet RJ45 port J71.
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
================
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

10BASE-T1S PHY support
======================
This example supports the internal 10BASE-T1S PHY (TENBASET_PHY) as an
alternative to the external RGMII PHY. To enable, define in board.h:

    #define BOARD_NETWORK_USE_TENBASET_PHY (1U)

Hardware setup for 10BASE-T1S mode:
- Set jumper J52 to position 1-2 to route the board analog mux to the
  T1S PHY.
- Attach the T1S link to RJ45 port J47 (the T1S connector dedicated to
  COMM_ENET on this EVK).

Hardware setup for RGMII mode (default):
- Use RJ45 port J71 for the RGMII link (no jumper action required for
  COMM_ENET).
