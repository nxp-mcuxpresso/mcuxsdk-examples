#### 25.09.00
- [wireless_mcu] Register NBU event callback which will be called upon NBU event indication.

#### 25.06.00-pvw2
- [wireless_mcu][board_lp] Trig temperature measurement on each wakeup - call `SENSORS_TriggerTemperatureMeasurementUnsafe()`
- [wireless_mcu][hardware_init.c] Implement a way to calibrate XTAL32M with temperature - call `PLATFORM_RegisterXtal32MTempCompLut()` when `gBoardUseXtal32MTempComp` is set
- [wireless_mcu][board_lp] Rework and remove unnecessary ADC APIs - Call `PLATFORM_ReinitAdc()` rather `PLATFORM_RestoreAdcContext()` on wakeup from low power
- [wireless_mcu][board_lp] Wait for prints to be ready after exit from low power

#### 25.06.00-pvw1
- [wireless_mcu][board_lp] Add new flags `gBoardDcdcRampTrim_c` and `gBoardDcdcEnableHighPowerModeOnNbu_d`  to smooth peak of current during DCDC voltage increase
