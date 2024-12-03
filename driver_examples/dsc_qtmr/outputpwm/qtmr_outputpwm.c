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
uint16_t g_u16Cmpld1Value    = 0U;
uint16_t g_u16Cmpld2Value    = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/
void QTMR_IRQ_HANDLER(void)
{
    /* Disable interrupt.*/
    QTMR_DisableInterrupts(BOARD_QTMR_BASEADDR, BOARD_QTMR_CHANNEL, (uint16_t)kQTMR_Compare2InterruptEnable);
    /* Clear interrupt flag.*/
    QTMR_ClearStatusFlags(BOARD_QTMR_BASEADDR, BOARD_QTMR_CHANNEL, (uint16_t)kQTMR_Compare2Flag);
    /* Update preload register value.*/
    QTMR_SetCompare1PreloadValue(BOARD_QTMR_BASEADDR, BOARD_QTMR_CHANNEL, g_u16Cmpld1Value);
    QTMR_SetCompare2PreloadValue(BOARD_QTMR_BASEADDR, BOARD_QTMR_CHANNEL, g_u16Cmpld2Value);

    g_bQtmrIsrFlag = true;
}

static void QTMR_GetCmpldValue(uint32_t u32PwmFreqHz, uint8_t u8DutyCycle, uint32_t u32SrcFreqHz)
{
    assert(u32SrcFreqHz > u32PwmFreqHz);
    assert(u8DutyCycle < 100U);

    uint32_t u32PeriodCount, u32HighCount, u32LowCount;
    u32PeriodCount = (u32SrcFreqHz / u32PwmFreqHz);
    u32HighCount   = (u32PeriodCount * u8DutyCycle) / 100U;
    u32LowCount    = u32PeriodCount - u32HighCount;

    if (u32HighCount > 0U)
    {
        u32HighCount -= 1U;
    }
    if (u32LowCount > 0U)
    {
        u32LowCount -= 1U;
    }

    /* This should not be a 16-bit overflow value. If it is, change to a larger divider for clock source. */
    assert(u32HighCount <= 0xFFFFU);
    assert(u32LowCount <= 0xFFFFU);

    g_u16Cmpld1Value = (uint16_t)u32LowCount;
    g_u16Cmpld2Value = (uint16_t)u32HighCount;
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint8_t i = 0;
    qtmr_channel_config_t sChannelConfig;
    qtmr_config_t sQtmrConfig = {0};
    uint8_t u8DutyCycle       = 50U;
    uint8_t u8GetCharValue    = 0U;

    /* Fill QTMR configure structure */
    sQtmrConfig.psChannelConfig[BOARD_QTMR_CHANNEL] = &sChannelConfig;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    PRINTF("\r\n****Output PWM example.****\n");
    PRINTF("\r\n*********Make sure to connect an oscilloscope.*********\n");
    PRINTF("\r\n****A 50%% duty cycle PWM wave is observed on an oscilloscope.****\n");

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
    QTMR_GetChannelDefaultConfig(&sChannelConfig);
    /* Setup the primary count source */
    sChannelConfig.sInputConfig.ePrimarySource = QTMR_PRIMARY_SOURCE;
    /* Setup count length to until compare */
    sChannelConfig.sCountConfig.eCountLength = kQTMR_CountLengthUntilCompare;
    /* Setup compare registers preload mode */
    sChannelConfig.sCountConfig.eCountPreload1 = kQTMR_CountPreloadOnComp2CompareEvent;
    sChannelConfig.sCountConfig.eCountPreload2 = kQTMR_CountPreloadOnComp1CompareEvent;
    /* Setup output toggle using alternating compare registers */
    sChannelConfig.sOutputConfig.eOutputMode = kQTMR_OutputToggleOnAltCompareReg;
    /* Setup output PWM initial value */
    sChannelConfig.sOutputConfig.eOutputValueOnForce  = kQTMR_OutputValueSetOnForce;
    sChannelConfig.sOutputConfig.bEnableSwForceOutput = true;
    sChannelConfig.sOutputConfig.bEnableOutputPin     = true;

    /* Generate a PWM signal with 50% dutycycle by default */
    QTMR_GetCmpldValue(QTMR_PWM_OUTPUT_FREQUENCY, 50U, QTMR_SOURCE_CLOCK);
    sChannelConfig.u16Comp1Preload = g_u16Cmpld1Value;
    sChannelConfig.u16Comp2Preload = g_u16Cmpld2Value;

    QTMR_Init(BOARD_QTMR_BASEADDR, &sQtmrConfig);

    /* Enable interrupt with priority */
    EnableIRQWithPriority(QTMR_IRQ_ID, QTMR_IRQ_PRIO);

    /* Start the second channel to count */
    QTMR_EnableChannels(BOARD_QTMR_BASEADDR, (uint16_t)(1UL << (uint16_t)BOARD_QTMR_CHANNEL));

    while (1)
    {
        do
        {
            PRINTF("\r\nPlease enter a value to update the Duty cycle:\r\n");
            PRINTF("Note: The range of value is 1 to 9.\r\n");
            PRINTF("For example: If enter '5', the duty cycle will be set to 50 percent.\r\n");
            PRINTF("Value:");
            u8GetCharValue = GETCHAR() - 0x30U;
            PRINTF("%d", u8GetCharValue);
            PRINTF("\r\n");
        } while ((u8GetCharValue > 9U) || (u8GetCharValue == 0U));

        u8DutyCycle = u8GetCharValue * 10U;
        /* According to the dutycycle to calculate the new preload comapre register value */
        QTMR_GetCmpldValue(QTMR_PWM_OUTPUT_FREQUENCY, u8DutyCycle, QTMR_SOURCE_CLOCK);
        QTMR_EnableInterrupts(BOARD_QTMR_BASEADDR, BOARD_QTMR_CHANNEL, (uint16_t)kQTMR_Compare2InterruptEnable);
        /* Check whether compare 2 interrupt occurs */
        while (!(g_bQtmrIsrFlag))
        {
        }
        g_bQtmrIsrFlag = false;
        PRINTF("The duty cycle was successfully updated!\r\n");
    }
}
