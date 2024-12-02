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
