/*
 * Copyright 2024 NXP
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
#define APP_POWER_NAME                                                     \
    {                                                                      \
        "Sleep", "Deep Sleep", "Power Down", "Deep Power Down" \
    }
#define APP_TARGET_POWER_NUM (4U)

#define APP_SLEEP_CONSTRAINTS                                                                                   \
    19U, PM_RESC_RAMA0_8K_ACTIVE, PM_RESC_RAMA1_8K_ACTIVE, PM_RESC_RAMA2_8K_ACTIVE, PM_RESC_RAMA3_8K_ACTIVE,    \
        PM_RESC_RAMX0_32K_ACTIVE, PM_RESC_RAMX1_32K_ACTIVE, PM_RESC_RAMX2_32K_ACTIVE, PM_RESC_RAMB0_32K_ACTIVE, \
        PM_RESC_RAMC0_32K_ACTIVE, PM_RESC_RAMC1_32K_ACTIVE, PM_RESC_RAMD0_32K_ACTIVE, PM_RESC_RAMD1_32K_ACTIVE, \
        PM_RESC_RAME0_32K_ACTIVE, PM_RESC_RAME1_32K_ACTIVE, PM_RESC_RAMF0_32K_ACTIVE, PM_RESC_RAMF1_32K_ACTIVE, \
        PM_RESC_RAMG01_32K_ACTIVE, PM_RESC_RAMG23_32K_ACTIVE, PM_RESC_RAMH01_32K_ACTIVE

#define APP_DEEP_SLEEP_CONSTRAINTS                                                            \
    19U, PM_RESC_RAMA0_8K_RETENTION, PM_RESC_RAMA1_8K_RETENTION, PM_RESC_RAMA2_8K_RETENTION,  \
        PM_RESC_RAMA3_8K_RETENTION, PM_RESC_RAMX0_32K_RETAINED, PM_RESC_RAMX1_32K_RETAINED,   \
        PM_RESC_RAMX2_32K_RETAINED, PM_RESC_RAMB0_32K_RETAINED, PM_RESC_RAMC0_32K_RETAINED,   \
        PM_RESC_RAMC1_32K_RETAINED, PM_RESC_RAMD0_32K_RETAINED, PM_RESC_RAMD1_32K_RETAINED,   \
        PM_RESC_RAME0_32K_RETAINED, PM_RESC_RAME1_32K_RETAINED, PM_RESC_RAMF0_32K_RETAINED,   \
        PM_RESC_RAMF1_32K_RETAINED, PM_RESC_RAMG01_32K_RETAINED, PM_RESC_RAMG23_32K_RETAINED, \
        PM_RESC_RAMH01_32K_RETAINED

#define APP_POWER_DOWN_CONSTRAINTS                                                            \
    19U, PM_RESC_RAMA0_8K_RETENTION, PM_RESC_RAMA1_8K_RETENTION, PM_RESC_RAMA2_8K_RETENTION,  \
        PM_RESC_RAMA3_8K_RETENTION, PM_RESC_RAMX0_32K_RETAINED, PM_RESC_RAMX1_32K_RETAINED,   \
        PM_RESC_RAMX2_32K_RETAINED, PM_RESC_RAMB0_32K_RETAINED, PM_RESC_RAMC0_32K_RETAINED,   \
        PM_RESC_RAMC1_32K_RETAINED, PM_RESC_RAMD0_32K_RETAINED, PM_RESC_RAMD1_32K_RETAINED,   \
        PM_RESC_RAME0_32K_RETAINED, PM_RESC_RAME1_32K_RETAINED, PM_RESC_RAMF0_32K_RETAINED,   \
        PM_RESC_RAMF1_32K_RETAINED, PM_RESC_RAMG01_32K_RETAINED, PM_RESC_RAMG23_32K_RETAINED, \
        PM_RESC_RAMH01_32K_RETAINED

#define APP_DEEP_POWER_DOWN_CONSTRAINTS \
    4U, PM_RESC_RAMA0_8K_RETENTION, PM_RESC_RAMA1_8K_RETENTION, PM_RESC_RAMA2_8K_RETENTION, PM_RESC_RAMA3_8K_RETENTION

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_InitWakeupSource(void);
void APP_Lptmr0WakeupService(void);
void APP_StartLptmr(uint64_t timeOutTickes);
void APP_StopLptmr(void);
uint32_t APP_GetWakeupTimeout(void);
void APP_RegisterNotify(void);
void APP_SetConstraints(uint8_t powerMode);
void APP_ReleaseConstraints(uint8_t powerMode);
/*${prototype:end}*/

#endif /* _APP_H_ */
