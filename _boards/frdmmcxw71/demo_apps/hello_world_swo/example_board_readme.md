Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXW71 Board
- Personal Computer
- Jlink plus

Board settings
============

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the FRDM board J10. Add a jumper on JP7 then the MCU-Link will detect the presence of JP7 and disable the MCU-Link debugger, allowing the external J-Link to enter into play.
2.  Connect Jlink plus probe to the debug trace connector(J12).
3.  Download the program to the target board.
4.  Open JlinkerSWOView(Such as C:\Program Files (x86)\SEGGER\JLink\JlinkerSWOView.exe), select the target device, such as MCXW716C and make sure the core clock and swo clock frequency are equal to the demo setting, the SWO clock frequency is defined in hello_world_swo.c by macro DEMO_DEBUG_CONSOLE_SWO_BAUDRATE(4MHz by default).
5.  After swo/core clock frequency is measured successfully, press ok to continue, make sure the bit 0 is selected in "Data from stimulus port(s)" item.
6.  Press the reset button on your board.
7.  Press SW2.
8.  Note: If use MCUxpresso IDE, you need to set the SDK debug console to UART when importing projects at
    Project Options -> SDK Debug Console -> UART.

Running the demo
================
The log below shows the output of the hello world swo demo in the JlinkerSWOView window and the demo will print output periodically or when SW2 is pressed:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SWO: timer_trigger
SWO: timer_trigger
SWO: timer_trigger
SWO: hello_world
SWO: timer_trigger
SWO: timer_trigger
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
