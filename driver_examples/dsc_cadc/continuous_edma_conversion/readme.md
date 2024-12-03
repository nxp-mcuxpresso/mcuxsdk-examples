# cadc_continuous_edma_conversion

## Overview
The CADC continuous EDMA demo application demonstrates the usage of the ADC and EDMA peripheral while in a loop Sequential mode. The
CADC is first set to loop Sequential mode. In loop Sequential convert configuration, only the initial rising-edge to launch continuous conversions is
observed, and until conversion is aborted, the CADC continues to do conversions on the same CLIST register that initiated the conversion. 
EDMA request will be asserted during an CADC conversion READY event noted when any of the ADC_RDY[RDYn] flags is asserted. EDMA will transfer
CADC results to memory and if users press any key, demo will average CADC results stored in memory and print average on the terminal.

## Supported Boards
- [MC56F80000-EVK](../../../_boards/mc56f80000evk/driver_examples/cadc/continuous_edma_conversion/example_board_readme.md)
- [MC56F81000-EVK](../../../_boards/mc56f81000evk/driver_examples/cadc/continuous_edma_conversion/example_board_readme.md)
- [MC56F83000-EVK](../../../_boards/mc56f83000evk/driver_examples/cadc/continuous_edma_conversion/example_board_readme.md)
