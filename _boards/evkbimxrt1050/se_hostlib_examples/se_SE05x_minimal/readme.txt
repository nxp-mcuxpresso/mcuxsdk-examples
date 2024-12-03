
.. highlight:: bat

.. _ex-se05x-minimal:

=======================================================================
 SE05X Minimal example
=======================================================================

This project gets available memory from secure element. This is a good 
starting point to work with SE05X at low level.


Prerequisites
=====================
- Micro USB cable
- Kinetis FRDM-K64F/imx-RT1050 board
- Personal Computer
- SE05x Arduino shield


Building the Demo
=======================================================================

- Build Plug and Trust middleware stack. (Refer :ref:`building`)
- Project: ``se05x_minimal``


Running the Example
=======================================================================

If you have built a binary, flash the binary on to the board and reset 
the board. 

If you have built an *exe* to be run from Windows using VCOM, run as::
	
	se05x_minimal.exe <PORT NAME>

Where **<PORT NAME>** is the VCOM COM port.


Console output
=======================================================================

If everything is setup correctly the output would be as follows ::

   		App:INFO :mem=32767
		App:INFO :ex_sss Finished



