# i3c_b2b_transfer_master

## Overview
The example demonstrates the I3C message mode DMA driver family. Message mode is the I3C
controller's hardware-framed transfer path where one message header register write makes the
hardware emit START (or repeated START), the target address, the direction and the message
length, so the CPU needs no per-byte or per-phase state handling. It supports both SDR and
HDR-DDR framing.

Message mode is serviced by DMA, its efficiency win over the core polling/interrupt transfer path
is eliminating per-halfword CPU servicing, so this example covers the DMA path.

In this example, one i3c instance is the master and another i3c instance on the other board is
the slave. Master first assigns the connected slave a dynamic address, then, through the DMA
message mode API, runs the same write/read exchange twice, once in
SDR mode and once in HDR-DDR mode:

- a message write of 30 bytes to the slave;
- the slave prepares the data and notifies the controller with an IBI carrying the length;
- a message read of the reported length that verifies the slave echo matches the sent data.

## Running the demo
The following message shows in the terminal if the example runs successfully.

I3C bus transfer example -- Master.

I3C master do dynamic address assignment to the I3C slaves on bus.
I3C master dynamic address assignment done.

Master will send data :
0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7
0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17
0x18  0x19  0x1a  0x1b  0x1c  0x1d

Start I3C master transfer in SDR mode.

Controller received target IBI, data size 30.

Receive sent data from slave :
0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7
0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17
0x18  0x19  0x1a  0x1b  0x1c  0x1d

I3C master transfer successful in SDR mode.

Start I3C master transfer in HDR-DDR mode.

Controller received target IBI, data size 30.

Receive sent data from slave :
0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7
0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17
0x18  0x19  0x1a  0x1b  0x1c  0x1d

I3C master transfer successful in HDR-DDR mode.

I3C bus transfer example -- all transfers successful!

## Supported Boards
- [FRDM-MCXN236](../../../../_boards/frdmmcxn236/driver_examples/i3c/b2b_transfer/master/example_board_readme.md)
- [FRDM-MCXN947](../../../../_boards/frdmmcxn947/driver_examples/i3c/b2b_transfer/master/example_board_readme.md)
