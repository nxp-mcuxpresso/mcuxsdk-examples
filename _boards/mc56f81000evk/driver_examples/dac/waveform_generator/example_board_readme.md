Hardware requirements
=====================
- 2 x Micro USB cable
- MC56F81000-EVK board
- Personal Computer
- Oscilloscope

Board hardware common setting
=============================
1. Power setting:
   > Connect J8 pin2 and pin3
   > Connect J5 pin1 and pin2
   > Connect J6 pin1 and pin2
2. Onboard PnE U-MultiLink debugger setting:
   > Connet J11 pin2 and pin3
   > Connect J13 pin 1-2, 3-4, 5-6, 7-8, to setup onboard debugger PnE U-MultiLink.
     Note: when use external debugger(connected to J10), leave all J13 pins open.

Board settings
==============
- Use oscilloscope to monitor TP1.

Prepare the Demo
================
1.  Connect USB cable between the host PC and the USB(J26) port on the target board to setup USB to UART bridge.
2.  Connect USB cable between the host PC and the USB(J12) port on the target baord to setup mulitlink debugger.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board with PnE U-MultiLink debug configuration.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
    Please be noted default FCF(flash configuration field) makes the MCU boot from flash.

Running the demo
================
When the demo runs successfully, the log would be seen on the serial terminal like:
Please note that for the limit of register width and the influence of DAC setting time,
the frequency of the waveform also limited. To generate the smooth and accurate waveform the following table
shows the recommended waveform frequency.
    | Waveform type | Range |
    | :------------:| :---: |
    | Sawtooth waveform | [50Hz, 15KHz] |
    | Triangle waveform | [50Hz, 20KHz] |
    | Square waveform | [1KHz, 250KHz] |

~~~~~~~~~~~~~~~~~~~~~
 DAC waveform generator Example.

Please select the desired waveform to generate.
Press A to generate sawtooth waveform0.
Press B to generate sawtooth waveform1.
Press C to generate triangle waveform.
Press D to generate square waveform.

Waiting for the waveform select...

Please input minium value of waveform:1861
Please input maximum value of waveform:3722
Please input step size of waveform:4
Please input the desired frequency of the waveform:1000
Succeeded to generate the desired waveform.

Press any key to generate another waveform.
~~~~~~~~~~~~~~~~~~~~~

Then the desired waveform will be output from DACA output pin(TP1).
Use the Oscilloscope to measure the signal of TP1, then you will see the desired waveform with the desired frequency.
