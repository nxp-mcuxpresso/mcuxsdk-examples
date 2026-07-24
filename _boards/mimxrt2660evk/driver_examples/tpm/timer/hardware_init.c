/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "app.h"
#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();

#if 0
    POWER_EnableWakeupSource(kPOWER_WakeupIrq_HspQtpm0Ch0);
    /* Waiting power driver expose this API*/
    POWER_ApplyWakeupSources();
#else
    uint32_t wakeupMask[12]             = {0U};
    wakeupMask[BOARD_TPM_IRQ_NUM / 32U] = (1UL << (BOARD_TPM_IRQ_NUM % 32U));
    POWERCON_EnableWakeupIRQ(SYSCON__POWERCON_CMC0_CTRL, wakeupMask);
#endif
}
/*${function:end}*/
