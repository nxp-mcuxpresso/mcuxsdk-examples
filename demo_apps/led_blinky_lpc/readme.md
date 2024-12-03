# led_blinky_lpc

## Overview
The LED Blinky demo application provides a sanity check for the new SDK build environments and board bring up. The LED Blinky demo 
uses the systick interrupt to realize the function of timing delay. The example takes turns to shine the LED. The purpose of this 
demo is to provide a simple project for debugging and further development.
The code of this demo has been prepared and updated for use with the MCUXpresso Configuration Tools (Pins/Clocks/Peripherals).

The LED Blinky demo mainly is to bring up the new SDK build environments on the board.
This demo will initialize the pin configuration, then configure System Tick to generate 
1ms interrupt. When 1000 interrupts occur(1s), the pin output of LED is toggled. We can 
see that LED is blinking on the board.

## Supported Boards
- [LPC845BREAKOUT](../../_boards/lpc845breakout/demo_apps/led_blinky/example_board_readme.md)
- [LPCXpresso54S018](../../_boards/lpcxpresso54s018/demo_apps/led_blinky/example_board_readme.md)
- [LPCXpresso54S018M](../../_boards/lpcxpresso54s018m/demo_apps/led_blinky/example_board_readme.md)
- [LPCXpresso55S06](../../_boards/lpcxpresso55s06/demo_apps/led_blinky/example_board_readme.md)
- [LPCXpresso55S16](../../_boards/lpcxpresso55s16/demo_apps/led_blinky/example_board_readme.md)
- [LPCXpresso55S28](../../_boards/lpcxpresso55s28/demo_apps/led_blinky/example_board_readme.md)
- [LPCXpresso55S36](../../_boards/lpcxpresso55s36/demo_apps/led_blinky/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/demo_apps/led_blinky/example_board_readme.md)
- [LPCXpresso802](../../_boards/lpcxpresso802/demo_apps/led_blinky/example_board_readme.md)
- [LPCXpresso804](../../_boards/lpcxpresso804/demo_apps/led_blinky/example_board_readme.md)
- [LPCXpresso824MAX](../../_boards/lpcxpresso824max/demo_apps/led_blinky/example_board_readme.md)
- [LPCXpresso845MAX](../../_boards/lpcxpresso845max/demo_apps/led_blinky/example_board_readme.md)
- [LPCXpresso860MAX](../../_boards/lpcxpresso860max/demo_apps/led_blinky/example_board_readme.md)
