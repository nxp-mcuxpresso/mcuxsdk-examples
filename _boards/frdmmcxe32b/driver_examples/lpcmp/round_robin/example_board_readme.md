Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXE32B board
- Personal Computer

Board settings
==============
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Pin Name    Board Location     checker channel number       pre-set value            trigger        
PTA0            J9-3                channel 0                    0              input < DAC output
PTA1            J9-5                channel 1                    1              input > DAC output
PTA2            J2-9                channel 2                    0              input < DAC output
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This example uses LPCMP1 instance, the fixed port is plusMuxPort, the fixed channel is internal DAC
module, and the checker channels are channel 0(CMP1_IN0, PTA0, J9-3), channel 4(CMP1_IN1, PTA1, J9-5)
and channel 5(CMP1_IN2, PTA2, J2-9) in miusMuxPort.

Set the input voltage of channel 0, channel 4 and channel 5 to be less than, greater than
and less than the output voltage of the DAC respectively. The comparison results of the
three channels are different from the pre-set value. So the interrupt occurred and the terminal
will print corresponding channel changed information.

Prepare the Demo
===============
1.  Connect a type-c USB cable between the host PC and the MCU-Link USB port (J13) on the target board.
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
When the demo runs successfully, following information can be seen on the MCU-Link terminal:
The DAC reference comes from 1.2 V PMC bandgap reference in this project, note that the 1.2 V internal reference
voltage is not available in Standby mode. The checker channel's pre-set value is 0x2U.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPCMP RoundRobin Example.
channel 0 comparison result is different from preset value!
channel 1 comparison result is different from preset value!
channel 2 comparison result is different from preset value!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
