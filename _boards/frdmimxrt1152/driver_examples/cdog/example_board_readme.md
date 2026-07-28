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
instruction timer:  ffffe9
instruction timer:  f9b236
instruction timer:  f3697a
instruction timer:  ed20b7
instruction timer:  e6d7fa
instruction timer:  e08f37
instruction timer:  da4676
instruction timer:  d3fdb7
instruction timer:  cdb4f9
instruction timer:  c76c38
instruction timer:  c12379
instruction timer:  badab7
instruction timer:  b491f7
instruction timer:  ae4938
instruction timer:  a80076
instruction timer:  a1b7b8
instruction timer:  9b6ef9
instruction timer:  95263b
instruction timer:  8edd76
instruction timer:  8894bb
instruction timer:  824bf9
instruction timer:  7c0337
instruction timer:  75ba76
instruction timer:  6f71b8
instruction timer:  6928f7
instruction timer:  62e037
instruction timer:  5c9777
instruction timer:  564eb7
instruction timer:  5005f7
instruction timer:  49bd37
instruction timer:  437477
instruction timer:  3d2bb8
instruction timer:  36e2f9
instruction timer:  309a3a
instruction timer:  2a5177
instruction timer:  2408b8
instruction timer:  1dbff9
instruction timer:  177737
instruction timer:  112e79
instruction timer:   ae5bb
instruction timer:   CDOG IRQ Reached 
* Timeout fault occured *

CDOG IRQ Reached 
49cf7
End of example
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
