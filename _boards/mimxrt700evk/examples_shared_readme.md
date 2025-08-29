This file contains the general information which can be shared by the examples on this board.
The example specific information can be found in *readme.md* or *example_board_readme.md*.

General Example Settings
========================
Hardware requirements
---------------------
- Micro USB cable
- MIMXRT700-EVK board
- Personal Computer

Prepare the Demo
---------------
1.  Connect a micro USB cable between the PC host and the USB port on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

USB example settings
===================
- For eUSB mode, please disable USB_DEVICE_CONFIG_CHARGER_DETECT macro and USB_DEVICE_CONFIG_DETACH_ENABLE. RT700 eUSB does not support DCD feature (device charger detect) and VBUS detection.
- For eUSB repeater mode, please populate R417 and R418, and remove R409 and R410. This is default setting on the board. Besides, please use external 5V power adapter to the board to make sure the power supply for eUSB PTN3222 is enough.
- For eUSB native mode, please populate R409 and R410, and remove R417 and R418.
