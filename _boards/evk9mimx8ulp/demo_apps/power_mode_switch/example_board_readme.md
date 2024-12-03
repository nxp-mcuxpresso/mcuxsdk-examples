Hardware requirements
=====================
- Micro USB cable
- MIMX8ULP-EVK/EVK9 board
- J-Link Debug Probe
- 5V power supply
- Personal Computer

Board settings
============
No special settings are required.

**Please note this application can support running with Linux BSP!**

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
5.  Press the reset button on your board to begin running the demo.

Running the demo
================
When running the demo, the debug console shows the menu to command the MCU to the target power mode, or query and set other system power behavior. The power mode switch demo need to run with Linux accordingly, so please make sure the power mode crossings between Cortex-A35 and Cortex-M33 are valid.

NOTE: Only input when the demo asks for input. Input entered at any other time might cause the debug console to overflow and receive the wrong input value.

Note: RTD low power mode is based on Cortex M33 sleep mechanism implementation, M33 provides sleep and deep sleep mode. RTD wait: cortex m33 sleep, RTD stop: cortex m33 deep sleep, RTD sleep/deep sleep/power down/deep power down = cortex m33 deep sleep + different power saving operation.

NOTE: For 'F' option (Power Down mode), it's not supported on A0.1 chip. Please try it on A1 chip instead.

NOTE: For 'G' option (Deep Power Down mode), it's not supported on Single Boot mode. Please try it on Dual Boot or Low Power Boot mode instead.

NOTE: For 'T' option, M33 side will force reboot A35 core.

NOTE: For 'U' and 'V' option (shutdown/boot CA35 core), please execute 'poweroff' command in A35 BSP Linux console in advance, then press 'U' on M33 console to shutdown A35, then press 'V' to boot A35 again.

Remarkable configuration of the application (in power_mode_switch.c):
"SYSTICK_WUU_WAKEUP":
  The demo leverages LPTMR0 as systick timer, and supports FreeRTOS tickless idle. In tickless idle mode, LPTMR0 takes RTC 1kHz clock as clock source and will overflow in 65 seconds. If setting SYSTICK_WUU_WAKEUP to "true", it means systick can wake up system in PD/DPD so that OS event like task delay or semaphore timeout may wake up SoC in addition to the wakeup source selected in application menu. Even no OS event occurs, the system will be woken up from PD/DPD every 65 seconds also to avoid LPTMR0 overflow which leads to systick loss. If setting SYSTICK_WUU_WAKEUP to "false", then systick(LPTMR0) cannot wakeup SoC in PD/DPD.

~~~~~~~~~~~~~~~~~~~~~
AD entered Active mode
Start SRTM communication
Task 1 is working now

####################  Power Mode Switch Task ####################

    Build Time: May 11 2023--16:54:05
    Core Clock: 160000000Hz
    Boot Type: Single Boot Type

Select the desired operation

Press  A to enter: Active mode
Press  B to enter: Cortex M33 Wait mode
Press  C to enter: Cortex M33 STOP mode
Press  D to enter: Sleep mode
Press  E to enter: Deep Sleep mode
Press  F to enter: Power Down(PD) mode
Press  G to enter: Deep Power Down(DPD) mode
Press  W for wake up CA35 core from PD/DPD mode
Press  T for reboot CA35 core
Press  U for shutdown CA35 core.
Press  V for boot CA35 core.
Press  S for showing supported LPM Mode Combination.
Press  M for switch Voltage Drive Mode between OD/ND/UD.
Press  N for supporting Deep Sleep Mode(Pls set it when the option IMX8ULP_DSL_SUPPORT of TF-A is enabled) of Linux. support_dsl_for_apd = 0

Waiting for power mode select..
~~~~~~~~~~~~~~~~~~~~~
