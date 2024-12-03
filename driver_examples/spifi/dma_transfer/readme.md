# spifi_dma_transfer

## Overview
The SPIFI DMA Example project is a demonstration program that uses the KSDK software to program external serial
flash using DMA and read through AHB bus.

## Prepare the Demo
Connect a serial cable from the debug UART port of the board to the PC. Start Tera Term
(http://ttssh2.osdn.jp) and make a connection to the virtual serial port.

1. Start Tera Term
2. New connection -> Serial
3. Set apropriate COMx port (x is port number) in Port context menu. Number is provided by operation
   system and could be different from computer to computer. Select COM number related to virtual
   serial port. Confirm selected port by OK button.
4. Set following connection parameters in menu Setup->Serial port...
        Baud rate:    115200
        Data:         8
        Parity:       none
        Stop:         1
        Flow control: one
5.  Confirm selected parameters by OK button.

## Running the demo
SPIFI flash dma example started
All data written is correct!
SPIFI DMA example Finished!

## Supported Boards
- [LPCXpresso54628](../../../_boards/lpcxpresso54628/driver_examples/spifi/dma_transfer/example_board_readme.md)
- [LPCXpresso54S018](../../../_boards/lpcxpresso54s018/driver_examples/spifi/dma_transfer/example_board_readme.md)
- [LPCXpresso54S018M](../../../_boards/lpcxpresso54s018m/driver_examples/spifi/dma_transfer/example_board_readme.md)
