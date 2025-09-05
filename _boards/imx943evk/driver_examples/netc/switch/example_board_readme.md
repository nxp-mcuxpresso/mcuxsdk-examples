Hardware requirements
=====================
- Micro USB cable
- IMX943-EVK board
- 12V~20V power supply
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect 12V~20V power supply to the board.
2.  Connect a micro USB cable between the host PC and the J15(FTDI_DEBUG) USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Program flash.bin of this demo to SD/eMMC.
5.  Switch SW1 to power on the board.

Note: To use 2.5G SGMII of switch port0 and port1 and on-board PHY instead of 100M MII,
      add below definition in app.h.

      #define EXAMPLE_USE_PHY_GPY215 1U

Running the demo
================
When the demo runs successfully, the log would be seen on the serial terminal like:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
NETC Switch example start.
Wait for PHY link up...

MAC learning.
The frame received from port 0. Dest Address 54:27:8d:00:00:f0 Src Address 54:27:8d:00:00:00. Port 0 bounds to MAC 54:27:8d:00:00:00.
The frame received from port 1. Dest Address 54:27:8d:00:00:f0 Src Address 54:27:8d:00:00:01. Port 1 bounds to MAC 54:27:8d:00:00:01.
The frame received from port 2. Dest Address 54:27:8d:00:00:f0 Src Address 54:27:8d:00:00:02. Port 2 bounds to MAC 54:27:8d:00:00:02.

Frame forwarding.
The frame received from pseudo port. Dest Address 54:27:8d:00:00:00 Src Address 54:27:8d:00:00:f0. Frame forwarding to port 0.
The frame received from pseudo port. Dest Address 54:27:8d:00:00:01 Src Address 54:27:8d:00:00:f0. Frame forwarding to port 1.
The frame received from pseudo port. Dest Address 54:27:8d:00:00:02 Src Address 54:27:8d:00:00:f0. Frame forwarding to port 2.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
