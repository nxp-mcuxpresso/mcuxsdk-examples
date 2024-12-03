Hardware requirements
=====================
- Mini USB cable
- TWR-MC56F8400 board
- Personal Computer

Board hardware common setting
=============================
1. Power setting:
   > Connect J11 pin1 and pin2
   > Connect J6 pin1 and J7 pin2
2. UART setting:
   > Connect J8 pin3 and pin4
   > Connect J9 pin3 and pin4
3. Onboard OSJTAG debugger setting:
   > Connect J20 pin1 and pin2.
   > Leave J17 open.
     Note: J17 is only used(connected) when firmware update for onboard debugger OSJTAG.
   > Connect J21 pin 1-2, 3-4, 5-6, 7-8, to setup onboard debugger OSJTAG.
     Note: when use external debugger(connected to J14), leave all J21 pins open

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect USB cable between the host PC and the JM60 USB(J18) port on the target board. It setups OSJTAG and COM port in PC device manager.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board with OSJTAG debug configuration.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PDB ADC16 Trigger Example.
ADC16_DoAutoCalibration() Done.
ADC Full Range: 4096

Type any key into terminal to trigger the PDB and then trigger the ADC's conversion ...
PDB Delay Interrupt Counter: 1
ADC Interrupt Count: 1
ADC Value: 3714

Type any key into terminal to trigger the PDB and then trigger the ADC's conversion ...
PDB Delay Interrupt Counter: 2
ADC Interrupt Count: 2
ADC Value: 3717

Type any key into terminal to trigger the PDB and then trigger the ADC's conversion ...
PDB Delay Interrupt Counter: 3
ADC Interrupt Count: 3
ADC Value: 3731

Type any key into terminal to trigger the PDB and then trigger the ADC's conversion ...
PDB Delay Interrupt Counter: 4
ADC Interrupt Count: 4
ADC Value: 3723

Type any key into terminal to trigger the PDB and then trigger the ADC's conversion ...
PDB Delay Interrupt Counter: 5
ADC Interrupt Count: 5
ADC Value: 3729

...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
