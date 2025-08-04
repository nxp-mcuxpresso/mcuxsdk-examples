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
#define APP_POWER_NAME                                              \
    {                                                               \
        "Sleep", "Deep Sleep", "Power Down", "Deep Power Down"      \
    }

#define APP_TARGET_POWER_NUM (4U)

#define APP_SLEEP_CONSTRAINTS                                                                    \
    12U, PM_RESC_FRO_16K_ACTIVE, PM_RESC_RETENTION_LDO_ACTIVE, PM_RESC_CORE_VDD_LVD_ACT_ACTIVE,  \
         PM_RESC_CORE_VDD_LVD_LP_ACTIVE, PM_RESC_SYS_VDD_LVD_ACTIVE, PM_RESC_SYS_VDD_HVD_ACTIVE, \
         PM_RESC_LPCAC_ACTIVE, PM_RESC_FLASH_ARRAY_ACTIVE, PM_RESC_SRAM_BLOCK0_ACTIVE,           \
         PM_RESC_SRAM_BLOCK1_ACTIVE, PM_RESC_SRAM_BLOCK2_ACTIVE, PM_RESC_SRAM_BLOCK3_ACTIVE
      
#define APP_DEEP_SLEEP_CONSTRAINTS                                                               \
    12U, PM_RESC_FRO_16K_ACTIVE, PM_RESC_RETENTION_LDO_ACTIVE, PM_RESC_CORE_VDD_LVD_ACT_ACTIVE,  \
         PM_RESC_CORE_VDD_LVD_LP_ACTIVE, PM_RESC_SYS_VDD_LVD_ACTIVE, PM_RESC_SYS_VDD_HVD_ACTIVE, \
         PM_RESC_LPCAC_POWEROFF, PM_RESC_FLASH_ARRAY_STATIC, PM_RESC_SRAM_BLOCK0_STATIC,         \
         PM_RESC_SRAM_BLOCK1_STATIC, PM_RESC_SRAM_BLOCK2_STATIC, PM_RESC_SRAM_BLOCK3_STATIC
                                               
#define APP_POWER_DOWN_CONSTRAINTS                                                                     \
    12U, PM_RESC_FRO_16K_ACTIVE, PM_RESC_RETENTION_LDO_POWEROFF, PM_RESC_CORE_VDD_LVD_ACT_POWEROFF,    \
         PM_RESC_CORE_VDD_LVD_LP_POWEROFF, PM_RESC_SYS_VDD_LVD_POWEROFF, PM_RESC_SYS_VDD_HVD_POWEROFF, \
         PM_RESC_LPCAC_POWEROFF, PM_RESC_FLASH_ARRAY_POWEROFF, PM_RESC_SRAM_BLOCK0_STATIC,             \
         PM_RESC_SRAM_BLOCK1_STATIC, PM_RESC_SRAM_BLOCK2_STATIC, PM_RESC_SRAM_BLOCK3_STATIC

#define APP_DEEP_POWER_DOWN_CONSTRAINTS                                                                \
    12U, PM_RESC_FRO_16K_ACTIVE, PM_RESC_RETENTION_LDO_ACTIVE, PM_RESC_CORE_VDD_LVD_ACT_POWEROFF,      \
         PM_RESC_CORE_VDD_LVD_LP_POWEROFF, PM_RESC_SYS_VDD_LVD_POWEROFF, PM_RESC_SYS_VDD_HVD_POWEROFF, \
         PM_RESC_LPCAC_POWEROFF, PM_RESC_FLASH_ARRAY_POWEROFF, PM_RESC_SRAM_BLOCK0_POWEROFF,           \
         PM_RESC_SRAM_BLOCK1_STATIC, PM_RESC_SRAM_BLOCK2_POWEROFF, PM_RESC_SRAM_BLOCK3_POWEROFF
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
