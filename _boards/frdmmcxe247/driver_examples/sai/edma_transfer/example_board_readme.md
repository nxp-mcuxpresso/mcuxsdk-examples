Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXE247 board
- Personal Computer
- Headphones
- Waveshare WM8960 Audio Codec board

Board settings
============
For the successful audio output, make the following connections between the FRDM-MCXE247 board and the Waveshare WM8960 Audio Codec board:
- WM8960 I2C_SCL    ←→    J2 Pin 20 (LPI2C1_SDA)
- WM8960 I2C_SDA    ←→    J2 Pin 18 (LPI2C1_SCL)
- WM8960 VCC_3V3    ←→    J2 Pin 16 (VVDA_MCU)
- WM8960 GND        ←→    J2 Pin 14 (GND)
- WM8960 I2S_CLK    ←→    J1 Pin 1  (SAI0_BCLK)
- WM8960 I2S_LRCLK  ←→    J1 Pin 11 (SAI0_SYNC)
- WM8960 I2S_DAC    ←→    J1 Pin 5  (SAI0_D1-TX_DAT)
- connect the headphones to the WM8960 Audio Codec board's headphone jack

Prepare the Demo
===============
1.  Connect a USB cable between the PC host and the MCU-Link USB port on the board.
2.  Open a serial terminal with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
Note: This demo outputs 1000Hz sine wave audio signal.
When the demo runs successfully, you can hear the tone and the log would be seen on the MCU-Link terminal like:

~~~~~~~~~~~~~~~~~~~
SAI EDMA example started!
SAI EDMA example finished!
~~~~~~~~~~~~~~~~~~~
