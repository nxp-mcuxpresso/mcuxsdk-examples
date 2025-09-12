/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "app.h"
#include "board.h"
#include "fsl_lpit.h"
#include "fsl_lpuart.h"
#include "CO_app_NXP.h"
/*${header:end}*/

/*${function:start}*/
volatile bool rxBufferEmpty = true;
volatile bool rxOnGoing = false;
extern uint8_t g_rxBuffer[1];
lpuart_handle_t g_lpuartHandle;
lpuart_transfer_t receiveXfer = {.dataSize = 1U, .data = g_rxBuffer};

void LPUART_UserCallback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
    userData = userData;

    if (kStatus_LPUART_RxIdle == status)
    {
        rxBufferEmpty = false;
        rxOnGoing     = false;
    }
}

void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitCANPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_SetIpSrc(kCLOCK_Lpit0, kCLOCK_IpSrcSircAsync);

    LPUART_TransferCreateHandle(EXAMPLE_LPUART, &g_lpuartHandle, LPUART_UserCallback, NULL);
}

void EXAMPLE_UARTProcess(void)
{
    if ((!rxOnGoing) && rxBufferEmpty)
    {
        rxOnGoing = true;
        LPUART_TransferReceiveNonBlocking(EXAMPLE_LPUART, &g_lpuartHandle, &receiveXfer, NULL);
    }
}

void EXAMPLE_InitTimer(void)
{
    lpit_config_t lpitConfig;
    lpit_chnl_params_t lpitChannelConfig;

    LPIT_GetDefaultConfig(&lpitConfig);
    LPIT_Init(EXAMPLE_TIMER, &lpitConfig);

    lpitChannelConfig.chainChannel          = false;
    lpitChannelConfig.enableReloadOnTrigger = false;
    lpitChannelConfig.enableStartOnTrigger  = false;
    lpitChannelConfig.enableStopOnTimeout   = false;
    lpitChannelConfig.timerMode             = kLPIT_PeriodicCounter;
    lpitChannelConfig.triggerSelect         = kLPIT_Trigger_TimerChn0;
    lpitChannelConfig.triggerSource         = kLPIT_TriggerSource_External;

    /* Init lpit channel 0 */
    LPIT_SetupChannel(EXAMPLE_TIMER, kLPIT_Chnl_0, &lpitChannelConfig);

    /* Set timer period for channel 0 */
    LPIT_SetTimerPeriod(EXAMPLE_TIMER, kLPIT_Chnl_0, EXAMPLE_TIMER_CLK_FREQ / 1000U);
}

void EXAMPLE_StartTimer(void)
{
    LPIT_EnableInterrupts(EXAMPLE_TIMER, kLPIT_Channel0TimerInterruptEnable);
    EnableIRQ(EXAMPLE_TIMER_IRQn);

    LPIT_StartTimer(EXAMPLE_TIMER, kLPIT_Chnl_0);
}

void EXAMPLE_StopTimer(void)
{
    LPIT_DisableInterrupts(EXAMPLE_TIMER, kLPIT_Channel0TimerInterruptEnable);
    DisableIRQ(EXAMPLE_TIMER_IRQn);

    LPIT_StopTimer(EXAMPLE_TIMER, kLPIT_Chnl_0);
}

void EXAMPLE_TIMER_IRQHandler(void)
{
    if ((LPIT_GetStatusFlags(EXAMPLE_TIMER) & kLPIT_Channel0TimerFlag) != 0U)
    {
        /* Clear interrupt flag.*/
        LPIT_ClearStatusFlags(EXAMPLE_TIMER, kLPIT_Channel0TimerFlag);
        canopen_app_interrupt();
    }

    SDK_ISR_EXIT_BARRIER;
}
/*${function:end}*/
