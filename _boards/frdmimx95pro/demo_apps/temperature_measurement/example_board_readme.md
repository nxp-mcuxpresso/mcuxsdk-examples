Hardware requirements
=====================
- Micro USB cable
- FRDM-IMX95 board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer

Board settings
============
No special settings are required.

Note
====
Cannot get temperature from sensor temp_a55. It's an expected behaviour.

Prepare the Demo
===============
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW4 to power on the board.
2.  Connect a micro USB cable between the host PC and the J31 USB port on the target board.
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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Temperature measurement example.
 desc[0].sensorId = 0x0
 desc[0].name = temp_ana
 desc[1].sensorId = 0x1
 desc[1].name = temp_a55
 desc[2].sensorId = 0x2
 desc[2].name = temp_pf09
 Pls input a number to choose sensor to get data:(valid input is 0-2)0
 Sensor(desc index is 0) temp_ana is selected
 Sensor is ready to read, press enter key to get the temperature.
 Current temperature is 39 degrees Celsius
 Pls input a number to choose sensor to get data:(valid input is 0-2)1
 Sensor(desc index is 1) temp_a55 is selected
 Sensor configuration failed!
 Pls input a number to choose sensor to get data:(valid input is 0-2)2
 Sensor(desc index is 2) temp_pf09 is selected
 Sensor is ready to read, press enter key to get the temperature.
 Current temperature is 105 degrees Celsius
 Pls input a number to choose sensor to get data:(valid input is 0-2)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
