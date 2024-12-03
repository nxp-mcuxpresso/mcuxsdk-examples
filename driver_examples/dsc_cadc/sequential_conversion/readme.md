# cadc_sequential_conversion

## Overview

The cadc_sequential_conversion example shows how to use a long conversion sequence (can include as many as all the 16 
sample slots) with CADC driver.

In this example, the CADC is configured as "kCADC_ScanModeTriggeredSequential" work mode. User should 
indicate 4 channel pairs to provide the differential voltage signal (can be controlled by user) as the inputs for CADC
sample slots. These sample slots would be assembled into a long conversion sequence.

In "kCADC_ScanModeTriggeredSequential" mode, all the sample slots are organized as a long conversion 
sequence. The converter A's control logic would be used to operate this long conversion sequence. It can start and stop
the sequence.

When running the project, typing any key into debug console would trigger the conversion. And then, the conversion 
result data of the four sample slots (Sample 0, 1, 2, 3) would be displayed in the terminal.

## Supported Boards
- [MC56F80000-EVK](../../../_boards/mc56f80000evk/driver_examples/cadc/sequential_conversion/example_board_readme.md)
- [MC56F81000-EVK](../../../_boards/mc56f81000evk/driver_examples/cadc/sequential_conversion/example_board_readme.md)
- [MC56F83000-EVK](../../../_boards/mc56f83000evk/driver_examples/cadc/sequential_conversion/example_board_readme.md)
- [TWR-MC56F8200](../../../_boards/twrmc56f8200/driver_examples/cadc/sequential_conversion/example_board_readme.md)
- [TWR-MC56F8400](../../../_boards/twrmc56f8400/driver_examples/cadc/sequential_conversion/example_board_readme.md)
