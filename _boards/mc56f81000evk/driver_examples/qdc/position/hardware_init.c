/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_pit.h"
#include "fsl_xbara.h"
#include "peripherals.h"
#include "app.h"
/*${header:end}*/

void DEMO_TriggerSignalISR(void);

/*${function:start}*/
#pragma interrupt alignsp saveall
void ivINT_PIT0_ROLLOVR(void);
void ivINT_PIT0_ROLLOVR(void)
{
    PIT_ClearStatusFlags(PIT0);
    DEMO_TriggerSignalISR();
}
#pragma interrupt off

/* Initialize QDC clock, return the clock frequency. */
uint32_t DEMO_InitQdcClock(void)
{
    CLOCK_ConfigQDCClockSrc(kCLOCK_IPClkSrc_BusClk);

    return CLOCK_GetIpClkSrcFreq(kCLOCK_QDC);
}

/* Initializes the 1kHz TRIGGER signal, ready to start. */
void DEMO_InitTrigger(void)
{
    /* Setup PIT as TRIGGER. */
    pit_config_t sPitConfig;

    PIT_GetDefaultConfig(&sPitConfig);
    sPitConfig.bEnableTimer     = false;
    sPitConfig.bEnableInterrupt = true;
    sPitConfig.u32PeriodCount   = (CLOCK_GetFreq(kCLOCK_BusClk) / 1000) - 1; /* 1kHz. */

    EnableIRQWithPriority(PIT0_ROLLOVR_IRQn, 2);

    PIT_Init(PIT0, &sPitConfig);

    XBARA_SetSignalsConnection(XBARA, kXBARA_InputPit0SyncOut, kXBARA_OutputQdcTrig);
}

/* Start the 1kHz TRIGGER signal. */
void DEMO_StartTrigger(void)
{
    PIT_StartTimer(PIT0);
}

/* Initialize QDC PHASEA and PHASEB signal connection. */
void DEMO_InitQdcInputSignal(void)
{
    /* XBAR. */
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputXbIn5, kXBARA_OutputQdcPha);
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputXbIn7, kXBARA_OutputQdcPhb);
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputXbIn3, kXBARA_OutputQdcIndex);
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    BOARD_InitDebugConsole();

    XBARA_Init(XBARA);

    EnableIRQWithPriority(BOARD_DEBUG_UART_RX_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    EnableIRQWithPriority(BOARD_DEBUG_UART_TX_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    EnableIRQWithPriority(BOARD_DEBUG_UART_RX_ERR_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    EnableIRQWithPriority(BOARD_DEBUG_UART_TX_IDLE_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    SetIRQBasePriority(0);
}
/*${function:end}*/
