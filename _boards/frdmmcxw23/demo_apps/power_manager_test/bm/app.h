/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define PIN_INT_SRC kINPUTMUX_GpioPort0Pin18ToPintsel

#define APP_TARGET_POWER_NUM (5)
#define APP_POWER_NAME          \
  { \
    "Sleep", "Deep Sleep", "Power Down(CPU RET)", "Deep Power Down", "Power Off"\
  }

#define APP_SLEEP_CONSTRAINTS                   \
    4U,  PM_RESC_CPU_CLK_OFF, PM_RESC_FLASH_ON, PM_RESC_RTC_WAKE_ON, PM_RESC_FRO_32K_ON
      
#define APP_DEEP_SLEEP_CONSTRAINTS              \
    4U, PM_RESC_CPU_CLK_OFF, PM_RESC_FLASH_OFF, PM_RESC_RTC_WAKE_ON, PM_RESC_FRO_32K_ON

#define APP_POWER_DOWN_CONSTRAINTS              \
     4U, PM_RESC_CPU_CLK_OFF, PM_RESC_FLASH_OFF, PM_RESC_RTC_WAKE_ON, PM_RESC_FRO_32K_ON

#define APP_DEEP_POWER_DOWN_CONSTRAINTS         \
      2U, PM_RESC_RTC_WAKE_ON, PM_RESC_FRO_32K_ON
        

#define APP_POWER_OFF_CONSTRAINTS  0U

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
status_t APP_UartControlCallback(pm_event_type_t eventType, uint8_t powerState, void *data);
void APP_RegisterNotify(void);
void APP_SetConstraints(uint8_t powerMode);
void APP_ReleaseConstraints(uint8_t powerMode);
uint32_t APP_GetWakeupTimeout(void);
void APP_InitWakeupSource(void);
/*${prototype:end}*/
#endif /* _APP_H_ */
