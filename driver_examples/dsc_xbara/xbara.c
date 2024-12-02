/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_qtmr.h"
#include "fsl_xbara.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile bool xbaraIsrFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
#pragma interrupt alignsp saveall
void DEMO_XBARA_IRQHandler(void);
void DEMO_XBARA_IRQHandler(void)
{
    if (XBARA_GetStatusFlags(DEMO_XBARA_BASEADDR) & kXBARA_EdgeDetectionOut0Flag)
    {
        /* Clear interrupt flag. */
        XBARA_ClearStatusFlags(DEMO_XBARA_BASEADDR, kXBARA_EdgeDetectionOut0Flag);
        xbaraIsrFlag = true;
    }
}
#pragma interrupt off

static void DEMO_InitQtmr(void)
{
    /* The QTMR maximum counter = 128*65536, can't generate 1Hz output by only one channel, so cascade CH0 and CH1 */

    qtmr_channel_config_t sFirstChannelConfig;
    qtmr_channel_config_t sSecondChannelConfig;
    qtmr_config_t sQtmrConfig = {0};

    sQtmrConfig.psChannelConfig[0] = &sFirstChannelConfig;
    sQtmrConfig.psChannelConfig[1] = &sSecondChannelConfig;

    /* Configure the QTMR CH0 to output 500Hz(CH0 actually works in 1KHz, it output is toggled on compare) */
    QTMR_GetChannelDefaultConfig(&sFirstChannelConfig);
    /* Setup the primary count source*/
    sFirstChannelConfig.sInputConfig.ePrimarySource = kQTMR_PrimarySrcIPBusClockDivide8;
    /* Setup count length to until compare*/
    sFirstChannelConfig.sCountConfig.eCountLength = kQTMR_CountLengthUntilCompare;
    /* Setup output mode to toggle on compare */
    sFirstChannelConfig.sOutputConfig.eOutputMode = kQTMR_OutputToggleOnCompare;
    /* Set the first channel period to be 1 millisecond */
    sFirstChannelConfig.u16Comp1 = MSEC_TO_COUNT(1U, CLOCK_GetFreq(kCLOCK_BusClk) / 8);

    /* Configue the QTMR CH1 to output 1Hz(CH1 actually works in 2Hz, its output is toggled on compare */
    QTMR_GetChannelDefaultConfig(&sSecondChannelConfig);
    /* Setup the primary count source*/
    sSecondChannelConfig.sInputConfig.ePrimarySource = kQTMR_PrimarySrcChannel0Output;
    /* Setup count length to until compare*/
    sSecondChannelConfig.sCountConfig.eCountLength = kQTMR_CountLengthUntilCompare;
    /* Setup output mode to toggle on compare */
    sSecondChannelConfig.sOutputConfig.eOutputMode = kQTMR_OutputToggleOnCompare;
    /* Set the second channel period to be 500 millisecond */
    sSecondChannelConfig.u16Comp1 = MSEC_TO_COUNT(500, 500);

    QTMR_Init(DEMO_QTMR_BASEADDR, &sQtmrConfig);

    /* Start the second channel to count */
    QTMR_EnableChannels(DEMO_QTMR_BASEADDR, 3);
}

static void DEMO_InitXBARA(void)
{
    /* Structure of initialize XBARA. */
    xbara_control_config_t xbaraConfig;

    /* Init xbara module. */
    XBARA_Init(DEMO_XBARA_BASEADDR);

    /* Configure the XBARA signal connections. */
    XBARA_SetSignalsConnection(DEMO_XBARA_BASEADDR, DEMO_XBARA_USER_CHANNEL_INPUT, DEMO_XBARA_USER_CHANNEL_OUTPUT);

    /* Configure the XBARA interrupt. */
    xbaraConfig.eActiveEdge  = kXBARA_EdgeRising;
    xbaraConfig.eRequestType = kXBARA_RequestInterruptEnable;
    XBARA_SetOutputSignalConfig(DEMO_XBARA_BASEADDR, DEMO_XBARA_USER_CHANNEL_OUTPUT, &xbaraConfig);

    /* Enable Interrupt. */
    EnableIRQWithPriority(DEMO_XBARA_IRQn, DEMO_XBARA_PRIO);
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("\r\nXBARA Peripheral Driver Example.\r\n");

    /* Init hardware peripheral */
    DEMO_InitQtmr();
    DEMO_InitXBARA();

    while (true)
    {
        if (true == xbaraIsrFlag)
        {
            PRINTF("\r\nXBARA interrupt is occurred !");
            xbaraIsrFlag = false;
        }
    }
}
