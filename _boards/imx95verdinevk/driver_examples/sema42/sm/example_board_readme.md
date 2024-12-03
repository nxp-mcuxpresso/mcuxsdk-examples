Hardware requirements
=====================
- Micro USB cable
- IMX95VERDIN-EVK board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer

Board settings
============
No special settings are required.

**Please note this application can't support running with Linux BSP!**

Prepare the Demo
===============
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch S6 to power on the board.
2.  Connect a micro USB cable between the host PC and the J4 USB port on the target board.
3.  Open two serial terminals (one for Cortex-M33 core, the other for Cortex-M7 core) with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either re-power up your board or launch the debugger in your IDE to begin running the example
6.  Make sure SystemManager run into console

Running the demo
================
The log below in the Cortex-M7 terminal window shows the commands to use in SystemManager(SM) console:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**************************************************************
* Please make sure the SM is started now.                 *
* Use the following commands in SM(M33) console for SEMA42 gate access *
* - md.b 0x44260003 1 : Get SEMA42 gate status.              *
*   - 0 - Unlocked;                                          *
*   - 9 - Locked by Cortex-M33;                                *
*   - 10 - Locked by Cortex-M7;                                *
* - mm.b 0x44260003 9 1 : Lock the SEMA42 gate.              *
* - mm.b 0x44260003 0 1 : Unlock the SEMA42 gate.            *
**************************************************************

Press anykey to start the example...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Follow the example output instructions:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Now the SEMA42 gate is unlocked, checking status in SM returns 0.
Press anykey to lock the SEMA42 gate...

Now the SEMA42 gate is locked, checking status in SM returns 3.
Lock or unlock the SEMA42 gate in SM, the status does not change.

Press anykey to unlock the SEMA42 gate...

Now the SEMA42 gate is unlocked, checking status in SM returns 0.

Lock the SEMA42 gate in SM, after locked, then press anykey...

Cortex-M33 has locked the SEMA42 gate in SM, Cortex-M7 could not lock.

Press anykey to reset the SEMA42 gate...

Now the SEMA42 gate is unlocked, checking status in SM returns 0.

Press anykey to finish the example...

SEMA42 SM example successed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
