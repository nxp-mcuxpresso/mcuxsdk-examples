# romapi_runbootloader

## Hardware requirements
- Mini/micro USB cable
- Personal Computer

## Board settings

## Prepare the Demo
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J1) on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

## Running the demo

When the example runs successfully, the following message is displayed in the terminal:

```
Calling the runBootloader API to force into the ISP mode: x...
The runBootloader ISP interface is choosen from the following one:
kIspPeripheral_Auto :     0
kIspPeripheral_UsbHid :   1
kIspPeripheral_Uart :     2
kIspPeripheral_SpiSlave : 3
kIspPeripheral_I2cSlave : 4
kIspPeripheral_Can :      5
Call the runBootloader API based on the arg : xxxxxxxx...
```

## Supported Boards
- [FRDM-MCXA153](../../../_boards/frdmmcxa153/driver_examples/romapi/runbootloader/example_board_readme.md)
- [FRDM-MCXA156](../../../_boards/frdmmcxa156/driver_examples/romapi/runbootloader/example_board_readme.md)
- [FRDM-MCXA166](../../../_boards/frdmmcxa166/driver_examples/romapi/runbootloader/example_board_readme.md)
- [FRDM-MCXA276](../../../_boards/frdmmcxa276/driver_examples/romapi/runbootloader/example_board_readme.md)
