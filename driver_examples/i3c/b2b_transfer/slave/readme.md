# i3c_b2b_transfer_slave

## Overview
The example is the slave side of the I3C message mode board to board transfer example. The master
board demonstrates the I3C DMA message mode driver (hardware-framed transfers serviced by DMA) in
both SDR and HDR-DDR mode; the slave IP has no message-mode registers, so this side uses the
standard i3c slave driver serviced by DMA.

In this example, one i3c instance is the slave and another i3c instance on the other board is the
master. Master first assigns the connected slave a dynamic address, then transfers data in message
mode. For each exchange the slave receives the written buffer, notifies the controller with an IBI
carrying the data length, and presents the same data for the controller's read-back. The exchange
runs twice: once for SDR and once for HDR-DDR. In HDR-DDR the controller's command byte arrives
inline as the first received byte, so the slave skips that framing byte and echoes only the data.

## Running the demo
The following message shows in the terminal if the example runs successfully.

I3C bus transfer example -- Slave.

Check I3C master SDR transfer.

Slave received 30 bytes :
0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7
0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17
0x18  0x19  0x1a  0x1b  0x1c  0x1d

I3C master SDR transfer finished.

Check I3C master HDR-DDR transfer.

Slave received 30 bytes :
0x 0  0x 1  0x 2  0x 3  0x 4  0x 5  0x 6  0x 7
0x 8  0x 9  0x a  0x b  0x c  0x d  0x e  0x f
0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17
0x18  0x19  0x1a  0x1b  0x1c  0x1d

I3C master HDR-DDR transfer finished.

## Supported Boards
- [FRDM-MCXN236](../../../../_boards/frdmmcxn236/driver_examples/i3c/b2b_transfer/slave/example_board_readme.md)
- [FRDM-MCXN947](../../../../_boards/frdmmcxn947/driver_examples/i3c/b2b_transfer/slave/example_board_readme.md)
