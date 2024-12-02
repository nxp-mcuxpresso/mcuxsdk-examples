/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "fsl_spc.h"
#include "board.h"
#include "app.h"
#include "fsl_port.h"
#include "fsl_spc.h"
#include "fsl_cmc.h"
#include "fsl_lptmr.h"
#include "fsl_vbat.h"
#include "fsl_clock.h"
#include "fsl_wuu.h"
#include "fsl_gpio.h"
/*${header:end}*/

/*${function:start}*/
void APP_InitDebugConsole(void)
{
    /*
     * Debug console RX pin is set to disable for current leakage, need to re-configure pinmux.
     * Debug console TX pin: Don't need to change.
     */
    BOARD_InitPins();
    BOARD_BootClockFROHF48M();
    BOARD_InitDebugConsole();
}

void APP_DeinitDebugConsole(void)
{
    DbgConsole_Deinit();
    PORT_SetPinMux(APP_DEBUG_CONSOLE_RX_PORT, APP_DEBUG_CONSOLE_RX_PIN, kPORT_MuxAsGpio);
    PORT_SetPinMux(APP_DEBUG_CONSOLE_TX_PORT, APP_DEBUG_CONSOLE_TX_PIN, kPORT_MuxAsGpio);
}

void APP_LPTMR_IRQ_HANDLER(void)
{
    if (kLPTMR_TimerInterruptEnable & LPTMR_GetEnabledInterrupts(APP_LPTMR))
    {
        LPTMR_DisableInterrupts(APP_LPTMR, kLPTMR_TimerInterruptEnable);
        LPTMR_ClearStatusFlags(APP_LPTMR, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(APP_LPTMR);
    }
}

void APP_VBAT_IRQ_HANDLER(void)
{
    if (VBAT_GetStatusFlags(APP_VBAT) & kVBAT_StatusFlagWakeupPin)
    {
        VBAT_ClearStatusFlags(APP_VBAT, kVBAT_StatusFlagWakeupPin);
    }
}

void APP_WUU_IRQ_HANDLER(void)
{
    if (WUU_GetExternalWakeUpPinsFlag(APP_WUU) == (1UL << (uint32_t)APP_WUU_WAKEUP_BUTTON_IDX))
    {
        /* Enter into WUU IRQ handler, 3 timess toggle. */
        WUU_ClearExternalWakeUpPinsFlag(APP_WUU, (1UL << (uint32_t)APP_WUU_WAKEUP_BUTTON_IDX));
    }
}

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockFROHF48M();
    BOARD_InitDebugConsole();

    /* Release the I/O pads and certain peripherals to normal run mode state, for in Power Down mode
     * they will be in a latched state. */
    if ((CMC_GetSystemResetStatus(APP_CMC) & kCMC_WakeUpReset) != 0UL)
    {
        SPC_ClearPeriphIOIsolationFlag(APP_SPC);
    }
}
/*${function:end}*/
