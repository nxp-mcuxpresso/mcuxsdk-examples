# flexcan_remote_request_non_blocking

## Overview
The flexcan_remote_request_non_blocking example shows how to use remote request feature in non-blocking mode by SDK driver.

In this example, 2 boards are connected through CAN bus.

Board A send a remote frame to request message from board B.  
After successful request, board A will receive response message.  
Board A uses interrupt-driven non-blocking transfer.

Board B receive remote frame and send response message manually with application control.  
After successful response, board B will update response message and wait for next remote request.  
Board B uses interrupt-driven non-blocking transfer.

Both boards use differnet message buffer to send and receive messages.  
Remote request frames are stored in RX message buffer instead of automatic response.


## Supported Boards
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/driver_examples/flexcan/remote_request_non_blocking/example_board_readme.md)
