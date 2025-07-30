Hardware requirements
=====================
- Mini/micro USB cable
- imx943evk board
- Personal Computer

Board settings
==============
Prepare two imx943evk boards
Connect swp0-swp0, swp2-swp2.
Connect swp1 of imx943evk board A to device A
Connect swp1 of imx943evk board B to device B

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

On device A:
ifconfig eth0 192.168.100.1 up

On device B:
ifconfig eth0 192.168.100.2 up
ping 192.168.100.1
Can ping successfully.

Disconnect one link swp0-swp0 or swp2-swp2.
ping 192.168.100.1 on device B, can ping successfully.

