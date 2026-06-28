/*
 * Copyright 2026 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "app.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_Init6524Pins();
    BOARD_InitCANPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /*Clock setting for FLEXCAN*/
    clock_root_config_t rootCfg = {0};
    rootCfg.mux                 = FLEXCAN_CLOCK_SOURCE_SELECT;
    rootCfg.div                 = FLEXCAN_CLOCK_SOURCE_DIVIDER;
    CLOCK_SetRootClock(kCLOCK_Root_Can2, &rootCfg);

    pcal6524_handle_t handle;
    BOARD_InitPCAL6524(&handle);
    PCAL6524_SetDirection(&handle, (1 << BOARD_PCAL6524_CAN2_STBY), kPCAL6524_Output);
    PCAL6524_ClearPins(&handle, (1 << BOARD_PCAL6524_CAN2_STBY));
}
/*${function:end}*/
