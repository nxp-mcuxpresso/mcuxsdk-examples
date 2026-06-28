Hardware requirements
=====================
- Type-C USB cable
- FRDM-IMXRT1152 board
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
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the example in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CDOG Peripheral Driver Example

CDOG IRQ Reached 
* Miscompare fault occured *

CDOG IRQ Reached 
* Sequence fault occured *

CDOG IRQ Reached 
intruction timer:  ffffe9
intruction timer:  f9b236
intruction timer:  f3697a
intruction timer:  ed20b7
intruction timer:  e6d7fa
intruction timer:  e08f37
intruction timer:  da4676
intruction timer:  d3fdb7
intruction timer:  cdb4f9
intruction timer:  c76c38
intruction timer:  c12379
intruction timer:  badab7
intruction timer:  b491f7
intruction timer:  ae4938
intruction timer:  a80076
intruction timer:  a1b7b8
intruction timer:  9b6ef9
intruction timer:  95263b
intruction timer:  8edd76
intruction timer:  8894bb
intruction timer:  824bf9
intruction timer:  7c0337
intruction timer:  75ba76
intruction timer:  6f71b8
intruction timer:  6928f7
intruction timer:  62e037
intruction timer:  5c9777
intruction timer:  564eb7
intruction timer:  5005f7
intruction timer:  49bd37
intruction timer:  437477
intruction timer:  3d2bb8
intruction timer:  36e2f9
intruction timer:  309a3a
intruction timer:  2a5177
intruction timer:  2408b8
intruction timer:  1dbff9
intruction timer:  177737
intruction timer:  112e79
intruction timer:   ae5bb
intruction timer:   CDOG IRQ Reached 
* Timeout fault occured *

CDOG IRQ Reached 
49cf7
End of example
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
