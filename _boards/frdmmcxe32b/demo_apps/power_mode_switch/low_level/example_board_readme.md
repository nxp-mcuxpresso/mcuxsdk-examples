Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXE32B board
- Personal Computer
- External power supply capable of providing a voltage below 1.6V

Board settings
============

Since the LPCMP1 round robin wake-up interrupt is enabled as one of the wake up source and the PTA0 is selected as the input reference Input, external powerful supply shall be connected to the PTA0 to provide voltage lower than 1.6V.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Pin Name    Board Location     checker channel number       pre-set value            trigger
PTA0            J9-3                channel 0                    0              input < DAC output
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Prepare the Demo
===============
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When running the demo, the debug console shows the menu to command the MCU to the target power mode.

The MCXE32B supports 2 modes:
1. Run mode (FPM): Main operation mode having full-chip performance and a higher current consumption as compared to Standby mode.
2. Standby mode (LPM): Low-performance mode of the chip in which the Run domain is turned off. Most of the cores and peripherals turn off in this mode.

When starting from POR, the application shows "Normal Boot".

```
Normal Boot.

###########################    Power Mode Switch Demo    ###########################
   Core Clock = 160000000Hz
   Power mode: Run in FPM

Select the desired operation

Press A to enter: Run mode
Press B to enter: Standby mode

Waiting for power mode select...
```

When waking up from low power mode through wakeup source, the application shows "Recover from standby mode."

```
Recover from standby mode.

###########################    Power Mode Switch Demo    ###########################
   Core Clock = 160000000Hz
   Power mode: Run in FPM

Select the desired operation

Press A to enter: Run mode
Press B to enter: Standby mode

Waiting for power mode select...
```

There are 6 wake up sources supported

```
	Press A to select SWT as wakeup source;
	Press B to select RTC API as wakeup source;
	Press C to select RTC as wakeup source;
	Press D to select LPCMP round robin wake-up interrupt as wakeup source;
	Press E to select RTI as wakeup source;
	Press F to select Button as wakeup source;
```

For A,B,C,D,E, you need to put the wake up time out(second):

```
Select the wake up timeout in seconds.
The allowed range is 1s ~ 9s.
Eg. enter 5 to wake up in 5 seconds.

Waiting for input timeout value...
```

For option D, the wake up time is the LPCMP round robin operation trigger cycle.

For F, you need to press SW2 to wake up the MCU:

```
Wakeup Button is selected as wakeup source.
Please press SW2 to wakeup.
```

Please only input when the demo asks for input. Input entered at any other time might cause the debug console to overflow and receive the wrong input value.

Note in FRDM-MCXE318 board, PTC5/JTAG_TDI-MCU-LINK has been pull up with VDD_HV_A_MCU_IO_REF and connect a 100k resistance to DGND. In MCXE32B device, the default configuration for pin PTC5 is pulling up. Those pulls work together to mistakenly give the PTC5 about 80mV voltage fluctuation which will greatly increase the current in low power mode. To avoid such voltage fluctuation in PTC5, the demo code has pulled down the PTC5 with `APP_PullDownJTAGTDI` function.

 

