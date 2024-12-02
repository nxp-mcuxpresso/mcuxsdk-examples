# enet_txrx_ptp1588_xfer

## Overview

The enet_rxtx_ptp1588 example shows the way to use ENET driver to
 receive and transmit frame in the 1588 feature required cases.

1. This example shows how to initialize the ENET MAC.
2. How to use ENET MAC to receive and transmit frame.
3. How to add to the multicast group to receive PTP 1588 message.
4. How to get the time stamp of the PTP 1588 timer.
4. How to use Get the ENET transmit and receive frame time stamp.

The example transmits 20 number PTP event frame, shows the timestamp of the transmitted frame.
The length, source MAC address and destination MAC address of the received frame will be print.
The time stamp of the received timestamp will be print when the PTP message frame is received.

Note, The RMII mode is used for default setting to initialize the ENET interface between MAC and the external PHY. you
can change it to MII mode as you wish. Please make sure the MII Mode setting in the MAC is synchronize to the setting
in TWR-SERIAL board for the external PHY.

## Supported Boards
- [FRDM-RW612](../../../_boards/frdmrw612/driver_examples/enet/txrx_ptp1588_transfer/example_board_readme.md)
- [RD-RW612-BGA](../../../_boards/rdrw612bga/driver_examples/enet/txrx_ptp1588_transfer/example_board_readme.md)
