Hardware requirements
=====================
- Micro USB cable
- RD-RW61X-BGA board
- Personal Computer
- Adafruit TFT LCD shield w/Cap Touch
- LCD-PAR-S035 panel

Board settings
============
There are different versions of Adafruit 2.8" TFT LCD shields. The shields marked
v2.0 works directly with this project. For the other shields, please solder
the center pin of IOREF pads to the 3.3V pad, and solder the pads 11, 12, and 13.
See the link for details:
https://community.nxp.com/t5/MCUXpresso-Community-Articles/Modifying-the-latest-Adafruit-2-8-quot-LCD-for-SDK-graphics/ba-p/1131104

To use Adafruit panel:
- Attach the LCD shield to the board Arduino header.
- Wire R123, R124, R125, R12. Remove R9, R11, R21.

To use LCD-PAR-S035:
- Short JP3, JP44, JP50
- Wire R624, R625, R626, R627, R629, R630, R631, R616, R662, R663, R664, R665
- Connect LCD-PAR-S035 to header J23, J23-1 match LCD J1-1
- Set the switch SW1 on LCD-PAR-S035 to 011 (IM[2:0), note the left one is IM[0], right one is IM[2].

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the MCU-Link USB port (J7) on the board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the example.

Select the panel
================
LCD-PAR-S035 panel is used by default, follow the steps to configure the sw to support different panel.

When Kconfig is used
-------------
Select the panel in Kconfig "Panel Selection".

When Package is used
-------------
To use Adafruit panel: define DEMO_PANEL as 0 in mcux_config.h.
To use LCD-PAR-S035: define DEMO_PANEL as 1 in mcux_config.h.

Running the demo
===============
If this example runs correctly, the sample GUI is displayed.
