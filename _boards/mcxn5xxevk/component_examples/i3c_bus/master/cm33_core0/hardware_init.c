/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
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
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1U);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* Attach PLL0 clock to I3C, 150MHz / 6 = 25MHz. */
    CLOCK_SetClkDiv(kCLOCK_DivI3c1FClk, 6U);
    CLOCK_AttachClk(kPLL0_to_I3C1FCLK);

    CLOCK_SetClkDiv(kCLOCK_DivI3c0FClkStc, 1U);
    CLOCK_AttachClk(kCLK_1M_to_I3C0FCLKSTC);

    CLOCK_SetClkDiv(kCLOCK_DivI3c0FClkS, 1U);
    CLOCK_AttachClk(kCLK_1M_to_I3C0FCLKS);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    demo_masterResource.clockInHz = I3C_MASTER_CLOCK_FREQUENCY;
}
/*${function:end}*/
