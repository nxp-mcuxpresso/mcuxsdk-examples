/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_power.h"
/*${header:end}*/

/*${macro:start}*/

/*${macro:end}*/

/*${variable:start}*/
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format on */

    SystemPlatformInit();

    BOARD_InitDebugConsolePins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();
}

int32_t APP_GRP_RESET()
{
	int32_t status = SCMI_SystemPowerStateSet(SCMI_A2P, 0, SCMI_SYS_STATE_GRP_RESET);
	if (status != SCMI_ERR_SUCCESS)
 	{
 		PRINTF("SCMI_SystemPowerStateSet SYS_STATE_GRP_RESET FAIL: %d\r\n", status);
 	}
	return status;
} 

/*${function:end}*/
