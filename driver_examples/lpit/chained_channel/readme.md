# lpit_chained_channel

## Overview
The LPIT chained_channel project is a simple example of the SDK LPIT driver. It sets up the LPIT 
hardware block to trigger a periodic interrupt after every 1 second in the channel No.0, the channel 
No.1 chained with channel No.0, if LPIT contain more than two channels, the channel No.2 chained with 
channel No.1....the channel No.N chained with Channel No.N-1. 
Chaining the timer channel causes them to work in a 'nested loop' manner thereby leading to an effective
timeout value up to TVALn × ((TVALn-1) + 1) at channel No.N. The timer counts down for 5 (TVALn + 1) timer
cycles until the timer reaches 0, then the timer generates an interrupt and loads the Timer Value register
(TVALn) value again. The example sets the timer period of the channel to 5 by using the API LPIT_SetTimerPeriod().
When the LPIT interrupt is triggered a message a printed on the serial terminal.

## Supported Boards
- [FRDM-K32L2A4S](../../../_boards/frdmk32l2a4s/driver_examples/lpit/chained_channel/example_board_readme.md)
- [FRDM-K32L3A6](../../../_boards/frdmk32l3a6/driver_examples/lpit/chained_channel/example_board_readme.md)
- [FRDM-KE15Z](../../../_boards/frdmke15z/driver_examples/lpit/chained_channel/example_board_readme.md)
- [FRDM-KE16Z](../../../_boards/frdmke16z/driver_examples/lpit/chained_channel/example_board_readme.md)
- [FRDM-KE17Z](../../../_boards/frdmke17z/driver_examples/lpit/chained_channel/example_board_readme.md)
- [FRDM-KE17Z512](../../../_boards/frdmke17z512/driver_examples/lpit/chained_channel/example_board_readme.md)
- [FRDM-MCXW71](../../../_boards/frdmmcxw71/driver_examples/lpit/chained_channel/example_board_readme.md)
- [IMX95LP4XEVK-15](../../../_boards/imx95lp4xevk15/driver_examples/lpit/chained_channel/example_board_readme.md)
- [IMX95LPD5EVK-19](../../../_boards/imx95lpd5evk19/driver_examples/lpit/chained_channel/example_board_readme.md)
- [imx95verdinevk](../../../_boards/imx95verdinevk/driver_examples/lpit/chained_channel/example_board_readme.md)
- [K32W148-EVK](../../../_boards/k32w148evk/driver_examples/lpit/chained_channel/example_board_readme.md)
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/driver_examples/lpit/chained_channel/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/driver_examples/lpit/chained_channel/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../../_boards/mcimx93autoevk/driver_examples/lpit/chained_channel/example_board_readme.md)
- [MCIMX93-EVK](../../../_boards/mcimx93evk/driver_examples/lpit/chained_channel/example_board_readme.md)
- [MCIMX93-QSB](../../../_boards/mcimx93qsb/driver_examples/lpit/chained_channel/example_board_readme.md)
- [MCX-W72-EVK](../../../_boards/mcxw72evk/driver_examples/lpit/chained_channel/example_board_readme.md)
