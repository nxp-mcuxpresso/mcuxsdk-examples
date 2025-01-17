/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _LPM_H_
#define _LPM_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SYSTICK_BASE       LPTMR1
#define SYSTICK_IRQn       LPTMR1_IRQn
#define SYSTICK_HANDLER    LPTMR1_IRQHandler
#define SYSTICK_CLOCK_NAME 32000000U

/* Peripheral LPI definitions */
#define CPU_PER_LPI_IDX_GPIO1           0U
#define CPU_PER_LPI_IDX_GPIO2           1U
#define CPU_PER_LPI_IDX_GPIO3           2U
#define CPU_PER_LPI_IDX_GPIO4           3U
#define CPU_PER_LPI_IDX_GPIO5           4U
#define CPU_PER_LPI_IDX_CAN1            5U
#define CPU_PER_LPI_IDX_CAN2            6U
#define CPU_PER_LPI_IDX_CAN3            7U
#define CPU_PER_LPI_IDX_CAN4            8U
#define CPU_PER_LPI_IDX_CAN5            9U
#define CPU_PER_LPI_IDX_LPUART1         10U
#define CPU_PER_LPI_IDX_LPUART2         11U
#define CPU_PER_LPI_IDX_LPUART3         12U
#define CPU_PER_LPI_IDX_LPUART4         13U
#define CPU_PER_LPI_IDX_LPUART5         14U
#define CPU_PER_LPI_IDX_LPUART6         15U
#define CPU_PER_LPI_IDX_LPUART7         16U
#define CPU_PER_LPI_IDX_LPUART8         17U
#define CPU_PER_LPI_IDX_WDOG3           18U
#define CPU_PER_LPI_IDX_WDOG4           19U
#define CPU_PER_LPI_IDX_WDOG5           20U   
   
   
/* MIX definitions */
#define PWR_NUM_MIX_SLICE               23U

#define PWR_MIX_SLICE_IDX_ANA           0U
#define PWR_MIX_SLICE_IDX_AON           1U
#define PWR_MIX_SLICE_IDX_BBSM          2U
#define PWR_MIX_SLICE_IDX_CAMERA        3U
#define PWR_MIX_SLICE_IDX_CCMSRCGPC     4U
#define PWR_MIX_SLICE_IDX_A55C0         5U
#define PWR_MIX_SLICE_IDX_A55C1         6U
#define PWR_MIX_SLICE_IDX_A55C2         7U
#define PWR_MIX_SLICE_IDX_A55C3         8U
#define PWR_MIX_SLICE_IDX_A55C4         9U
#define PWR_MIX_SLICE_IDX_A55C5         10U
#define PWR_MIX_SLICE_IDX_A55P          11U
#define PWR_MIX_SLICE_IDX_DDR           12U
#define PWR_MIX_SLICE_IDX_DISPLAY       13U
#define PWR_MIX_SLICE_IDX_GPU           14U
#define PWR_MIX_SLICE_IDX_HSIO_TOP      15U
#define PWR_MIX_SLICE_IDX_HSIO_WAON     16U
#define PWR_MIX_SLICE_IDX_M7            17U
#define PWR_MIX_SLICE_IDX_NETC          18U
#define PWR_MIX_SLICE_IDX_NOC           19U
#define PWR_MIX_SLICE_IDX_NPU           20U
#define PWR_MIX_SLICE_IDX_VPU           21U
#define PWR_MIX_SLICE_IDX_WAKEUP        22U

#define PWR_MEM_SLICE_IDX_AON           0U
#define PWR_MEM_SLICE_IDX_CAMERA        1U
#define PWR_MEM_SLICE_IDX_A55C0         2U
#define PWR_MEM_SLICE_IDX_A55C1         3U
#define PWR_MEM_SLICE_IDX_A55C2         4U
#define PWR_MEM_SLICE_IDX_A55C3         5U
#define PWR_MEM_SLICE_IDX_A55C4         6U
#define PWR_MEM_SLICE_IDX_A55C5         7U
#define PWR_MEM_SLICE_IDX_A55P          8U
#define PWR_MEM_SLICE_IDX_A55L3         9U
#define PWR_MEM_SLICE_IDX_DDR           10U
#define PWR_MEM_SLICE_IDX_DISPLAY       11U
#define PWR_MEM_SLICE_IDX_GPU           12U
#define PWR_MEM_SLICE_IDX_HSIO          13U
#define PWR_MEM_SLICE_IDX_M7            14U
#define PWR_MEM_SLICE_IDX_NETC          15U
#define PWR_MEM_SLICE_IDX_NOC1          16U
#define PWR_MEM_SLICE_IDX_NOC2          17U
#define PWR_MEM_SLICE_IDX_NPU           18U
#define PWR_MEM_SLICE_IDX_VPU           19U
#define PWR_MEM_SLICE_IDX_WAKEUP        20U

/* CPU definitions */
#define CPU_NUM_IDX         9U

#define CPU_IDX_M33P        0U
#define CPU_IDX_M7P         1U
#define CPU_IDX_A55C0       2U
#define CPU_IDX_A55C1       3U
#define CPU_IDX_A55C2       4U
#define CPU_IDX_A55C3       5U
#define CPU_IDX_A55C4       6U
#define CPU_IDX_A55C5       7U
#define CPU_IDX_A55P        8U

typedef enum _lpm_power_mode
{
    LPM_PowerModeRun = 0, /* Normal RUN mode */
    LPM_PowerModeWait,    /* WAIT mode. */
    LPM_PowerModeStop,    /* STOP mode. */
    LPM_PowerModeSuspend, /* SUSPEND mode. */
} lpm_power_mode_t;

typedef bool (*lpm_power_mode_callback_t)(lpm_power_mode_t curMode, lpm_power_mode_t newMode, void *data);

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/* Initialize the Low Power Management */
bool LPM_Init(void);

/* Deinitialize the Low Power Management */
void LPM_Deinit(void);

/* Save target mode to internal variable, not program hardware yet */
bool LPM_SetPowerMode(lpm_power_mode_t mode);

/* Get low power mode previously set.
 */
lpm_power_mode_t LPM_GetPowerMode(void);

/* LPM_SetPowerMode() won't switch system power status immediately,
 * instead, such operation is done by LPM_WaitForInterrupt().
 * It can be callled in idle task of FreeRTOS, or main loop in bare
 * metal application. The sleep depth of this API depends
 * on current power mode set by LPM_SetPowerMode().
 * The timeoutMilliSec means if no interrupt occurs before timeout, the
 * system will be waken up by systick. If timeout exceeds hardware timer
 * limitation, timeout will be reduced to maximum time of hardware.
 * timeoutMilliSec only works in FreeRTOS, in bare metal application,
 * it's just ignored.
 * Return true if power mode switch succeeds.
 */
bool LPM_WaitForInterrupt(uint32_t timeoutMilliSec);

/* Register power mode switch listener. When LPM_SetPowerMode()
 * is called, all the registered listeners will be invoked. The
 * listener returns true if it allows the power mode switch,
 * otherwise returns FALSE.
 */
void LPM_RegisterPowerListener(lpm_power_mode_callback_t callback, void *data);

/* Unregister power mode switch listener */
void LPM_UnregisterPowerListener(lpm_power_mode_callback_t callback, void *data);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _LPM_H_ */
