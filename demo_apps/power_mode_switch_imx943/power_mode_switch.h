/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _POWER_MODE_SWITCH_H_
#define _POWER_MODE_SWITCH_H_

#include "fsl_common.h"
//#include "hal_clock.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined(CPU_MIMX94398AVKM_cm7_core0)
#define MCU_HAL_CLOCK_ID kCLOCK_M70
#define APP_CPU_ID SYSTEM_PLATFORM_M70_ID
#define MCU_NVIC_ISER_NUM (8U)
#define APP_CPU_PWR_MIX_SLICE_IDX POWER_MIX_SLICE_IDX_M7_0
#define APP_CPU_PWR_MEM_SLICE_IDX POWER_MEM_SLICE_IDX_M7_0
#elif defined(CPU_MIMX94398AVKM_cm7_core1)
#define MCU_HAL_CLOCK_ID kCLOCK_M71
#define APP_CPU_ID SYSTEM_PLATFORM_M71_ID
#define MCU_NVIC_ISER_NUM (8U)
#define APP_CPU_PWR_MIX_SLICE_IDX POWER_MIX_SLICE_IDX_M7_1
#define APP_CPU_PWR_MEM_SLICE_IDX POWER_MEM_SLICE_IDX_M7_1
#elif defined(CPU_MIMX94398AVKM_cm33_core1)
#define MCU_HAL_CLOCK_ID kCLOCK_M33sync
#define APP_CPU_ID SYSTEM_PLATFORM_M33S_ID
#define MCU_NVIC_ISER_NUM (16U)
#define APP_CPU_PWR_MIX_SLICE_IDX POWER_MIX_SLICE_IDX_NETC
#define APP_CPU_PWR_MEM_SLICE_IDX POWER_MEM_SLICE_IDX_NETC
#else
#error "Pls define CPU macro!"
#endif

/* Index of first mask to set */
#define APP_CPU_ID_MASK_IDX (0U)

#define CLOCK_FREQ_32K_HZ (32000UL)
#define LPTMR_PRESCALER_CLOCK_FREQ CLOCK_FREQ_32K_HZ

#define SM_PLATFORM_SM_LMM_ID     0
#define SM_PLATFORM_M33S_LMM_ID   1
#define SM_PLATFORM_M70_LMM_ID    2
#define SM_PLATFORM_M71_LMM_ID    3
#define SM_PLATFORM_AP_LMM_ID     4

#if defined(CPU_MIMX94398AVKM_cm7_core0) || defined(CPU_MIMX94398AVKM_cm7_core1)
#define APP_LPM_SETTING SCMI_CPU_LPM_SETTING_ON_RUN_WAIT_STOP
#else
#define APP_LPM_SETTING SCMI_CPU_LPM_SETTING_ON_ALWAYS
#endif

/* Power mode definition used in application. */
typedef enum _app_power_mode
{
    kAPP_PowerModeRun = 'A', /* Normal RUN mode */
    kAPP_PowerModeWait,      /* WAIT mode. */
    kAPP_PowerModeStop,      /* STOP mode. */
    kAPP_PowerModeSuspend,   /* SUSPEND mode. */

} app_power_mode_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _POWER_MODE_SWITCH_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
