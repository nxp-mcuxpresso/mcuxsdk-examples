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
#include "fsl_pit.h"
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
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitFlexCANPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Clock setting for FLEXCAN */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcan012PeClk, 1U);
    CLOCK_AttachClk(kAIPS_PLAT_CLK_to_FLEXCAN012_PE);

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
    pit_config_t config;

    PIT_GetDefaultConfig(&config);
    PIT_Init(EXAMPLE_TIMER, &config);

    PIT_SetTimerPeriod(EXAMPLE_TIMER, kPIT_Chnl_0, EXAMPLE_TIMER_CLK_FREQ / 1000U);
}

void EXAMPLE_StartTimer(void)
{
    PIT_EnableInterrupts(EXAMPLE_TIMER, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
    PIT_ClearStatusFlags(EXAMPLE_TIMER, kPIT_Chnl_0, kPIT_TimerFlag);
    EnableIRQ(EXAMPLE_TIMER_IRQn);

    PIT_StartTimer(EXAMPLE_TIMER, kPIT_Chnl_0);
}

void EXAMPLE_StopTimer(void)
{
    PIT_DisableInterrupts(EXAMPLE_TIMER, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
    PIT_ClearStatusFlags(EXAMPLE_TIMER, kPIT_Chnl_0, kPIT_TimerFlag);
    DisableIRQ(EXAMPLE_TIMER_IRQn);

    PIT_StopTimer(EXAMPLE_TIMER, kPIT_Chnl_0);
}

void EXAMPLE_TIMER_IRQHandler(void)
{
    if (PIT_GetStatusFlags(EXAMPLE_TIMER, kPIT_Chnl_0))
    {
        /* Clear interrupt flag.*/
        PIT_ClearStatusFlags(EXAMPLE_TIMER, kPIT_Chnl_0, kPIT_TimerFlag);
        canopen_app_interrupt();
    }

    SDK_ISR_EXIT_BARRIER;
}
/*${function:end}*/
