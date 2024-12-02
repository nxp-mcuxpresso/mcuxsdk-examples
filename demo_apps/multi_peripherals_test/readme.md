# multi_peripherals_test

## Overview
The multi-peripherals-test demo application does the basic peripheral test on
board. This demo will test audio, usb, led, button, sdcard, accelerometer and rtc.
To test audio function, connect an audio source, for example from a mobile phone
or PC, to Line-IN connector, connect a headphone or speaker to Line-Out connector. Then
the input audio can be heared in the headphone or speaker.
To test usb, connect a USB cable between the PC and board's high speed connector,
the cursor on the PC will move in a square pattern.
To test sdcard function, insert a uSD memory card in to the connector, then
the test result will be printed, please make sure the message is not a failed
message.
Please verify the green LED in the RGB-LED flashed with a 1Hz rate. This indicates
that the RTC crystal works.

## Supported Boards
- [LPCXpresso55S16](../../_boards/lpcxpresso55s16/demo_apps/multi_peripherals_test/example_board_readme.md)
- [LPCXpresso55S28](../../_boards/lpcxpresso55s28/demo_apps/multi_peripherals_test/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/demo_apps/multi_peripherals_test/example_board_readme.md)
