Hardware requirements
=====================
- Type-C USB cable
- MCX-N9XX-EVK board
- Personal Computer

Board settings
============
Use the on-board MCULink and MCUXpresso IDE energy measurement function to measure the MCU current.
When use this method, please disconnect JP29.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the EVK board J5.
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
The log below shows the output of the power manager test demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Power Manager Test.

Normal Boot.

Please select the desired power mode:
        Press A to enter: Sleep
        Press B to enter: Deep Sleep
        Press C to enter: Power Down
        Press D to enter: Deep Power Down

Waiting for power mode select...

Selected to enter Sleep.

 Select the wake up timeout in seconds.
 The allowed range is 1s - 9s.
 Eg. enter 5 to wake up in 5 seconds.
 Waiting for input timeout value...
 Will wakeup in 5 seconds.
 Enable VBAT.
 De-init UART.
 Re-init UART.
 Disable VBAT.
Next Loop

Please select the desired power mode:
        Press A to enter: Sleep
        Press B to enter: Deep Sleep
        Press C to enter: Power Down
        Press D to enter: Deep Power Down

Waiting for power mode select...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
