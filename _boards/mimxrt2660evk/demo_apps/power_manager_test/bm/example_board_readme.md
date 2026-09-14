Hardware requirements
=====================
- USB cable (for debug console)
- MIMXRT2660EVK board
- Personal Computer

Board settings
============
No special board settings are required.

Prepare the Demo
================
1.  Connect a USB cable between the PC host and the debug USB port on the board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Wakeup source
=============
All 5 power modes wake via the VBAT LPTMR after a fixed 5-second timeout (`APP_LPTMR_TIMEOUT_MS`
in `app.h`). The VBAT LPTMR lives in the always-on VBAT domain, so the same timer wakes the
board from every mode, including Power Down and Deep Power Down (both of which reset on
wakeup -- the demo re-prints the prompt after reboot).

Running the demo
================
The log below shows the output of the power manager test demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Power Manager Test.

Normal Boot.

Please select the desired power mode:
        Press A to enter: WFI
        Press B to enter: Sleep
        Press C to enter: Deep Sleep
        Press D to enter: Power Down
        Press E to enter: Deep Power Down

Waiting for power mode select...

Next Loop

Please select the desired power mode:
        Press A to enter: WFI
        Press B to enter: Sleep
        Press C to enter: Deep Sleep
        Press D to enter: Power Down
        Press E to enter: Deep Power Down

Waiting for power mode select...

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
