Hardware requirements
=====================
- USB Type-C cable
- i.MX943-EVK board
- Personal Computer
- Power supply 24V
- Motor control board (FRDM-LVPMSM-FA)
- EnDat2.2 sensor

Board settings
==============
Insert FRDM-LVPMSM-FA board into Arduino interface of motor control 2.
Connect EnDat2.2 sensor and power supply to FRDM-LVPMSM-FA

EnDat2.2 connection:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE(FRDM-LVPMSM-FA)  CONNECTS TO  INSTANCE(EnDat2.2 sensor)
Pin Name        Board Location              Signal Name
ENC_CLK_P          J70-2                     CLOCK_P
ENC_CLK_N          J70-7                     CLOCK_N
ENC_DATA_IO_P      J70-4                     DATA_P
ENC_DATA_IO_N      J70-9                     DATA_N
VENC               J70-1                     UP - Power supply voltage
GND                J70-6                     GND
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

SW90 setting:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SW[1-2] OFF:ON Half duplex
SW[3]   OFF    Echo Enabled
SW[4]   ON     ENDAT is used.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

SW30 setting - Select the appropriate voltage based on the properties of the sensor

~~~~~~~~~~~~~~~~
SW[1] ON 3.3V
SW[2] ON 5.2V
SW[3] ON 9V
SW[4] ON 12V
~~~~~~~~~~~~~~~~

Prepare the Demo
===============
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW1(SYS_PWR) to power on the board.
2.  Connect a micro USB cable between the host PC and the J15(FTDI_DEBUG) USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either re-power up your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
The log below shows the output of the hello world demo in the terminal window:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Start ENDAT2.2 Diagnostic application
ENDAT2P2_SYS_CLOCK 100000000
EnDat Encoder: Multiturn rotary encoder with gears
Version: 2.2
encoder ID: 1164811 02  SN:  76758949
Position: 31 bits (singleturn: 19, multiturn: 12)
[resolution: 524288 M/rev]
Addational Info1 status: 0xd
Addational Info1: support Position Value 2
Addational Info1: support Temperature sensor 1
Addational Info1: support Temperature sensor 2
Addational Info2 status: 0x10
Addational Info2: support Operating status error sources

propagation time: 11 fsysclk
For EnDat2.2, change recovery time I to 3.75us

--------------------------------------------------------
      Select the encoder command from following
--------------------------------------------------------
  1: Encoder send position values
  2: Selection of memory area
  3: Encoder receive parameter
  4: Encoder send parameter
  5: Encoder receive reset
  6: Encoder send position with ADDINFO
  7: Encoder send position with ADDINFO select memory
  8: Encoder send position with ADDINFO recv param
  9: Encoder send position with ADDINFO send param
10: Encoder send position with ADDINFO recv err reset
11: Encoder receive communication command

100: Display encoder information
101: Display registers
102: Configure clock
103: Enable Delay compensation
104: Enable interrupt
105: Disable interrupt

200: Software strobe 10 times loop
201: Internal timer strobe 10 times loop
202: Hardware strobe 10 times loop
203: Test the performance
--------------------------------------------------------
Input command value:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
