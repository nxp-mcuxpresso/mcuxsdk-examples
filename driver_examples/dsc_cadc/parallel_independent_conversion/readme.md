# cadc_parallel_independent_conversion

## Overview

The cadc_parallel_independent_conversion example shows how to use each converter independently.

In this example, the CADC is configured as "kCADC_ScanModeTriggeredParallelIndependent" work mode. User should indicate
4 channel pairs to provide the differential voltage signal (can be controlled by user) as the inputs for CADC sample 
slots. These sample slots would be assembled into the two short conversion sequences (can include 8 sample slots as 
most). Also the "simultaneous mode" is disabled, so that each converter can be operated by their own control logic 
separately.

When running the project, typing any key into debug console would trigger the conversion. And then, the conversion 
result data of the converter A's sample slots (Sample 0, 1) would be displayed in the terminal. Type the keyboard again
to trigger the converter B, the conversion result data of the converter B's two sample slots (Sample 8, 9) would be 
displayed.

## Supported Boards
- [MC56F80000-EVK](../../../_boards/mc56f80000evk/driver_examples/cadc/parallel_independent_conversion/example_board_readme.md)
- [MC56F81000-EVK](../../../_boards/mc56f81000evk/driver_examples/cadc/parallel_independent_conversion/example_board_readme.md)
- [MC56F83000-EVK](../../../_boards/mc56f83000evk/driver_examples/cadc/parallel_independent_conversion/example_board_readme.md)
- [TWR-MC56F8200](../../../_boards/twrmc56f8200/driver_examples/cadc/parallel_independent_conversion/example_board_readme.md)
- [TWR-MC56F8400](../../../_boards/twrmc56f8400/driver_examples/cadc/parallel_independent_conversion/example_board_readme.md)
