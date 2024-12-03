# cadc_parallel_simultaneous_conversion

## Overview

The cadc_dual_parallel_conversion example shows how to use the both converter parallel at the same time with CADC driver.

In this example, the CADC is configured as "kCADC_ScanModeTriggeredParallelSimultaneous" (default) work mode. User should
indicate 4 channel pairs to provide the differential voltage signal (can be controlled by user) as the inputs for CADC
sample slots. These sample slots would be assembled into the two conversion sequence for each converter. 

In "kCADC_ScanModeTriggeredParallelSimultaneous" mode, the converter A converts sample 0 and sample 1 (sample 0-7 are
available) and the converter B converts sample 8 and sample 9 (sample 8-15 are available). As the "simultaneous mode" is
enabled by default, the converter A's control logic can operate both the two converters, to start and stop together.

When running the project, typing any key into debug console would trigger the conversion. And then, the conversion 
result data of the four sample slots (Sample 0, 1, 8, 9) would be displayed in the terminal.

## Supported Boards
- [MC56F80000-EVK](../../../_boards/mc56f80000evk/driver_examples/cadc/parallel_simultaneous_conversion/example_board_readme.md)
- [MC56F81000-EVK](../../../_boards/mc56f81000evk/driver_examples/cadc/parallel_simultaneous_conversion/example_board_readme.md)
- [MC56F83000-EVK](../../../_boards/mc56f83000evk/driver_examples/cadc/parallel_simultaneous_conversion/example_board_readme.md)
- [TWR-MC56F8200](../../../_boards/twrmc56f8200/driver_examples/cadc/parallel_simultaneous_conversion/example_board_readme.md)
- [TWR-MC56F8400](../../../_boards/twrmc56f8400/driver_examples/cadc/parallel_simultaneous_conversion/example_board_readme.md)
