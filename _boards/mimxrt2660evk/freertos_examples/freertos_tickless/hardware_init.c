/*
 * Copyright 2021, 2026 NXP
 * All rights reserved.
 *
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
    BOARD_InitBUTTONsPins();
    SystemCoreClockUpdate();

#if 0
    POWER_EnableWakeupSource(kPOWER_WakeupIrq_VbatLptmr);
    POWER_EnableWakeupSource(kPOWER_WakeupIrq_VbatGpioCh0);
    /* Waiting power driver expose this API*/
    POWER_ApplyWakeupSources();
#else
    uint32_t wakeupMask[12]             = {0U};
    wakeupMask[VBAT_LPTMR_IRQn / 32U] |= (1UL << (VBAT_LPTMR_IRQn % 32U));
    wakeupMask[VBAT_GPIO_CH0_IRQn / 32U] |= (1UL << (VBAT_GPIO_CH0_IRQn % 32U));
    POWERCON_EnableWakeupIRQ(SYSCON__POWERCON_CMC0_CTRL, wakeupMask);
#endif
}
/*${function:end}*/
