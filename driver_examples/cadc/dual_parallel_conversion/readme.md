# cadc_dual_parallel_conversion

## Overview

The cadc_dual_parallel_conversion example shows how to use the both converter parallel at the same time with CADC driver.

In this example, the CADC is configured as "kCADC_DualConverterWorkAsTriggeredParallel" (default) work mode. User should
indicate 4 channel pairs to provide the differential voltage signal (can be controlled by user) as the inputs for CADC
sample slots. These sample slots would be assembled into the two conversion sequence for each converter. 

In "kCADC_DualConverterWorkAsTriggeredParallel" mode, the converter A converts sample 0 and sample 1 (sample 0-7 are
available) and the converter B converts sample 8 and sample 9 (sample 8-15 are available). As the "simultaneous mode" is
enabled by default, the converter A's control logic can operate both the two converters, to start and stop together.

When running the project, typing any key into debug console would trigger the conversion. And then, the conversion 
result data of the four sample slots (Sample 0, 1, 8, 9) would be displayed in the terminal. 
