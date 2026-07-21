/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_spc.h"
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

    /* Disable CPU1 clock. */
    CLOCK_EnableCpu1Clock(SYSCON, false);
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
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);

#if defined(SDIO_ENABLED)
    /* attach FRO HF to USDHC */
    CLOCK_AttachClk(kFRO_HF_to_USDHC);
    CLOCK_SetClkDiv(kCLOCK_DivUSdhcClk, 1u);
#elif defined(SDIO_SPI_ENABLED)
    /* attach FRO 96M to FLEXCOMM1 */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom1Clk, 1u);
    CLOCK_AttachClk(kFRO_HF_DIV_to_FLEXCOMM1);
#endif

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
