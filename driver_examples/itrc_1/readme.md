# itrc_1

## Overview
The ITRC Example project is a demonstration program that uses the MCUX SDK software to set up Intrusion and Tamper Response Controller.
Then tests the expected behaviour.

## Running the demo
When the demo runs successfully, the terminal displays similar information like the following:
~~~~~~~~~~~~~~~~~~

ITRC Peripheral Driver Example

Pass: No Event/Action triggered after Init

Enable ITRC IRQ Action response to SW Event 0

Trigger SW Event 0

ITRC IRQ Reached!
ITRC STATUS0:
Software event 0 occurred!
ITRC triggered ITRC_IRQ output!

Clear ITRC IRQ and SW Event 0 STATUS

Disable ITRC IRQ Action response to SW Event 0

Trigger SW Event 0

Pass: No Action triggered after disabling

End of example

~~~~~~~~~~~~~~~~~~

## Supported Boards
- [FRDM-RW612](../../_boards/frdmrw612/driver_examples/itrc/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/driver_examples/itrc/example_board_readme.md)
