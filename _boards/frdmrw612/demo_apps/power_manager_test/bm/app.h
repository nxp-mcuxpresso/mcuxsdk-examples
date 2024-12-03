/*
 * Copyright 2020-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_POWER_NAME                                                                   \
    {                                                                                    \
        "PM0 - Active", "PM1 - Idle", "PM2 - Standby", "PM3 - Sleep", "PM4 - Deep Sleep" \
    }
#define APP_TARGET_POWER_NUM (PM_LP_STATE_COUNT)

#define APP_PM2_CONSTRAINTS                                                                           \
    6U, PM_RESC_SRAM_0K_384K_STANDBY, PM_RESC_SRAM_384K_448K_STANDBY, PM_RESC_SRAM_448K_512K_STANDBY, \
        PM_RESC_SRAM_512K_640K_STANDBY, PM_RESC_SRAM_640K_896K_STANDBY, PM_RESC_SRAM_896K_1216K_STANDBY

#define APP_PM3_CONSTRAINTS                                                                                 \
    6U, PM_RESC_SRAM_0K_384K_RETENTION, PM_RESC_SRAM_384K_448K_RETENTION, PM_RESC_SRAM_448K_512K_RETENTION, \
        PM_RESC_SRAM_512K_640K_RETENTION, PM_RESC_SRAM_640K_896K_RETENTION, PM_RESC_SRAM_896K_1216K_RETENTION

#define APP_PM4_CONSTRAINTS 0U

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
status_t APP_UartControlCallback(pm_event_type_t eventType, uint8_t powerState, void *data);
void APP_InitWakeupSource(void);
void APP_StartRtcTimer(uint64_t timeOutTickes);
void APP_StopRtcTimer(void);
uint32_t APP_GetWakeupTimeout(void);
void APP_RegisterNotify(void);
void APP_SetConstraints(uint8_t powerMode);
void APP_ReleaseConstraints(uint8_t powerMode);
/*${prototype:end}*/

#endif /* _APP_H_ */
