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
port J71.

This example exercises the IEEE 1588 timestamping path on the MAC's
adjustable timer. The PTP timer reference clock root used is
kCLOCK_Root_COMM_eth0_timerclk; if your application requires a specific
PTP frequency, set it via CLOCK_SetRootClock before BOARD_InitHardware
returns or update the PTP_CLK_FREQ macro in app.h.

Connect the loopback cable to RJ45 port J71.

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

ENET PTP 1588 example start.

Get the 1-th time xx second xx nanosecond
........
Get the 10-th time xx second xx nanosecond

The 1 frame transmitted success! the timestamp is xx second, xx nanosecond
The 2 frame transmitted success! the timestamp is xx second, xx nanosecond
The 3 frame transmitted success! the timestamp is xx second, xx nanosecond
The 4 frame transmitted success! the timestamp is xx second, xx nanosecond
......
The 20 frame transmitted success! the timestamp is xx second, xx nanosecond

Note: the xx second and xx nanosecond should not be zero and should be number with solid increment.

The transmitted frame is a 1000 length broadcast frame.

When a 1000 length ptp event message frame is received, the log would be added to the terminal like:
A frame received. the length 1000 the timestamp is xx second, xx nanosecond
Dest Address xx:xx:xx:xx:xx:xx Src Address xx:xx:xx:xx:xx:xx

A frame received. the length 1000 Dest Address xx:xx:xx:xx:xx:xx Src Address xx:xx:xx:xx:xx:xx

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
