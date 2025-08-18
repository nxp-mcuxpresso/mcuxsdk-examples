#### 25.09.00-rfp
- [refdes] Enabled NBU debug module to detect NBU faults and extract debug data.
- [refdes] Enabled debug console. Used to dump NBU context/state on NBU fault detection.

#### 25.06.00-pvw2
- [board_platform.h] Please note that the feature to calibrate XTAL32M with temperature by adding 'gBoardUseXtal32MTempComp' is currently not ready for implementation. We have observed issues when this feature is enabled
- [refdes] increase Timer manager stack `TM_TASK_STACK_SIZE' from 324 to 350 to avoid overflow on refdes applications
- [board_utility.c] Rework and remove unnecessary ADC APIs

#### 25.06.00-pvw1
- Add new flags `gBoardDcdcRampTrim_c` and `gBoardDcdcEnableHighPowerModeOnNbu_d`  to smooth peak of current during DCDC voltage increase

#### 24.12.00-pvw1
- update macro default values : BOARD_32MHZ_XTAL_CDAC_VALUE to 10, BOARD_32MHZ_XTAL_ISEL_VALUE to 11