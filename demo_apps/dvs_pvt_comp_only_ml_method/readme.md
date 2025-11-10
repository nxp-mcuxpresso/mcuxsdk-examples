# dvs_pvt_comp_only_ml_method

## Overview
The dvs_pvt_comp_only_ml_method application shows the usage of Dynamic Voltage Scalling using PVT Sensor using ML.   
It is an improvement of the standard dvs_pvt_comp_only demo where only 3 frequency points are supported.
Leveraging the ML, the PVTS can be enabled for every frequency.     
User can use the driver to retrieve the correct PVTS delay depending on the core targeted and the desired frequency.   
The model can predict the PVTS delay for the 4 cores (see struct pvts_core_t) at any frequency.   
Before using the model to predict the PVTS delay, user must ensure that :
- CPU/DSP clock is properly configured,
- PVTS_ReadParametersFromOTP is called in order to retrieve the A and B parameters, used by the ML model. 

## Supported Boards
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/demo_apps/dvs_pvt_comp_only_ml_method/example_board_readme.md)
