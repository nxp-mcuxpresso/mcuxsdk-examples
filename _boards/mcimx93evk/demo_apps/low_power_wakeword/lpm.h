/*
 * Copyright 2022-2023 NXP
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

#if defined(SRTM_DDR_RETENTION_USED) && SRTM_DDR_RETENTION_USED
/*
 * Hook to do what is needed before accessing DDR.
 * E.g. restore DDR when in retention.
 */
void LPM_ddr_pre_access(void);

/*
 * Hook to insert code to save power when DDR access is not needed by application.
 * Typically place DDR in retention.
 */
void LPM_ddr_post_access(void);
#endif

#if defined(SRTM_OCRAM_POWER_OPTIM_USED) && SRTM_OCRAM_POWER_OPTIM_USED
/*
 * to be called before ocram access when power optimizations prevents its access
 */
void LPM_ocram_pre_access(void);

/*
 * to be called after ocram access to save some power (e.g. reducing bus clocks)
 */
void LPM_ocram_post_access(void);
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _LPM_H_ */
