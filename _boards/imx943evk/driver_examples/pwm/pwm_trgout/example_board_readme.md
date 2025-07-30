Hardware requirements
=====================
- Mini/micro USB cable
- IMX943-EVK board
- Personal Computer
- Oscilloscope

Board settings
==============

Prepare the Demo
================
1. Connect a USB cable between the PC host and the OpenSDA USB port on the board.
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
When the example runs successfully, the following message is displayed in the terminal:
~~~~~~~~~~~~~~~~~~~~~~~~
IPSYNC trigger signal connected!
FlexPWM driver example
LPIT init
Starting channel No.0 ...
 Channel No.0 interrupt is occurred !
 Channel No.0 interrupt is occurred !
 ...
~~~~~~~~~~~~~~~~~~~~~~~

Delete the below line in code to disconnect the trigger signal, the LPIT cannot be started.
	"XBAR_SetSignalsConnection(kXBAR1_InputFlexpwm1Mux0Trigger0, kXBAR1_OutputLpit1LpitExtTrigIn0)"

Log displayed in terminal:
~~~~~~~~~~~~~~~~~~~~~~~~
IPSYNC trigger signal connected!
FlexPWM driver example
LPIT init
Starting channel No.0 ...
~~~~~~~~~~~~~~~~~~~~~~~
