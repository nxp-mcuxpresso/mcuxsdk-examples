Hardware requirements
=====================
- Mini/micro USB cable
- Three imx943evk boards
- Two end devices (e.g. Linux PC or any Ethernet-capable device)
- Personal Computer

Board settings
==============
Prepare three imx943evk boards (Board A, Board B, Board C) and two end devices.
Connect the boards in a ring topology using swp0 and swp1:

  Board A swp0 <-> Board B swp0
  Board B swp1 <-> Board C swp0
  Board C swp1 <-> Board A swp1

Connect end devices to swp2:

  Board A swp2 <-> Device A
  Board B swp2 <-> Device B

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the USB DBG port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Program flash.bin to boot source and start the board.

Running the demo
===============

On Device A:
ifconfig eth0 192.168.100.1 up

On Device B:
ifconfig eth0 192.168.100.2 up
ping 192.168.100.1
Can ping successfully.

Disconnect any one link in the ring
(e.g. unplug the cable between Board A swp0 and Board B swp0).
RSTP detects the topology change and reconverges within a few seconds.
ping 192.168.100.1 on Device B recovers and succeeds again.
