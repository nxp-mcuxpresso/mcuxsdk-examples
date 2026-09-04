Hardware requirements
=====================
- Type-C USB cable
- MIMXRT2660-EVK
- Personal Computer
- LCM_RGB_5INCH DPI panel
- OV5640 module

Board settings
==============
1. Connect the OV5640 camera module to J4.
2. Connect the LCM_RGB_5INCH DPI panel to J98.
3. Use 5V power supply with J6 1-2 connected instead of usb cable.

Prepare the Demo
================
1.  Connect a USB cable between the host PC and the OpenSDA USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the demo runs successfully, the camera preview is shown on the panel through
the DCIF PDI low latency path.

Note
================
For PDI demo to work, the camera and the display frame rate must be the same. The highest frame rate
of OV5640 is 30fps, however in 30fps if using the original parameters of LCM_RGB_5INCH panel(800x480),
the pixel clock is 12.3MHz and the panel does not support pixel clock this low. So for this demo to
work the parameters of this panel is changed to 1280x800(and all the hsync and vsync parameters) to
increase the pixel clock. In this way only partial of the camera frame is shown on panel. Demo enablement
on different SoC shall pay attention to this special handling.