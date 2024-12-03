Hardware requirements
=====================
- Micro USB cable
- MC56F83000-EVK board
- Personal Computer
- Oscilloscope

Board hardware common setting
=============================
1. Power setting:
   > Connect J19 pin2 and pin3
   > Connect J12 pin1 and pin2
   > Connect J13 pin1 and pin2
   > Connect J15 pin1 and pin2
   > Connect J16 pin2 and pin3
   > Connect J17 pin1 and pin2
2. Onboard OSJTAG debugger setting:
   > Leave J5 open
   > Leave J6 open
     Note: J6 is only used(connected) when firmware update for onboard debugger OSJTAG.
   > Connect J11 pin 1-2, 3-4, 5-6, 7-8, to setup onboard debugger OSJTAG.
     Note: when use external debugger(connected to J10), leave all J11 pins open.

Board settings
==============
- Use oscilloscope to monitor TP9

Prepare the Demo
================
1.  Connect USB cable between the host PC and the JM60 USB(J8) port on the target board. It setups OSJTAG and COM port in PC device manager.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board with OSJTAG debug configuration.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo. 
    Please be noted that default FCF(flash configuration field) setting makes MCU boot from bootloader. So after reset button press,
    it will wait 5s(wait in bootloader) to run the application code.

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

Then the desired waveform will be output from DACA output pin(TP9).
Use the Oscilloscope to measure the signal of TP9, then you will see the desired waveform with the desired frequency.
