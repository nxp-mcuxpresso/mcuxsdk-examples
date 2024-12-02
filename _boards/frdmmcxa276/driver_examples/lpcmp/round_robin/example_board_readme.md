Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA276 board
- Personal Computer

Board settings
==============
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Pin Name  Board Location  checker channel number   pre-set value      trigger        
CMP0_IN1/P1_3     J2-17        channel 0              1           input > DAC output
CMP0_IN2/P1_4     J2-7         channel 1              0           input < DAC output
CMP0_IN3/P1_0     J4-10        channel 2              1           input > DAC output
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This example uses LPCMP0 instance, the fixed port is plusMuxPort, the fixed channel is
channel 7(Internal DAC module), and the checker channels are channel 1(CMP0_IN1, P1_3, J2-17),
channel 2(CMP0_IN2, P1_4, J2-7) and channel 3(CMP0_IN3, P1_0, J4-10), in miusMuxPort.
The LPCMP's internal DAC module output voltage signal(half of VREFI), the channel's pre-set value
is 0x0AU.

Set the input voltage of channel 1, channel 2 and channel 3 to be greater than, less than
and greater than the output voltage of the DAC respectively. The comparison results of the 
three channels are different from the pre-set value. So the interrupt occurred and the terminal
will print corresponding channel changed information.

Prepare the Demo
================
1. Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
2. Open a serial terminal on PC for the serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running
   the demo.

Running the demo
================
When the demo runs successfully, following information can be seen on the OpenSDA terminal:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LPCMP RoundRobin Example.
channel 1 comparison result is different from preset value!
channel 2 comparison result is different from preset value!
channel 3 comparison result is different from preset value!

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
