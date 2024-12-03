/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_port.h"
#include "fsl_smc.h"
#include "fsl_rcm.h"
#include "fsl_lpuart.h"
#include "fsl_pmc.h"
#include "power_mode_switch.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "app.h"
/*${header:end}*/

extern void APP_SetClockRunFromVlpr(void);

/*${function:start}*/
static void APP_InitDefaultDebugConsole(void)
{
    uint32_t uartDefaultClkSrcFreq;
    CLOCK_SetLpuart0Clock(APP_DEBUG_UART_DEFAULT_CLKSRC);
    uartDefaultClkSrcFreq = CLOCK_GetFreq(APP_DEBUG_UART_DEFAULT_CLKSRC_NAME);
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, APP_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartDefaultClkSrcFreq);
}

static void APP_InitVlprDebugConsole(void)
{
    uint32_t uartVlprClkSrcFreq;
    CLOCK_SetLpuart0Clock(APP_DEBUG_UART_VLPR_CLKSRC);
    uartVlprClkSrcFreq = CLOCK_GetFreq(APP_DEBUG_UART_VLPR_CLKSRC_NAME);
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, APP_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartVlprClkSrcFreq);
}

void BOARD_InitHardware(void)
{
    /* Must configure pins before PMC_ClearPeriphIOIsolationFlag */
    BOARD_InitBootPins();

    /* Power related. */
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
    if (kRCM_SourceWakeup & RCM_GetPreviousResetSources(RCM)) /* Wakeup from VLLS. */
    {
        PMC_ClearPeriphIOIsolationFlag(PMC);
        NVIC_ClearPendingIRQ(LLWU_IRQn);
    }

    BOARD_InitBootClocks();
    APP_InitDefaultDebugConsole();
}

void APP_PowerPreSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode)
{
    /* Wait for debug console output finished. */
    while (!(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
    {
    }
    DbgConsole_Deinit();

    if ((kAPP_PowerModeRun != targetMode) && (kAPP_PowerModeVlpr != targetMode))
    {
        /*
         * Set pin for current leakage.
         * Debug console RX pin: Set to pinmux to disable.
         * Debug console TX pin: Don't need to change.
         */
        PORT_SetPinMux(DEBUG_CONSOLE_RX_PORT, DEBUG_CONSOLE_RX_PIN, kPORT_PinDisabledOrAnalog);
    }
}

void APP_PowerPostSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode)
{
    smc_power_state_t powerState = SMC_GetPowerModeState(SMC);

    /*
     * For some other platforms, if enter LLS mode from VLPR mode, when wakeup, the
     * power mode is VLPR. But for some platforms, if enter LLS mode from VLPR mode,
     * when wakeup, the power mode is RUN. In this case, the clock setting is still
     * VLPR mode setting, so change to RUN mode setting here.
     */
    if ((kSMC_PowerStateVlpr == originPowerState) && (kSMC_PowerStateRun == powerState))
    {
        APP_SetClockRunFromVlpr();
    }

    if ((kAPP_PowerModeRun != targetMode) && (kAPP_PowerModeVlpr != targetMode))
    {
        /*
         * Debug console RX pin is set to disable for current leakage, nee to re-configure pinmux.
         * Debug console TX pin: Don't need to change.
         */
        PORT_SetPinMux(DEBUG_CONSOLE_RX_PORT, DEBUG_CONSOLE_RX_PIN, DEBUG_CONSOLE_RX_PINMUX);
    }

    /* Set debug console clock source. */
    if (kSMC_PowerStateVlpr == powerState)
    {
        APP_InitVlprDebugConsole();
    }
    else
    {
        APP_InitDefaultDebugConsole();
    }
}
/*${function:end}*/
