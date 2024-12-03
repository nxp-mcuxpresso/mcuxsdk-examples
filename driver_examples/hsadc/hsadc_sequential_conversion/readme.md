# hsadc_sequential_conversion

## Overview

The hsadc_sequential_conversion example shows how to use a long conversion sequence (can include as many as all the 16 sample slots) with HSADC driver.

In this example, the HSADC is configured as "kHSADC_DualConverterWorkAsTriggeredSequential" work mode. User should indicate 4 channel pairs to provide the differential voltage signal or 4 channels to provide the single ended voltage signal as the inputs for HSADC sample slots. If user select channel 6 and 7 of converter A or converter B as differential analog input pair for specific sample slot, the sub mux channel number of channel 6 and channel 7 should be configured to be same. If user select channel 6 or 7 of converter A or converter B as single ended analog input for specific sample slot, the sub mux channel number of channel 6 and channel 7 can be different. The sample slots would be assembled into a long conversion sequence.

In "kHSADC_DualConverterWorkAsTriggeredSequential" mode, all the sample slots are organized as a long conversion sequence. The converter A's control logic would be used to operate this long conversion sequence. It can start and stop the sequence.

When running the project, typing any key into debug console would trigger the conversion. And then, the conversion result data of the four sample slots (Sample 0, 1, 2, 3) would be displayed in the terminal. 

## Running the demo
When the example runs successfully, following information can be seen on the OpenSDA terminal:
Press any key to trigger the conversion ...
Sample 0    Sample 1    Sample 8    Sample 9
Result 0    Result 1    Result 2    Result 3
