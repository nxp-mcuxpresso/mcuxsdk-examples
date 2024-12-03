# hsadc_dual_seperate_conversion

## Overview

The hsadc_dual_seperate_conversion example shows how to use each converter separately with HSADC driver.

In this example, the HSADC is configured as "kHSADC_DualConverterWorkAsTriggeredParallel" work mode. User should indicate 4 channel pairs to provide the differential voltage signal or 4 channels to provide the single ended voltage signal as the inputs for HSADC sample slots. If user select channel 6 and 7 of converter A or converter B to sample for specific sample slot, the sub mux channel number of channel 6 and channel 7 also should be configured. The sample slots would be assembled into the two short conversion sequences (can include 8 sample slots as most). Also the "simultaneous mode" is disabled, so that each converter can be operated by their own control logic separately.

When running the project, typing any key into debug console would trigger the conversion. And then, the conversion result data of the converter A's sample slots (Sample 0, 1) would be displayed in the terminal. Type the keyboard again to trigger the converter B, the conversion result data of the converter B's two sample slots (Sample 8, 9) would be displayed.

## Running the demo
When the example runs successfully, following information can be seen on the OpenSDA terminal:
Press any key to trigger the conversion ...
Sample 0    Sample 1    Sample 8    Sample 9
Result 0    Result 1    Result 2    Result 3