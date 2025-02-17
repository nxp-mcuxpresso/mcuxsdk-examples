Hardware requirements
=====================
- Micro USB cable
- JTAG/SWD debugger
- MIMXRT700-EVK board
- Personal Computer

Board settings
==============
None.

Prepare the Demo
================
NOTE: To be able to build the DSP project, please see the document
'Getting Started with Xplorer for MIMXRT700-EVK.pdf'.

1.  Download the LE Audio program for CM33 core to the target board.
2.  Launch the debugger in your IDE to begin running the demo.
3.  If building debug configuration, start the xt-ocd daemon and download the program for DSP core to the target board.
4.  If building debug configuration, launch the Xtensa IDE or xt-gdb debugger to begin running the demo.

NOTE: DSP image can only be debugged using J-Link debugger. See again
'Getting Started with Xplorer for MIMXRT700-EVK.pdf' for more information.
