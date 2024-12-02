# adc_selftest

## Overview
The SAR ADC self-test example shows the simplest way to use the SAR ADC driver.

In this example, three channels (channels 0, 1, and 2) are used to form a conversion chain,
these three channels are executed in the normal conversion scan mode. The end-of-channel 
interrupt, pre-sample, DMA transfer, and watchdog features of each channel in the conversion
chain are disabled. The self-test function is enabled, and the self-test watchdog threshold 
is used to check whether the self-test channel converted data is out of the range specified 
in the threshold, the watchdog timer is an additional check that monitors the sequence of the
self-test algorithm implemented. It also verifies that the algorithm is completed within a safe
period. The user should provide a voltage signal to each conversion channel. When running the
project, When the execution of normal conversion ends, the terminal will print out the valid
conversion data. When the conversion is more than 3 rounds and the current conversion chain is
in the test channel conversion, abort the current conversion chain to make a watchdog sequence
error, and the corresponding error information will be printed to the debug console.

## Supported Boards
- [IMX95LP4XEVK-15](../../../_boards/imx95lp4xevk15/driver_examples/sar_adc/selftest/example_board_readme.md)
- [IMX95LPD5EVK-19](../../../_boards/imx95lpd5evk19/driver_examples/sar_adc/selftest/example_board_readme.md)
- [imx95verdinevk](../../../_boards/imx95verdinevk/driver_examples/sar_adc/selftest/example_board_readme.md)
