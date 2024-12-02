Hardware requirements
=====================
- Micro USB cable
- FRDM-KE17Z512 board
- Personal Computer

Board settings
==============
This example project uses ADC12 instance 0 to sample and convert the voltage signal input from channel 0(PTE9).
So the voltage signal input from PTE9(J2-19) should be changed to observe the dynamical conversion result.

Prepare the Demo
================
1. Connect a USB cable between the PC host and the MCU-LINK USB port on the board.
2. Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the demo runs successfully, the log would be seen on the MCU-LINK terminal like:

ADC12 CONTINUOUS EDMA DEMO
ADC calibration is done!
ADC Full Range: XXXX
Press any key to get user channel's ADC value ...
ADC value: 2824
ADC value: 2709
ADC value: 2757
ADC value: 2834
ADC value: 2798
ADC value: 2617
ADC value: 2792
