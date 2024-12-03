Overview
========
The Trusted Firmware M (TF-M) demo application provides a sanity check for the TF-M core and RoT services.
It can be used as a template for a new application which requires functionality of the TF-M Secure Services.

Toolchain supported
===================
- GCC ARM Embedded  8.3.1
- Keil MDK  5.29
- MCUXpresso  11.1.1

Hardware requirements
=====================
- Type-C USB cable
- LPCXpresso55S36 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J1) on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Use secure project to download the program to target board
    For IAR environment go to project options/Debugger/Images. Open file dialog for first extra image. Select file <application_name>_ns.out and click on open button. 
    Now you have absolute path in the path box. The flash loader requires absolute path to this file.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the TFM demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
[Sec Thread] Secure image initializing!
Booting TFM v1.0
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Device header file and secure/non-secure access to the peripherals
Both secure and non-secure project uses identical device header file. The access to secure and non-secure aliases for all peripherals
is managed using compiler macro __ARM_FEATURE_CMSE.

For secure project using <PERIPH_BASE> means access through secure alias (address bit A28=1), 
using <PERIPH_BASE>_NS means access through non-secure alias(address bit A28=0)
For non-secure project using <PERIPH_BASE> means access through non-secure alias (address bit A28=0). 
The non-secure project doesn't have access to secure memory or peripherals regions so the secure access is not defined.
Customization options
=====================

Known Issues
============
In MCUXpresso IDE, when the non-secure application is loaded to the device in axf format, the tfm example does not run as expected.

Workaround:
1. Click on "lpcxpresso55s36_tfm_regression_ns LinkServer Debug.launch" configuration in non-secure application
2. Go to the GUI Flash Tool and in the Program list select bin format and select base address 0x18000
3. Load again the non-secure application to the device 

