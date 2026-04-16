# adc_watchdog

## Overview

The SAR ADC watchdog example shows the simplest way to use the SAR ADC driver.

In this example, the user-specified channel is used to execute in the normal conversion
scan mode. The end-of-conversion, pre-sample, and DMA transfer features of each channel
in the conversion chain are disabled. The analog watchdog function is enabled to monitor
the conversion result, and the low and high threshold interrupt is enabled, The user
should provide a voltage signal to each conversion channel. Running the project, after
the channel conversion is completed, the conversion data will be compared with the low
and high threshold values. If the conversion data is lower than the low threshold value,
the low threshold interrupt will be asserted, if the conversion data is higher than the
high threshold value, the high threshold interrupt will be asserted, and if the conversion
data is valid, the conversion data will be printed on the debug console.

## Supported Boards

- [IMX952LPD5EVK-19](../../../_boards/imx952evk/driver_examples/sar_adc/watchdog/example_board_readme.md)
- [IMX952LPD5EVK-15](../../../_boards/imx952evk/driver_examples/sar_adc/watchdog/example_board_readme.md)
- [IMX95LP4XEVK-15](../../../_boards/imx95lp4xevk15/driver_examples/sar_adc/watchdog/example_board_readme.md)
- [IMX95LPD5EVK-19](../../../_boards/imx95lpd5evk19/driver_examples/sar_adc/watchdog/example_board_readme.md)
- [FRDM-IMX95](../../../_boards/frdmimx95/driver_examples/sar_adc/watchdog/example_board_readme.md)
- [FRDM-IMX952](../../../_boards/frdmimx952/driver_examples/sar_adc/watchdog/example_board_readme.md)
- [FRDM-IMX937](../../../_boards/frdmimx937/driver_examples/sar_adc/watchdog/example_board_readme.md)
- [MCIMX93-EVK](../../../_boards/mcimx93evk/driver_examples/sar_adc/watchdog/example_board_readme.md)
- [MCIMX93W-EVK](../../../_boards/mcimx93wevk/driver_examples/sar_adc/watchdog/example_board_readme.md)
- [IMX943-EVK](../../../_boards/imx943evk/driver_examples/sar_adc/watchdog/example_board_readme.md)
- [FRDM-MCXE31B](../../../_boards/frdmmcxe31b/driver_examples/sar_adc/watchdog/example_board_readme.md)
