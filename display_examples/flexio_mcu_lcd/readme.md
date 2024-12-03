# flexio_mcu_lcd

## Overview
The FlexIO MCU LCD demo application demonstrates how to use SDK FlexIO to drive
MCU interface LCD.

The example displays one picture and an arrow at the right bottom of the screen.
When press the arrow, a new picture is shown.

The project has provide the source code of the pictures (pictures.c), you can
modify and re-generate the pictures.bin. For example, the pictures.bin could be
generated using armgcc commands:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
arm-none-eabi-gcc -c pictures.c -DBUILD_PIC_BIN
arm-none-eabi-objcopy -O binary pictures.o pictures.bin
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Running the demo
This demo could run without debug console. The debug console only shows some
instructions and demo status.

At the beginning, the debug console shows:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FLEXIO MCU LCD example:
Touch the arrow to show next picture...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Press the arrow to show the next picture. If touch point is out of arrow, there
is error log from the debug console:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Not touch the arrow...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
