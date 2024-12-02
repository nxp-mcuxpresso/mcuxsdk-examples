/*
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_component_i3c_adapter.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
i3c_master_adapter_resource_t demo_masterResource = {.base      = EXAMPLE_MASTER,
                                                     .transMode = kI3C_MasterTransferInterruptMode};
i3c_device_control_info_t i3cMasterCtlInfo        = {
    .funcs = (i3c_device_hw_ops_t *)&master_ops, .resource = &demo_masterResource, .isSecondary = false};

void BOARD_InitHardware(void)
{
    /* Attach 12M clock to I3C */
    CLOCK_SetClockDiv(kCLOCK_DivI3C0_FCLK, 1U);
    CLOCK_AttachClk(kFRO_LF_DIV_to_I3C0FCLK);

    BOARD_InitDEBUG_UARTPins();
    BOARD_InitI3CPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    demo_masterResource.clockInHz = I3C_CLOCK_FREQUENCY;
    demo_masterResource.slowClockInHz = I3C_CLOCK_FREQUENCY;
}
/*${function:end}*/
