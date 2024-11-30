Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXN236 board
- Personal Computer

Board settings
============

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
        Press A to enter: Sleep (IDD_SLEEP)
        Press B to enter: Deep Sleep (IDD_DSLEEP_LP)
        Press C to enter: Power Down (IDD_PDOWN_RET_0V7)
        Press D to enter: Deep Power Down (IDD_DPOWN_32K)

Waiting for power mode select...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Power consumption in sleep mode corresponds to IDD_SLEEP on the datesheet.
Power consumption in deep sleep mode corresponds to IDD_DSLEEP_LP on the datesheet.
Power consumption in power down mode corresponds to IDD_PDOWN_RET_0V7 on the datesheet.
Power consumption in power down mode corresponds to IDD_DPOWN_32K on the datesheet.

---------------------------------------------------------------------------
|Symbol           |Power consumption in datasheet|Tested Power consumption|
|-----------------|------------------------------|------------------------|
|IDD_SLEEP        |             1.48mA           |         1.65mA         |
|IDD_DSLEEP_LP    |             0.12mA           |         0.09mA         |
|IDD_PDOWN_RET_0V7|             2.52uA           |         2.7uA          |
|IDD_DPOWN_32K    |             1.28uA           |         1.4uA          |
---------------------------------------------------------------------------
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
