Hardware requirements
=====================
- Micro USB cable
- MIMX8ULP-EVK/EVK9 board
- J-Link Debug Probe
- 5V power supply
- Oscilloscope
- Personal Computer

Board settings
==============
No special settings are required.

**Please note this application can't support running with Linux BSP!**

Prepare the Demo
===============
1.  Connect 5V power supply and J-Link Debug Probe to the board, switch SW10 to power on the board.
2.  Connect a micro USB cable between the host PC and the J17 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

Running the demo
================
When the example runs successfully, if the input duty cycle range is 0~100,
You can see similar information from the terminal as follows:
~~~~~~~~~~~~
FLEXIO_PWM demo start.

Please input a value (0 - 100) to set duty cycle: 10
Input value is 10

PWM duty cycle is: 10
PWM leave is: 0

Please input a value (0 - 100) to set duty cycle: 150
Input value is 150
Your value is output of range.
Set pwm output to IDLE.

Please input pwm idle status (0 or 1): 1
Input IDLE state value is 1

PWM leave is: 1 

Please input a value (0 - 100) to set duty cycle: 
......
~~~~~~~~~~~~
Note:
1. The duty cycle of the PWM is variable, except for the idle state, 0 and 100.
2. If the input duty cycle range exceeds 100, the pwm will be set to idle state
3. If the set pwm idle value is out of range, it will output "Your value is output of range."
   and then return to set the duty cycle.
