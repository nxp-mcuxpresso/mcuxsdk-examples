# nbu_bb_bm

## Overview
This is the nbu_bb_bm example, as a transparent transmission tool, to convenient to communicate using the standard HCI command.
Following is an example with RD-RW61X-BGA board.

## Prepare the Demo
1.  Connect a micro USB cable between the PC host and the MCU-Link USB port (J7) on the board.
2.  Connect a micro USB cable between the PC host and the USB-UART port (J21) on the board.
3.  The project set the 115200 baud rate as default, set flow as default. Use the USB-UART port (J21) to communicate.
    if need use 3000000 baud rate, need modify the code.
    open the uart debugging tool, configure
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - flow control(tx and rx)
4.  Download the program to the target board.


## Running the demo
1. Send the standard hci command from PC to board using the USB-UART port (J21).
2. For example, enable adv hci command.
  send command  01 0A 20 01 01
  response data 04 0E 04 01 0A 20 00

## Supported Boards
- [FRDM-RW612](../../_boards/frdmrw612/bt_ble_examples/nbu_bb_bm/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/bt_ble_examples/nbu_bb_bm/example_board_readme.md)
