/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_POWER_NAME                                                                                  \
    {                                                                                                   \
        "Sleep (IDD_SLEEP)", "Deep Sleep (IDD_DSLEEP_LP)", "Power Down (IDD_PDOWN_RET_0V7)",            \
        "Deep Power Down (IDD_DPOWN_32K)"                                                               \
    } 
#define APP_TARGET_POWER_NUM (4U)

#define APP_SLEEP_CONSTRAINTS                                                                     \
    37U, PM_RESC_CODE_CACHE_OFF,      PM_RESC_FRO_180M_OFF,       PM_RESC_FRO_12M_OFF,            \
         PM_RESC_FRO_16K_ON,          PM_RESC_OSC_RTC_OFF,        PM_RESC_OSC_SYC_OFF,            \
         PM_RESC_APLL_OFF,            PM_RESC_SPLL_OFF,           PM_RESC_LDO_CORE_OFF,           \
         PM_RESC_RETENTION_LDO_OFF,   PM_RESC_CORE_VDD_IVS_OFF,   PM_RESC_CORE_VDD_LVD_ACT_ON,    \
         PM_RESC_CORE_VDD_HVD_ACT_ON, PM_RESC_SYS_VDD_LVD_ACT_ON, PM_RESC_SYS_VDD_HVD_ACT_ON,     \
         PM_RESC_IO_VDD_LVD_ACT_ON,   PM_RESC_IO_VDD_HVD_ACT_ON,  PM_RESC_CORE_VDD_LVD_LP_ON,     \
         PM_RESC_CORE_VDD_HVD_LP_ON,  PM_RESC_SYS_VDD_LVD_LP_ON,  PM_RESC_SYS_VDD_HVD_LP_ON,      \
         PM_RESC_IO_VDD_LVD_LP_ON,    PM_RESC_IO_VDD_HVD_LP_ON,   PM_RESC_RAM_A0_ON,              \
         PM_RESC_RAM_A1_ON,           PM_RESC_RAM_A2_ON,          PM_RESC_RAM_A3_ON,              \
         PM_RESC_RAM_X0_ON,           PM_RESC_RAM_X1_ON,          PM_RESC_RAM_X2_ON,              \
         PM_RESC_RAM_B0_ON,           PM_RESC_RAM_C0_ON,          PM_RESC_RAM_C1_ON,              \
         PM_RESC_RAM_D0_ON,           PM_RESC_RAM_D1_ON,          PM_RESC_RAM_E0_ON,              \
         PM_RESC_RAM_E1_ON

#define APP_DEEP_SLEEP_CONSTRAINTS                                                                  \
    37U, PM_RESC_CODE_CACHE_OFF,       PM_RESC_FRO_180M_OFF,        PM_RESC_FRO_12M_OFF,            \
         PM_RESC_FRO_16K_ON,           PM_RESC_OSC_RTC_OFF,         PM_RESC_OSC_SYC_OFF,            \
         PM_RESC_APLL_OFF,             PM_RESC_SPLL_OFF,            PM_RESC_LDO_CORE_OFF,           \
         PM_RESC_RETENTION_LDO_OFF,    PM_RESC_CORE_VDD_IVS_ON,     PM_RESC_CORE_VDD_LVD_ACT_OFF,   \
         PM_RESC_CORE_VDD_HVD_ACT_OFF, PM_RESC_SYS_VDD_LVD_ACT_OFF, PM_RESC_SYS_VDD_HVD_ACT_OFF,    \
         PM_RESC_IO_VDD_LVD_ACT_OFF,   PM_RESC_IO_VDD_HVD_ACT_OFF,  PM_RESC_CORE_VDD_LVD_LP_OFF,    \
         PM_RESC_CORE_VDD_HVD_LP_OFF,  PM_RESC_SYS_VDD_LVD_LP_OFF,  PM_RESC_SYS_VDD_HVD_LP_OFF,     \
         PM_RESC_IO_VDD_LVD_LP_OFF,    PM_RESC_IO_VDD_HVD_LP_OFF,   PM_RESC_RAM_A0_HOLD,            \
         PM_RESC_RAM_A1_HOLD,          PM_RESC_RAM_A2_HOLD,         PM_RESC_RAM_A3_HOLD,            \
         PM_RESC_RAM_X0_HOLD,          PM_RESC_RAM_X1_HOLD,         PM_RESC_RAM_X2_HOLD,            \
         PM_RESC_RAM_B0_HOLD,          PM_RESC_RAM_C0_HOLD,         PM_RESC_RAM_C1_HOLD,            \
         PM_RESC_RAM_D0_HOLD,          PM_RESC_RAM_D1_HOLD,         PM_RESC_RAM_E0_HOLD,            \
         PM_RESC_RAM_E1_HOLD

