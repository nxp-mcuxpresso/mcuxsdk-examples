Hardware requirements
=====================
- USB-C cable
- Loopback network cable RJ45 standard
- MIMXRT2660-EVK board
- Personal Computer

Board settings
==============
Connect jumper J52 pins 2-3 to route the on-board analog mux to the
external RGMII PHY. The COMM_ENET_QOS MAC then drives the on-board
YT8531 gigabit PHY (RGMII, PHY address 0x00) which terminates at RJ45
port J57.

This example exercises the IEEE 1588 PTP timestamping path. The PTP
reference clock is taken at 50 MHz (ENET_PTP_REF_CLK in app.h).

Connect the loopback cable to RJ45 port J57.

Prepare the Demo
================
1.  Connect a USB-C cable between the host PC and the debug USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Insert loopback network cable to Ethernet RJ45 port J57.
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

ENET example start.
 Get the 1-th time 0 second, 1864 nanosecond
 Get the 2-th time 0 second, 7275192 nanosecond
 Get the 3-th time 0 second, 14804820 nanosecond
 Get the 4-th time 0 second, 22420658 nanosecond
 Get the 5-th time 0 second, 30036030 nanosecond
 Get the 6-th time 0 second, 37651402 nanosecond
 Get the 7-th time 0 second, 45267240 nanosecond
 Get the 8-th time 0 second, 52882612 nanosecond
 Get the 9-th time 0 second, 60498450 nanosecond
 Get the 10-th time 0 second, 68113822 nanosecond

Transmission start now!
The 1 frame transmitted success!
 the timestamp is 0 second, 167770756 nanosecond
 One frame received. the length 1014
 the timestamp is 0 second, 167973396 nanosecond
The 2 frame transmitted success!
 the timestamp is 0 second, 200548167 nanosecond
 One frame received. the length 1014
 the timestamp is 0 second, 200750756 nanosecond
The 3 frame transmitted success!
 the timestamp is 0 second, 233324728 nanosecond
 One frame received. the length 1014
 the timestamp is 0 second, 233527436 nanosecond
The 4 frame transmitted success!
 the timestamp is 0 second, 266102139 nanosecond
 One frame received. the length 1014
 the timestamp is 0 second, 266305136 nanosecond
The 5 frame transmitted success!
 the timestamp is 0 second, 298878785 nanosecond

......
The 20 frame transmitted success!
 the timestamp is 0 second, 792377480 nanosecond
 One frame received. the length 1014
 the timestamp is 0 second, 792580698 nanosecond

10BASE-T1S PHY support
======================
This example supports the internal 10BASE-T1S PHY (TENBASET_PHY) as an
alternative to the external RGMII PHY. To enable, define in board.h:

    #define BOARD_NETWORK_USE_TENBASET_PHY (1U)

Hardware setup for 10BASE-T1S mode:
- Set jumper J52 to position 1-2 to route the board analog mux to the
  T1S PHY.
- Attach the T1S link to RJ45 port J40 (the T1S connector dedicated to
  COMM_ENET_QOS on this EVK).

Hardware setup for RGMII mode:
- Set jumper J52 to position 2-3 to route the board analog mux to the
  external RGMII PHY.
- Use RJ45 port J57 for the RGMII link.
