Hardware requirements
=====================
- USB Type-C cable
- i.MX943-EVK board
- Personal Computer
- Power supply 24V
- Motor control board (FRDM-LVPMSM-FA)
- BiSS sensor

Board settings
==============
Insert FRDM-LVPMSM-FA board into Arduino interface of motor control 2.
Connect BiSS sensor and power supply to FRDM-LVPMSM-FA

BiSS point-to-point connection:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE(FRDM-LVPMSM-FA)  CONNECTS TO  INSTANCE(BiSS sensor)
Pin Name        Board Location              Signal Name
ENC_CLK_P          J70-2                MASTER_CLOCK_INPUT_P
ENC_CLK_N          J70-7                MASTER_CLOCK_INPUT_N
ENC_DATA_IN_P      J70-3                SLAVE_DATA_P
ENC_DATA_IN_N      J70-8                SLAVE_DATA_N
VENC               J70-1                VDC - Power supply voltage
GND                J70-6                GND
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

BiSS daisy-chain connection:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE(FRDM-LVPMSM-FA)  CONNECTS TO  INSTANCE(BiSS sensor)
Pin Name        Board Location              Signal Name
ENC_CLK_P          J70-2                MASTER_CLOCK_INPUT_P
ENC_CLK_N          J70-7                MASTER_CLOCK_INPUT_N
ENC_DATA_IN_P      J70-3                SLAVE_DATA_P
ENC_DATA_IN_N      J70-8                SLAVE_DATA_N
ENC_DATA_IO_P      J70-4                SLAVE_DATA_INPUT_P
ENC_DATA_IO_N      J70-9                SLAVE_DATA_INPUT_N
VENC               J70-1                VDC - Power supply voltage
GND                J70-6                GND
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

SW90 setting:
~~~~~~~~~~~~~~~~~~~~~~~~~~~
SW[1-2] ON:OFF Full duplex
SW[3]   OFF    Echo Enabled
SW[4]   ON     ENDAT is used.
~~~~~~~~~~~~~~~~~~~~~~~~~~~

SW30 setting - Select the appropriate voltage based on the properties of the sensor

~~~~~~~~~~~~~~~~~~~~~~~~~~~
SW[1] ON 3.3V
SW[2] ON 5.2V
SW[3] ON 9V
SW[4] ON 12V
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Jumpers:
~~~~~~~~~~~~~~~
J300: pin1 <--> pin2
~~~~~~~~~~~~~~~

Prepare the Demo
===============
1.  Connect a USB cable between the host PC and the MCU-LINK USB port on the target board.
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
The log below shows the output of the demo in the terminal window:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BiSS test example.
This example use one board as BiSS master and connect to the BiSS encoder.
Please make sure you make the correct line connection. Basically, the connection is:
   MA       --   BISS Clock Line Output
   MO       --   BISS Data Line Output
   SL       --   BISS Data Line Input

Select the encoder command from following:
1: Dump slave position
2: Dump registers
3: Enable instruction trigger
4: Enable AGS repetition trigger
5: Enable timeout trigger
6: GETSENS pin trigger
7: Reset BiSS-C
8: Re-scan BiSS bus
9: Dump slave information
0: Test Performance
Input command values:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
