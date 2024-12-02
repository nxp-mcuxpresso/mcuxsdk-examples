# secure_rgpio_s

## Overview
The Secure GPIO demo application demonstrates using of TRDC and GPIO PIN control.
TRDC allows user to control GPIO peripheral into secure world or into normal world.
The Pin Control Non-Secure Register (PCNS) configures secure/non-secure access protection for each pin.
Thus for all pins, user can select whether the pin is controlled from secure or non-secure domain.

This Secure GPIO demo uses GPIO peripheral to read SW7 button from secure world and normal world.
If SW7 button is pressed (logical zero is read) the RGPIOA PIN15 is pulled high in secure world
and concurrently the RGPIOA PIN18 is low from normal world.

The second part of the demo is GPIO PIN control Non-secure access protection feature. This feature is controlled by
button SW8. If the SW8 button is released, The SW7 is only allowed by software in secure world. SW7 is read zero
(also is zero When SW7 pressed) in normal world. If the SW8 button is pressed, the SW7 is for normal world. Thus normal
world can read state of SW7 button. The RGPIO PIN18 is pulled high while SW7 is released. Meanwhile the SW7 is pressed
PIN18 is low and PIN15 is pulled high.

## Supported Boards
- [EVK9-MIMX8ULP](../../_boards/evk9mimx8ulp/trustzone_examples/secure_rgpio/secure_rgpio_s/example_board_readme.md)
- [EVK-MIMX8ULP](../../_boards/evkmimx8ulp/trustzone_examples/secure_rgpio/secure_rgpio_s/example_board_readme.md)
