Hardware requirements
=====================
- USB Type-C cable
- i.MX943-EVK board
- Personal Computer
- Power supply 24V
- Motor control board (FRDM-LVPMSM-FA)
- Hiperface sensor

Board settings
==============
Insert FRDM-LVPMSM-FA board into Arduino interface of motor control 2.
Connect EnDat3 sensor and power supply to FRDM-LVPMSM-FA

EnDat3 connection:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Pin Name        Board Location              Signal Name
ENC_DATA_IO_VENC_P      J70-5                     DATA_P
ENC_DATA_IO_CENC_N      J70-10                    DATA_N

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

The Jumper settings:
~~~~~~~~~~~~~~~~
To make the example work, the below jumpers should be connected:
Jumper setting:
    J72:   pin3 <--> pin 1
    J72:   pin4 <--> pin 2
    J73:   pin2 <--> pin 1
    J300:  pin2 <--> pin 1
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