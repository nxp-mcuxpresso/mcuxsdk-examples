# mcx_enet_txrx_ptp1588_transfer

## Overview

The enet_rxtx_ptp1588_transfer example shows the way to use ENET driver to  
 receive and transmit frame in the 1588 feature required cases.

1. This example shows how to initialize the ENET.
2. How to get the time stamp of the PTP 1588 timer.
3. How to use Get the ENET transmit and receive frame time stamp.

The example transmits 20 number PTP event frame, shows the timestamp of the transmitted frame.
The length, source MAC address and destination MAC address of the received frame will be print. 
The time stamp of the received timestamp will be print when the PTP message frame is received(the outside loopback cable can be used to see the right rx time-stamping log since we send the ptp message).

## Supported Boards
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/enet/txrx_ptp1588_transfer/example_board_readme.md)
- [MCX-N5XX-EVK](../../../_boards/mcxn5xxevk/driver_examples/enet/txrx_ptp1588_transfer/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/enet/txrx_ptp1588_transfer/example_board_readme.md)
