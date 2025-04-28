#### 25.06.00-pvw2
- [board_platform.h] Please note that the feature to calibrate XTAL32M with temperature by adding 'gBoardUseXtal32MTempComp' is currently not ready for implementation. We have observed issues when this feature is enabled
- [refdes] increase Timer manager stack `TM_TASK_STACK_SIZE' from 324 to 350 to avoid overflow on refdes applications
- [board_utility.c] Rework and remove unnecessary ADC APIs