/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "app.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_lptmr.h"
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
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Clock setting for FLEXCAN */
    CLOCK_AttachClk(kFRO_HF_DIV_to_FLEXCAN0);
    CLOCK_AttachClk(kFRO12M_to_LPTMR0);
    CLOCK_SetClockDiv(kCLOCK_DivFLEXCAN0, 1U);

    CLOCK_SetClockDiv(kCLOCK_DivFRO_HF_DIV, 1U);
    CLOCK_SetClockDiv(kCLOCK_DivLPTMR0, 1u);

    CLOCK_SetupFRO16KClocking(kCLKE_16K_SYSTEM | kCLKE_16K_COREMAIN);

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
    lptmr_config_t lptmrConfig;

    LPTMR_GetDefaultConfig(&lptmrConfig);
    LPTMR_Init(EXAMPLE_TIMER, &lptmrConfig);

    LPTMR_SetTimerPeriod(EXAMPLE_TIMER, EXAMPLE_TIMER_CLK_FREQ / 1000U);
}

void EXAMPLE_StartTimer(void)
{
    LPTMR_EnableInterrupts(EXAMPLE_TIMER, kLPTMR_TimerInterruptEnable);
    EnableIRQ(EXAMPLE_TIMER_IRQn);

    LPTMR_StartTimer(EXAMPLE_TIMER);
}

void EXAMPLE_StopTimer(void)
{
    LPTMR_DisableInterrupts(EXAMPLE_TIMER, kLPTMR_TimerInterruptEnable);
    DisableIRQ(EXAMPLE_TIMER_IRQn);

    LPTMR_StopTimer(EXAMPLE_TIMER);
}

void EXAMPLE_TIMER_IRQHandler(void)
{
    if ((LPTMR_GetStatusFlags(EXAMPLE_TIMER) & kLPTMR_TimerCompareFlag) != 0U)
    {
        /* Clear interrupt flag.*/
        LPTMR_ClearStatusFlags(EXAMPLE_TIMER, kLPTMR_TimerCompareFlag);
        canopen_app_interrupt();
    }

    SDK_ISR_EXIT_BARRIER;
}
/*${function:end}*/
