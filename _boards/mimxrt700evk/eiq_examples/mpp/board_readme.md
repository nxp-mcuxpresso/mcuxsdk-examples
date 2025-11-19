Hardware requirements
=====================
- MIMXRT700-EVK board (versions A1, A2, A3, B0)
- Mini/micro USB cable
- Personal computer
- RK055AHD091 or RK055MHD091 display
- OV7670(Flexio camera) or Logitech C920 PRO HD WEBCAM(USB camera)

Board settings
==============
Connect external 5V power to J45
Connect the display to J25
If using OV7670 flexio camera, connect it to J53(3.3V on pin1) and move JP7 to 2-3 to enable 3.3v FlexIO.
If using USB camera(Logitech C920 PRO HD WEBCAM), use a USB OTG adapter to connect it to J40(USB_OTGO). If nothing appears
on the screen after flashing, try to unplug-re-plug the USB cable.

Prepare the Demo
================
1. Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Build the project. The project expects the RK055MHD091 panel by default. To use the RK055AHD091 panel,
    change #define DEMO_PANEL DEMO_PANEL_RK055MHD091 to #define DEMO_PANEL MIPI_PANEL_RK055AHD091
    in display_support.h. The default camera is flexio OV7670, when using USB camera define USE_USB_CAMERA in reconfig.cmake.
4. Download the program to the target board using flasher or debugger utility.
5. Press the reset button (SW2) on the board to begin running the demo (CPU reset via MCU-Link/CMSIS may not work).
