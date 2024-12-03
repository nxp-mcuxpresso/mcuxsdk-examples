/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_qtmr.h"
#include "fsl_xbara.h"
#include "peripherals.h"
#include "app.h"
/*${header:end}*/

void DEMO_TriggerSignalISR(void);

/*${function:start}*/
#pragma interrupt alignsp saveall
void ivINT_TMRA_0(void)
{
    /* Clear interrupt flag.*/
    QTMR_ClearStatusFlags(BOARD_QTMR_BASEADDR, BOARD_QTMR_CHANNEL, (uint16_t)kQTMR_CompareFlag);
    DEMO_TriggerSignalISR();
}
#pragma interrupt off

/* Initialize QDC clock, return the clock frequency. */
uint32_t DEMO_InitQdcClock(void)
{
    return CLOCK_GetIpClkSrcFreq(kCLOCK_QDC);
}

/* Initializes the 1kHz TRIGGER signal, ready to start. */
void DEMO_InitTrigger(void)
{
    /* Setup QTMR as TRIGGER. */
    qtmr_channel_config_t sQtmrChannelConfig;
    qtmr_config_t sQtmrConfig                       = {0};
    sQtmrConfig.psChannelConfig[BOARD_QTMR_CHANNEL] = &sQtmrChannelConfig;

    QTMR_GetChannelDefaultConfig(&sQtmrChannelConfig);
    sQtmrChannelConfig.sInputConfig.ePrimarySource = QTMR_PRIMARY_SOURCE;
    sQtmrChannelConfig.sCountConfig.eCountMode     = kQTMR_CountPrimarySrcRiseEdge;
    sQtmrChannelConfig.sCountConfig.eCountLength   = kQTMR_CountLengthUntilCompare;
    sQtmrChannelConfig.u16EnabledInterruptMask     = kQTMR_CompareInterruptEnable;
    sQtmrChannelConfig.sOutputConfig.eOutputMode   = kQTMR_OutputToggleOnAltCompareReg;
    sQtmrChannelConfig.u16Comp1                    = MSEC_TO_COUNT(1U, QTMR_SOURCE_CLOCK) - 1;
    sQtmrChannelConfig.u16Comp2                    = MSEC_TO_COUNT(1U, QTMR_SOURCE_CLOCK) + 1;

    /* Enable interrupt with priority */
    EnableIRQWithPriority(TMRA_0_IRQn, 1);

    QTMR_Init(TMRA, &sQtmrConfig);

    XBARA_SetSignalsConnection(XBARA, kXBARA_InputTa0Out, kXBARA_OutputQdcTrig);
}

/* Start the 1kHz TRIGGER signal. */
void DEMO_StartTrigger(void)
{
    /* Start the channel to count */
    QTMR_EnableChannels(BOARD_QTMR_BASEADDR, (uint16_t)(1UL << (uint16_t)BOARD_QTMR_CHANNEL));
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
