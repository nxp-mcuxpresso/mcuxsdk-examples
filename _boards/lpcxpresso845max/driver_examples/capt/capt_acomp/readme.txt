## Overview
-----------
The CAPT ACOMP Example shows the Analog comparator measurement way to use CAPT driver and help user with a quick start.

In this example, user should indicate  the set of X pins to use by writing to the XPINSEL field in the
control register. The CAPT work in Analog comparator measurement menthod. In Measure Voltage state, 
the module samples the analog comparator output, which is connected internally to the module. The analog
comparator must be enabled and properly configured, and one of the comparator analog inputs must be enabled
and connected to the YH port pin. On some devices the YH port pin and an analog comparator input may share
a pad or pin. Otherwise, the YH port pin and the analog comparator input pin must be connected externally (wire-OR’d).
Until the voltage on the analog comparator input increases above the configured threshold of the comparator, 
the module will sample ‘0’, above that it will sample ‘1’ (that is, the analog comparator has “triggered”.
Then we would handle the CAPT ISR for triggered events or no-triggered events.

## Functional Description
-------------------------
Step1: Enable the fixed-pin functions CAPT_YH, CAPT_YL, and any of CAPT_X0,
       CAPT_X1, etc. that will be used, by writing to the appropriate PINENABLE register(s)
       in the SWM.
Step2: Disable pull-ups and pull-downs on those pads by writing to the appropriate registers
       in the IOCON.
Step3：Enable the bus clock to the Capacitive Touch module by setting the appropriate bit in
       one of the SYSAHBCLKCTRL registers in SYSCON.
Step4: Provide a function clock (FCLK) to the module by writing to the CAPTCLKSEL register
       in SYSCON.
Step5: Identify to the module the set of X pins to use by writing to the XPINSEL field in the
       control register. 
Step6: Set the threshold, and various other timing and counting parameters, by writing to the
       POLL_TCNT register (an initial POLL_NOW polling operation can be launched in
       order for the baseline no-touch trigger point to be determined).
Step7: Enable interrupts and/or DMA requests in the CTRL register.
Step8: Handle CAPT ISR.

## Toolchain Supported
---------------------
- IAR embedded Workbench 8.50.5
- Keil MDK 5.31
- GCC ARM Embedded  9.2.1
- MCUXpresso 11.2.0

## Hardware Requirements
------------------------
- Micro USB cable
- LPC845 MP board
- LPC845 BM board
- Personal Computer

## Board Settings
------------------------
Connect LPC845 MP board and LPC845 BM board

## Run the Project
------------------------
Run this example by performing the following steps:

1. Connect a micro USB cable between the PC host and the CMSIS DAP port(CN2 on the 
   LPCXpresso804 board) for receiving debug information.

2. Open a serial terminal in PC(for example PUTTY) with the following settings:
   - 9600 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control

3. Compile and download the program to the target board.
   More information about how to compile and program the project can refer to 

   [Getting Started with MCUXpresso SDK](../../../../../docs/Getting Started with MCUXpresso SDK.pdf).

4. Start the slave board on another board first, then launch the debugger in your IDE to
   begin running this project.

5. Monitor the information on the debug console.

## Expected Result
------------------------
When touch the electrode plate, the corresponding LED would turn on.

