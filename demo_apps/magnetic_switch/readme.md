# magnetic_switch

## Overview
The magnetic_switch example demonstrates communication with the on-board I2C magnetic switch NMH1000.
The value of magnetic field strength is read from the magnetic switch and print to terminal.

Please refer to NMH1000 Magnetic Switch Sensor Datasheet for more information.
https://www.nxp.com/docs/en/data-sheet/NMH1000.pdf

## Running the demo
When the example runs successfully, you can see the similar information from the terminal as below. To get different magnetic field strength values, place magnet near the NMH1000 sensor.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
I2C magnetic switch NMH1000
Relative magnetic field strength: 1 G
Relative magnetic field strength: 0 G
Relative magnetic field strength: 0 G
Relative magnetic field strength: 1 G
Relative magnetic field strength: 1 G
Relative magnetic field strength: 34 G
Relative magnetic field strength: 53 G
Relative magnetic field strength: 47 G
Relative magnetic field strength: 8 G
Relative magnetic field strength: 5 G
Relative magnetic field strength: 39 G
...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [FRDM-MCXE247](../../_boards/frdmmcxe247/demo_apps/magnetic_switch/example_board_readme.md)
