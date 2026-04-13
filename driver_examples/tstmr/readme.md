# tstmr

## Overview

The tstmr example shows the usage of TSTMR driver in application. The TSTMR module is a free running incrementing counter that starts running after system reset de-assertion and can be read at any time by the software for determining the software ticks.The TSTMR runs off the 1 MHz clock and resets on every system reset.

In this example, it would output a time stamp information when the application is ready. And then, delay for 1 second with TSTMR_DelayUs() function. Before and after the delay, it would output the two time stamps information again.

## Supported Boards
- [EVK9-MIMX8ULP](../../_boards/evk9mimx8ulp/driver_examples/tstmr/example_board_readme.md)
- [IMX952LPD5EVK-19](../../_boards/imx952evk/driver_examples/tstmr/example_board_readme.md)
- [IMX952LPD5EVK-15](../../_boards/imx952evk/driver_examples/tstmr/example_board_readme.md)
- [EVK-MCIMX7ULP](../../_boards/evkmcimx7ulp/driver_examples/tstmr/example_board_readme.md)
- [EVK-MIMX8ULP](../../_boards/evkmimx8ulp/driver_examples/tstmr/example_board_readme.md)
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/driver_examples/tstmr/example_board_readme.md)
- [FRDM-K32L2A4S](../../_boards/frdmk32l2a4s/driver_examples/tstmr/example_board_readme.md)
- [FRDM-K32L3A6](../../_boards/frdmk32l3a6/driver_examples/tstmr/example_board_readme.md)
- [FRDM-MCXW71](../../_boards/frdmmcxw71/driver_examples/tstmr/example_board_readme.md)
- [MCX-W71-EVK](../../_boards/mcxw71evk/driver_examples/tstmr/example_board_readme.md)
- [IMX95LP4XEVK-15](../../_boards/imx95lp4xevk15/driver_examples/tstmr/example_board_readme.md)
- [IMX95LPD5EVK-19](../../_boards/imx95lpd5evk19/driver_examples/tstmr/example_board_readme.md)
- [FRDM-IMX95](../../_boards/frdmimx95/driver_examples/tstmr/example_board_readme.md)
- [KW45B41Z-EVK](../../_boards/kw45b41zevk/driver_examples/tstmr/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../_boards/mcimx93autoevk/driver_examples/tstmr/example_board_readme.md)
- [MCIMX93-EVK](../../_boards/mcimx93evk/driver_examples/tstmr/example_board_readme.md)
- [MCIMX93W-EVK](../../_boards/mcimx93wevk/driver_examples/tstmr/example_board_readme.md)
- [MCIMX93-QSB](../../_boards/mcimx93qsb/driver_examples/tstmr/example_board_readme.md)
- [IMX943-EVK](../../_boards/imx943evk/driver_examples/tstmr/example_board_readme.md)
- [FRDM-IMXRT1186](../../_boards/frdmimxrt1186/driver_examples/tstmr/example_board_readme.md)
- [FRDM-KW43](../../_boards/frdmkw43/driver_examples/tstmr/example_board_readme.md)
- [FRDM-MCXW70](../../_boards/frdmmcxw70/driver_examples/tstmr/example_board_readme.md)
