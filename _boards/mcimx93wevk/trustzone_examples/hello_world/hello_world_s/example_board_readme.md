Hardware requirements
=====================
- USB Type-C cable
- MCIMX93W-EVK board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer

Board settings
============

Note
====
Please run the application in Low Power boot mode (without Linux BSP).
The IP module resource of the application is also used by Linux BSP.
Or, run with Single Boot mode by changing Linux BSP to avoid resource
conflict.

Prepare the Demo
===============
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW301 to power on the board.
2.  Connect a USB Type-C cable between the host PC and the J1401 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the flash.bin to the target board.
    Reference 'How to get flash.bin of TrustZone examples'
    Reference 'Getting Started with MCUXpresso SDK for MCIMX93W-EVK.pdf' to make and download flash.bin.
5.  Press the reset button on your board.

How to get flash.bin of TrustZone examples:
===========================================
1. Build secure binary and non-secure binary. For example (if west build is used),
   west build -p -b mcimx93wevk examples/trustzone_examples/hello_world_s -Dcore_id=cm33 -d hello_world_s
   west build -p -b mcimx93wevk examples/trustzone_examples/hello_world_ns -Dcore_id=cm33 -d hello_world_ns
2. Create m33_image.bin
   dd if=hello_world_ns/hello_world_ns_cm33.bin of=hello_world_s/hello_world_s_cm33.bin bs=1 count=`ls -l hello_world_ns/hello_world_ns_cm33.bin | awk '{print $5}'` seek=65536
   Rename hello_world_s/hello_world_s_cm33.bin to m33_image.bin
3. Copy m33_image.bin into imx-mkimage to make low power boot flash.bin.

Running the demo
================
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Hello from secure world!
Entering normal world.
Welcome in normal world!
This is a text printed from normal world!
Comparing two string as a callback to normal world
String 1: Test1
String 2: Test2
Both strings are not equal!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
