# cadc_dual_seperate_conversion

## Overview

The cadc_dual_seperate_conversion example shows how to use each converter separately with CADC driver.

In this example, the CADC is configured as "kCADC_DualConverterWorkAsTriggeredParallel" work mode. User should indicate
4 channel pairs to provide the differential voltage signal (can be controlled by user) as the inputs for CADC sample 
slots. These sample slots would be assembled into the two short conversion sequences (can include 8 sample slots as 
most). Also the "simultaneous mode" is disabled, so that each converter can be operated by their own control logic 
separately.

When running the project, typing any key into debug console would trigger the conversion. And then, the conversion 
result data of the converter A's sample slots (Sample 0, 1) would be displayed in the terminal. Type the keyboard again
to trigger the converter B, the conversion result data of the converter B's two sample slots (Sample 8, 9) would be 
displayed.
