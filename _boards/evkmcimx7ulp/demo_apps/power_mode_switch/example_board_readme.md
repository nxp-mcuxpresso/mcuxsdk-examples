Hardware requirements
=====================
- Micro USB cable
- MCIMX7ULP-EVK board
- J-Link Debug Probe
- 5V power supply
- Personal Computer

Board settings
============
No special settings are required.

#### Please note this application can support running with Linux BSP! And gcc debug target
exceeds the RAM size so it's not available! ####

Prepare the Demo
===============
1.  Connect 5V power supply and J-Link Debug Probe to the board, switch SW1 to power on the board.
2.  Connect a micro USB cable between the host PC and the J6 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Generate image file with imgutil and write to QSPI flash with U-Boot. For details, please refer to Getting Started with SDK v.2.0 for i.MX 7ULP Derivatives (Doc No: SDK20IMX7ULPGSUG)
5.  Press the reset button on your board to begin running the demo.

Running the demo
================
When running the demo, the debug console shows the menu to command the MCU to the target power mode, or query and set other system power behavior. The power mode switch demo need to run with Linux accordingly, so please make sure the power mode crossings between Cortex-A7 and Cortex-M4 are valid.

NOTE: Only input when the demo asks for input. Input entered at any other time might cause the debug console to overflow and receive the wrong input value.

Remarkable configuration of the application (in power_mode_switch.c):
"SYSTICK_LLWU_WAKEUP":
  The demo leverages LPTMR0 as systick timer, and supports FreeRTOS tickless idle. In tickless idle mode, LPTMR0 takes LPO 1kHz clock as clock source and will overflow in 65 seconds. If setting SYSTICK_LLWU_WAKEUP to "true", it means systick can wake up system in LLS/VLLS so that OS event like task delay or semaphore timeout may wake up SoC in addition to the wakeup source selected in application menu. Even no OS event occurs, the system will be woken up from LLS/VLLS every 65 seconds also to avoid LPTMR0 overflow which leads to systick loss. If setting SYSTICK_LLWU_WAKEUP to "false", then systick(LPTMR0) cannot wakeup SoC in LLS/VLLS.
"APP_ENABLE_GPIO_PAD_LOW_POWER":
  This is an IO low power switch. If setting to "1", then the SoC IO leakage can be optimized with the limitation that only fixed voltage can be applied to the IO pads. Please read "GPIO pads operating range configuration" in Reference Manual SIM module carefully to avoid malfunction or even SoC pad damage.

~~~~~~~~~~~~~~~~~~~~~
Task 1 is working now

MCU wakeup source 0x6...

####################  Power Mode Switch Task ####################
    Build Time: Apr  9 2021--15:24:08
    Core Clock: 115200000Hz
    Power mode: RUN

Select the desired operation

Press  A for enter: RUN      - Normal RUN mode
Press  B for enter: WAIT     - Wait mode
Press  C for enter: STOP     - Stop mode
Press  D for enter: VLPR     - Very Low Power Run mode
Press  E for enter: VLPW     - Very Low Power Wait mode
Press  F for enter: VLPS     - Very Low Power Stop mode
Press  G for enter: HSRUN    - High Speed RUN mode
Press  H for enter: LLS      - Low Leakage Stop mode
Press  I for enter: VLLS     - Very Low Leakage Stop mode
Press  Q for query CA7 core power status.
Press  W for wake up CA7 core in VLLS/VLPS.
Press  T for reboot CA7 core.
Press  U for shutdown CA7 core.
Press  V for boot CA7 core.
Press  R for read PF1550 Register.
Press  S for set PF1550 Register.
Press  Z for enhanced power configuration.

Waiting for power mode select..
~~~~~~~~~~~~~~~~~~~~~
