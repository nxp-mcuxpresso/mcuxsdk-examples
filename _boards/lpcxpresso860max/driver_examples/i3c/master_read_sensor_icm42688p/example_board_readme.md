Hardware requirements
=====================
- Micro USB cable
- LPCXpresso860max board
- Personal Computer

Board settings
============
Note: Please check if the U5 is populated on the board, this demonstration requires this sensor for operation.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J4) on the board
2.  Open a serial terminal with the following settings:
    - 9600 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Press reset button to begin running the demo.

Running the demo
================
The log below shows the output of the example in the terminal window. You could tap the sensor on board(U5) and see sensor read stops, each tap will generate a new IBI data print log "Received slave IBI request.". 
~~~~~~~~~~~~~~~~~~~~~
I3C master read sensor data example.

I3C master do dynamic address assignment to the sensor slave.
I3C master dynamic address assignment done, sensor address: 0x 8.

Sensor reset is done, re-assgin dynamic address.
I3C master dynamic address assignment done, sensor address: 0x 8.

Sensor Data: ACCEL X 11, Y -4, Z 2077; GYRO X -23, Y -36, Z 1171.
Sensor Data: ACCEL X 11, Y -2, Z 2075; GYRO X -23, Y -36, Z 1171.
Sensor Data: ACCEL X 15, Y -2, Z 2072; GYRO X -11, Y -17, Z 878.
Sensor Data: ACCEL X 17, Y -3, Z 2070; GYRO X 16, Y 9, Z 139.
......
~~~~~~~~~~~~~~~~~~~~~
