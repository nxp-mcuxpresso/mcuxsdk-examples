/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "hal_power.h"
#include "hal_clock.h"
#include "sm_platform.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    hal_pwr_s_t pwrst = {
        .did = HAL_POWER_PLATFORM_MIX_SLICE_IDX_DISPLAY,
        .st = hal_power_state_on,
    };

    SM_Platform_Init();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    /* Power on the Displaymix */
    HAL_PowerSetState(&pwrst);
    if (HAL_PowerSetState(&pwrst) != SCMI_ERR_SUCCESS) {
        PRINTF("Set Displaymix power failed: %d\r\n", HAL_PowerGetState(&pwrst));
    }
    if (HAL_PowerGetState(&pwrst) != hal_power_state_on) {
        PRINTF("Get Displaymix power failed: %d\r\n", HAL_PowerGetState(&pwrst));
    }
}
/*${function:end}*/
