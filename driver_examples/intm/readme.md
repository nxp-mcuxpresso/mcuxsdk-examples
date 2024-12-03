# intm

## Overview

The INTM project is a simple demonstration of the SDK INTM driver.
The role of INTM is to monitor the interrupt response. The main monitoring is whether the interrupt has timed out from 
the request to the response. Up to 4 interrupts can be monitored simultaneously.
This example is to detect the key interrupt, verify the function of INTM by delaying the time from the trigger to 
the response of the key interrupt, and confirm the interrupt timeout by the flag bit one.

## Supported Boards
- [FRDM-MCXN236](../../_boards/frdmmcxn236/driver_examples/intm/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/driver_examples/intm/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/driver_examples/intm/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/driver_examples/intm/example_board_readme.md)
