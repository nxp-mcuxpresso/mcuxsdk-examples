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
#include "fsl_pmc.h"
#include "fsl_lpuart.h"
#include "fsl_notifier.h"
#include "power_manager.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "app.h"
/*${header:end}*/

extern void APP_InitClock(void); /* SCG init function defined in clock_scg.c */

/*${function:start}*/
static void APP_InitDebugConsole(void)
{
    CLOCK_SetIpSrc(kCLOCK_Lpuart0, kCLOCK_IpSrcSircAsync);
    uint32_t uartClkSrcFreq = CLOCK_GetIpFreq(kCLOCK_Lpuart0);
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, APP_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
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

    APP_InitClock();
    APP_InitDebugConsole();
}

status_t callback0(notifier_notification_block_t *notify, void *dataPtr)
{
    user_callback_data_t *userData = (user_callback_data_t *)dataPtr;
    status_t ret                   = kStatus_Fail;
    app_power_mode_t targetMode    = ((power_user_config_t *)notify->targetConfig)->mode;

    switch (notify->notifyType)
    {
        case kNOTIFIER_NotifyBefore:
            userData->beforeNotificationCounter++;
            /* Wait for debug console output finished. */
            while (
                !(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
            {
            }
            DbgConsole_Deinit();

            if ((kAPP_PowerModeRun != targetMode) && (kAPP_PowerModeHsrun != targetMode) &&
                (kAPP_PowerModeVlpr != targetMode))
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
            if ((kAPP_PowerModeRun != targetMode) && (kAPP_PowerModeHsrun != targetMode) &&
                (kAPP_PowerModeVlpr != targetMode))
            {
                /*
                 * Debug console RX pin is set to disable for current leakage, nee to re-configure pinmux.
                 * Debug console TX pin: Don't need to change.
                 */
                PORT_SetPinMux(DEBUG_CONSOLE_RX_PORT, DEBUG_CONSOLE_RX_PIN, DEBUG_CONSOLE_RX_PINMUX);
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
