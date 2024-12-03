/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_power.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_component_i3c_adapter.h"
/*${header:end}*/

/*${function:start}*/
i3c_master_adapter_resource_t demo_masterResource = {.base      = EXAMPLE_MASTER,
                                                     .transMode = kI3C_MasterTransferInterruptMode};
i3c_device_control_info_t i3cMasterCtlInfo        = {
           .funcs = (i3c_device_hw_ops_t *)&master_ops, .resource = &demo_masterResource, .isSecondary = false};

void BOARD_InitHardware(void)
{
    /* Select the main clock as source clock of USART0 (debug console) */
    CLOCK_Select(BOARD_DEBUG_USART_CLK_ATTACH);

    /* Enable lp_osc 1MHz */
    POWER_DisablePD(kPDRUNCFG_PD_LPO_OSC);

    /* I3C FCLK = 60M/5 = 12M */
    CLOCK_Select(kI3C_Clk_From_Fro);
    CLOCK_SetI3CFClkDiv(5);

    CLOCK_Select(kI3C_TC_Clk_From_LpOsc);
    CLOCK_SetI3CTCClkDiv(1);
    CLOCK_SetI3CSClkDiv(1);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    demo_masterResource.clockInHz = I3C_CLOCK_FREQUENCY;
    demo_masterResource.slowClockInHz = I3C_SLOW_CLOCK_FREQUENCY;
}
/*${function:end}*/
