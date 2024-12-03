/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_port.h"
#include "fsl_smc.h"
#include "fsl_rcm.h"
#include "fsl_pmc.h"
#include "fsl_uart.h"
#include "fsl_notifier.h"
#include "power_manager.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/

/* Delay at least 1ms for FLL stable. */
static void APP_FllStableDelay(void)
{
    volatile uint32_t i = 30000U;
    while (i--)
    {
        __NOP();
    }
}

void APP_SetClockVlpr(void)
{
    const sim_clock_config_t simConfig = {
        .pllFllSel = 0U,          /* PLLFLLSEL select FLL */
        .er32kSrc  = 0U,          /* ERCLK32K selection, use OSC */
        .clkdiv1   = 0x03000000U, /* SIM_CLKDIV1 */
    };

    CLOCK_SetSimSafeDivs();
    CLOCK_SetInternalRefClkConfig(kMCG_IrclkEnable, kMCG_IrcFast, 0U);

    /* MCG works in FEE mode now, will switch to BLPI mode. */

    CLOCK_SetFbiMode(kMCG_Dmx32Default, kMCG_DrsLow, NULL); /* Enter FBI. */
    CLOCK_SetLowPowerEnable(true);                          /* Enter BLPI. */

    CLOCK_SetSimConfig(&simConfig);
}

void APP_SetClockRunFromVlpr(void)
{
    const sim_clock_config_t simConfig = {
        .pllFllSel = 0U,          /* PLLFLLSEL select FLL */
        .er32kSrc  = 0U,          /* ERCLK32K selection, use OSC */
        .clkdiv1   = 0x02000000U, /* SIM_CLKDIV1 */
    };

    CLOCK_SetSimSafeDivs();

    /* Currently in BLPI mode, will switch to FEE mode. */
    /* Enter FBI. */
    CLOCK_SetLowPowerEnable(false);
    /* Enter FEE. */
    CLOCK_SetFeeMode(0U, kMCG_Dmx32Fine, kMCG_DrsMidHigh, APP_FllStableDelay);

    CLOCK_SetSimConfig(&simConfig);
}

static void APP_InitDebugConsole(void)
{
    uint32_t uartClkSrcFreq;
    uartClkSrcFreq = CLOCK_GetFreq(APP_DEBUG_UART_CLKSRC_NAME);
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, APP_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}

void BOARD_InitHardware(void)
{
    /* Must configure pins before PMC_ClearPeriphIOIsolationFlag */
    BOARD_InitPins();

    /* Power related. */
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
    if (kRCM_SourceWakeup & RCM_GetPreviousResetSources(RCM)) /* Wakeup from VLLS. */
    {
        PMC_ClearPeriphIOIsolationFlag(PMC);
        NVIC_ClearPendingIRQ(LLWU_IRQn);
    }
    BOARD_BootClockRUN();
    APP_InitDebugConsole();
}

status_t callback0(notifier_notification_block_t *notify, void *dataPtr)
{
    user_callback_data_t *userData     = (user_callback_data_t *)dataPtr;
    status_t ret                       = kStatus_Fail;
    app_power_mode_t targetMode        = ((power_user_config_t *)notify->targetConfig)->mode;
    smc_power_state_t originPowerState = userData->originPowerState;

    switch (notify->notifyType)
    {
        case kNOTIFIER_NotifyBefore:
            userData->beforeNotificationCounter++;
            /* Wait for debug console output finished. */
            while (!(kUART_TransmissionCompleteFlag & UART_GetStatusFlags((UART_Type *)BOARD_DEBUG_UART_BASEADDR)))
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

            ret = kStatus_Success;
            break;
        case kNOTIFIER_NotifyRecover:
            break;
        case kNOTIFIER_CallbackAfter:
            userData->afterNotificationCounter++;
            if ((kAPP_PowerModeRun != targetMode) && (kAPP_PowerModeVlpr != targetMode))
            {
                /*
                 * Debug console RX pin is set to disable for current leakage, nee to re-configure pinmux.
                 * Debug console TX pin: Don't need to change.
                 */
                PORT_SetPinMux(DEBUG_CONSOLE_RX_PORT, DEBUG_CONSOLE_RX_PIN, DEBUG_CONSOLE_RX_PINMUX);
            }

            /*
             * For some other platforms, if enter LLS mode from VLPR mode, when wakeup, the
             * power mode is VLPR. But for some platforms, if enter LLS mode from VLPR mode,
             * when wakeup, the power mode is RUN. In this case, the clock setting is still
             * VLPR mode setting, so change to RUN mode setting here.
             */
            if ((kSMC_PowerStateVlpr == originPowerState) && (kSMC_PowerStateRun == SMC_GetPowerModeState(SMC)))
            {
                APP_SetClockRunFromVlpr();
            }

            APP_InitDebugConsole();

            ret = kStatus_Success;
            break;
        default:
            break;
    }
    return ret;
}
/*${function:end}*/
