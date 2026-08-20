/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_pgmc.h"
#include "fsl_pf5020.h"
/*${header:end}*/

/*${function:start}*/
void APP_TriggerPMICStandby(bool enable)
{
    PGMC_PPC_TriggerPMICStandbySoftMode(PGMC_PPC0, enable);
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

void BOARD_InitPMIC(pf5020_handle_t *handle)
{
    /* RT1170 requires PWRUP_CTRL[1:0] = 0b00 to meet the power-down sequence.
     * The PF5020 default register value may be non-zero, so clear bit[1:0] here. */
    (void)PF5020_ModifyReg(handle, PF5020_PWRUP_CTRL, 0x3U, 0x0U);
}

/*${function:end}*/
