Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXN947 board
- Personal Computer

Board settings
============
Two methods to measure the power consumption:
1. Measure the current of the MCU by connecting the ammeter to the J24 port.
2. Connect the J9 port of the MCU-Link Debug Probe and the J24 port of the FRDM board,
then use the energy measurement function of MCUXpresso IDE to measure the current of the MCU.
---------------------------------------------
|  Signal  |  FRDM Board  |  MCU-LinkProbe  |
---------------------------------------------
|  IDD_IN  |    J24-1     | J9 (current in) |
---------------------------------------------
|  IDD_OUT |    J24-2     | J9 (current out)|
---------------------------------------------
|   VSS    |    J10-4     |     J9 GND      |
---------------------------------------------
For MCU-Link Debug Probe, please refer to the link https://www.nxp.com/design/design-center/software/development-software/mcuxpresso-software-and-tools-/mcu-link-debug-probe:MCU-LINK

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the FRDM board J17.
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
