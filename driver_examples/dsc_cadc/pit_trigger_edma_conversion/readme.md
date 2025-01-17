# cadc_pit_trigger_edma_conversion

## Overview
The PIT TRIGGER EDMA conversion demo application demonstrates how to use PIT to trigger CADC conversion and configure xbara 
to connect PIT output with CADC input. CADC performs data conversion after each count period of PIT. The EDMA request will be 
set during any CADC conversion ready event recorded when the ADC_STAT[EOSI0] flag is set. EDMA will transfer CADC results into memory.
Enter any key via terminal will start PIT to trigger CADC periodly, and then the average value(16 times) of thermistor will be printed on terminal.

## Supported Boards
- [MC56F80000-EVK](../../../_boards/mc56f80000evk/driver_examples/cadc/pit_trigger_edma_conversion/example_board_readme.md)
- [MC56F81000-EVK](../../../_boards/mc56f81000evk/driver_examples/cadc/pit_trigger_edma_conversion/example_board_readme.md)
- [MC56F83000-EVK](../../../_boards/mc56f83000evk/driver_examples/cadc/pit_trigger_edma_conversion/example_board_readme.md)
