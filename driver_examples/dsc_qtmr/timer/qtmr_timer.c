/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

#include "fsl_qtmr.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef QTMR_IRQ_PRIO
#define QTMR_IRQ_PRIO 0
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_bQtmrIsrFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
#pragma interrupt alignsp saveall
void QTMR_IRQ_HANDLER(void);
void QTMR_IRQ_HANDLER(void)
{
    /* Clear interrupt flag.*/
    QTMR_ClearStatusFlags(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, (uint16_t)kQTMR_CompareFlag);

    g_bQtmrIsrFlag = true;
}
#pragma interrupt off

/*!
 * @brief Main function
 */
int main(void)
{
    uint8_t i = 0;
    qtmr_channel_config_t sFirstChannelConfig;
    qtmr_channel_config_t sSecondChannelConfig;
    qtmr_config_t sQtmrConfig = {0};

    sQtmrConfig.psChannelConfig[BOARD_FIRST_QTMR_CHANNEL]  = &sFirstChannelConfig;
    sQtmrConfig.psChannelConfig[BOARD_SECOND_QTMR_CHANNEL] = &sSecondChannelConfig;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    PRINTF("\r\n*********QUADTIMER EXAMPLE START*********");

    /*
     * psConfig->sInputConfig.ePrimarySource = kQTMR_PrimarySrcIPBusClockDivide2;
     * psConfig->sCountConfig.eCountMode = kQTMR_CountPrimarySrcRiseEdge;
     * psConfig->sCountConfig.eCountLength = kQTMR_CountLengthUntilRollOver;
     * psConfig->sCountConfig.eCountDir = kQTMR_CountDirectionUp;
     * psConfig->sCountConfig.eCountTimes = kQTMR_CountTimesRepeat;
     * psConfig->sCountConfig.eCountLoadMode = kQTMR_CountLoadNormal;
     * psConfig->sOutputConfig.eOutputMode = kQTMR_OutputAssertWhenCountActive;
     * psConfig->sCooperationConfig.bEnableMasterReInit = false;
     * psConfig->sCooperationConfig.bEnableMasterForceOFLAG = false;
     * psConfig->sCooperationConfig.bEnableMasterMode = false;
     * psConfig->eDebugMode = kQTMR_DebugRunNormal;
     * psConfig->u16Comp1 = 0x0U;
     * psConfig->u16EnabledInterruptMask = 0x0U;
     * psConfig->bEnableChannel = false;
     */
    QTMR_GetChannelDefaultConfig(&sSecondChannelConfig);
    /* Setup the primary count source*/
    sSecondChannelConfig.sInputConfig.ePrimarySource = QTMR_PRIMARY_SOURCE;
    /* Setup count length to until compare*/
    sSecondChannelConfig.sCountConfig.eCountLength = kQTMR_CountLengthUntilCompare;
    /* Enable channel compare interrupt */
    sSecondChannelConfig.u16EnabledInterruptMask = kQTMR_CompareInterruptEnable;
    /* Set the first channel period to be 50 millisecond */
    sSecondChannelConfig.u16Comp1 = MSEC_TO_COUNT(50U, QTMR_SOURCE_CLOCK);

    PRINTF("\r\n****Timer use-case, 50 millisecond tick.****\n");
    QTMR_Init(BOARD_QTMR_BASEADDR, &sQtmrConfig);

    /* Enable interrupt with priority */
    EnableIRQWithPriority(QTMR_IRQ_ID, QTMR_IRQ_PRIO);

    /* Start the second channel to count */
    QTMR_EnableChannels(BOARD_QTMR_BASEADDR, (uint16_t)(1UL << (uint16_t)BOARD_SECOND_QTMR_CHANNEL));

    for (i = 0; i < 10; i++)
    {
        /* Check whether compare interrupt occurs */
        while (!(g_bQtmrIsrFlag))
        {
        }
        PRINTF("\r\n Timer interrupt has occurred !");
        g_bQtmrIsrFlag = false;
    }
    QTMR_Deinit(BOARD_QTMR_BASEADDR);

    PRINTF("\r\n****Chain Timer use-case, 10 second tick.****\n");

    QTMR_GetChannelDefaultConfig(&sFirstChannelConfig);
    QTMR_GetChannelDefaultConfig(&sSecondChannelConfig);
    sFirstChannelConfig.sInputConfig.ePrimarySource = QTMR_PRIMARY_SOURCE;
    /* Setup count length to until compare*/
    sFirstChannelConfig.sCountConfig.eCountLength = kQTMR_CountLengthUntilCompare;
    /* Set the first channel period to be 1 millisecond */
    sFirstChannelConfig.u16Comp1 = MSEC_TO_COUNT(1U, QTMR_SOURCE_CLOCK);

    /* Init the second channel to use output of the first channel as we are chaining the first channel and the second
     * channel */
    sSecondChannelConfig.sInputConfig.ePrimarySource = QTMR_ClockCounterOutput;
    /* Setup the second channel in cascase mode, chained to the first channel as set earlier via the primary source
     * selection */
    sSecondChannelConfig.sCountConfig.eCountMode = kQTMR_CountCascadeWithOtherChannel;
    /* Setup count length to until compare*/
    sSecondChannelConfig.sCountConfig.eCountLength = kQTMR_CountLengthUntilCompare;
    /* Enable channel compare interrupt */
    sSecondChannelConfig.u16EnabledInterruptMask = kQTMR_CompareInterruptEnable;
    /* Set the second channel count which increases every millisecond, set compare event for 10 second */
    sSecondChannelConfig.u16Comp1 = 10000;

    QTMR_Init(BOARD_QTMR_BASEADDR, &sQtmrConfig);

    /* Start the channels to count */
    QTMR_EnableChannels(BOARD_QTMR_BASEADDR, (uint16_t)((1UL << (uint16_t)BOARD_SECOND_QTMR_CHANNEL) |
                                                        (1UL << (uint16_t)BOARD_FIRST_QTMR_CHANNEL)));

    for (i = 0; i < 5; i++)
    {
        /* Check whether compare interrupt occurs*/
        while (!(g_bQtmrIsrFlag))
        {
        }
        PRINTF("\r\n Timer interrupt has occurred !");
        g_bQtmrIsrFlag = false;
    }

    QTMR_Deinit(BOARD_QTMR_BASEADDR);

    /* ERRATA050194: Overflow flag and related interrupt cannot be generated successfully in upward count mode.
     * Workaround: using compare interrupt instead of overflow interrupt by setting compare value to 0xFFFF.
     * The compare interrupt has the same timing effect as overflow interrupt in this way.
     */
    PRINTF("\r\n****Timer use-case, about 65.5s Over flow Test.****\n");

    /* Set the second channel count which increases every millisecond, set compare event for 65.5 second */
    sSecondChannelConfig.u16Comp1 = 0xFFFF;
    QTMR_Init(BOARD_QTMR_BASEADDR, &sQtmrConfig);

    /* Start the channels to count */
    QTMR_EnableChannels(BOARD_QTMR_BASEADDR, (uint16_t)((1UL << (uint16_t)BOARD_SECOND_QTMR_CHANNEL) |
                                                        (1UL << (uint16_t)BOARD_FIRST_QTMR_CHANNEL)));

    /* Check whether compare(overflow) interrupt occurs*/
    while (!(g_bQtmrIsrFlag))
    {
    }
    PRINTF("\r\n Timer Overflow has occurred !");
    g_bQtmrIsrFlag = false;

    QTMR_Deinit(BOARD_QTMR_BASEADDR);

    PRINTF("\r\n*********QUADTIMER EXAMPLE END.*********");

    while (1)
    {
    }
}