#define APP_POWER_DOWN_CONSTRAINTS                                                                \
    37U, PM_RESC_CODE_CACHE_OFF,      PM_RESC_FRO_180M_OFF,       PM_RESC_FRO_12M_OFF,            \
         PM_RESC_FRO_16K_ON,          PM_RESC_OSC_RTC_OFF,        PM_RESC_OSC_SYC_OFF,            \
         PM_RESC_APLL_OFF,            PM_RESC_SPLL_OFF,           PM_RESC_LDO_CORE_OFF,           \
         PM_RESC_RETENTION_LDO_OFF,   PM_RESC_CORE_VDD_IVS_OFF,   PM_RESC_CORE_VDD_LVD_ACT_ON,    \
         PM_RESC_CORE_VDD_HVD_ACT_ON, PM_RESC_SYS_VDD_LVD_ACT_ON, PM_RESC_SYS_VDD_HVD_ACT_ON,     \
         PM_RESC_IO_VDD_LVD_ACT_ON,   PM_RESC_IO_VDD_HVD_ACT_ON,  PM_RESC_CORE_VDD_LVD_LP_OFF,    \
         PM_RESC_CORE_VDD_HVD_LP_OFF, PM_RESC_SYS_VDD_LVD_LP_OFF, PM_RESC_SYS_VDD_HVD_LP_OFF,     \
         PM_RESC_IO_VDD_LVD_LP_OFF,   PM_RESC_IO_VDD_HVD_LP_OFF,  PM_RESC_RAM_A0_HOLD,            \
         PM_RESC_RAM_A1_HOLD,         PM_RESC_RAM_A2_HOLD,        PM_RESC_RAM_A3_HOLD,            \
         PM_RESC_RAM_X0_HOLD,         PM_RESC_RAM_X1_HOLD,        PM_RESC_RAM_X2_HOLD,            \
         PM_RESC_RAM_B0_HOLD,         PM_RESC_RAM_C0_HOLD,        PM_RESC_RAM_C1_HOLD,            \
         PM_RESC_RAM_D0_HOLD,         PM_RESC_RAM_D1_HOLD,        PM_RESC_RAM_E0_HOLD,            \
         PM_RESC_RAM_E1_HOLD

#define APP_DEEP_POWER_DOWN_CONSTRAINTS                                                             \
    37U, PM_RESC_CODE_CACHE_OFF,       PM_RESC_FRO_180M_OFF,        PM_RESC_FRO_12M_OFF,            \
         PM_RESC_FRO_16K_ON,           PM_RESC_OSC_RTC_OFF,         PM_RESC_OSC_SYC_OFF,            \
         PM_RESC_APLL_OFF,             PM_RESC_SPLL_OFF,            PM_RESC_LDO_CORE_OFF,           \
         PM_RESC_RETENTION_LDO_ON,     PM_RESC_CORE_VDD_IVS_OFF,    PM_RESC_CORE_VDD_LVD_ACT_OFF,   \
         PM_RESC_CORE_VDD_HVD_ACT_OFF, PM_RESC_SYS_VDD_LVD_ACT_OFF, PM_RESC_SYS_VDD_HVD_ACT_OFF,    \
         PM_RESC_IO_VDD_LVD_ACT_OFF,   PM_RESC_IO_VDD_HVD_ACT_OFF,  PM_RESC_CORE_VDD_LVD_LP_OFF,    \
         PM_RESC_CORE_VDD_HVD_LP_OFF,  PM_RESC_SYS_VDD_LVD_LP_OFF,  PM_RESC_SYS_VDD_HVD_LP_OFF,     \
         PM_RESC_IO_VDD_LVD_LP_OFF,    PM_RESC_IO_VDD_HVD_LP_OFF,   PM_RESC_RAM_A0_HOLD,            \
         PM_RESC_RAM_A1_HOLD,          PM_RESC_RAM_A2_HOLD,         PM_RESC_RAM_A3_HOLD,            \
         PM_RESC_RAM_X0_OFF,           PM_RESC_RAM_X1_OFF,          PM_RESC_RAM_X2_OFF,             \
         PM_RESC_RAM_B0_OFF,           PM_RESC_RAM_C0_OFF,          PM_RESC_RAM_C1_OFF,             \
         PM_RESC_RAM_D0_OFF,           PM_RESC_RAM_D1_OFF,          PM_RESC_RAM_E0_OFF,             \
         PM_RESC_RAM_E1_OFF

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_RegisterNotify(void);
void APP_InitWakeupSource(void);
void APP_WakeUpButtonConfig(void);
void APP_SetConstraints(uint8_t powerMode);
void APP_ReleaseConstraints(uint8_t powerMode);
uint32_t APP_GetWakeupTimeout(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
