# trdc_flw

## Overview
The trdc flw example shows how to use the trdc flash logical window funtion.

In this example, flash logical window is configured and enabled. First memory
outside the window is touched and the hardfault occurs, then enlarge the window
size to make the access legal. Next the data mapping between the physical address
and programmable flash address is checked.

## Supported Boards
- [FRDM-MCXW71](../../../_boards/frdmmcxw71/driver_examples/trdc/flw/example_board_readme.md)
- [K32W148-EVK](../../../_boards/k32w148evk/driver_examples/trdc/flw/example_board_readme.md)
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/driver_examples/trdc/flw/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/driver_examples/trdc/flw/example_board_readme.md)
- [MCX-W72-EVK](../../../_boards/mcxw72evk/driver_examples/trdc/flw/example_board_readme.md)
