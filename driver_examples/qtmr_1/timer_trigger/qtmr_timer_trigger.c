/*
 * Copyright 2017-2020 NXP
 * Copyright 2025 NXP
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
#include "fsl_lpit.h"
#include "fsl_xbar.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile bool qtmrIsrFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void QTMR_IRQ_HANDLER(void)
{
    /* Clear interrupt flag.*/
    QTMR_ClearStatusFlags(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, kQTMR_CompareFlag);

    qtmrIsrFlag = true;
    SDK_ISR_EXIT_BARRIER;
}

void lpit_init(void)
{
    lpit_config_t lpitConfig;
    lpit_chnl_params_t lpitChannelConfig;

    LPIT_GetDefaultConfig(&lpitConfig);
    LPIT_Init(DEMO_LPIT_BASE, &lpitConfig);
    lpitChannelConfig.chainChannel          = false;
    lpitChannelConfig.enableReloadOnTrigger = false;
    lpitChannelConfig.enableStartOnTrigger  = false;
    lpitChannelConfig.enableStopOnTimeout   = false;
    lpitChannelConfig.timerMode             = kLPIT_PeriodicCounter;
    /* Set default values for the trigger source */
    lpitChannelConfig.triggerSelect = kLPIT_Trigger_TimerChn0;
    lpitChannelConfig.triggerSource = kLPIT_TriggerSource_External;

    /* Init lpit channel 0 */
    LPIT_SetupChannel(DEMO_LPIT_BASE, kLPIT_Chnl_0, &lpitChannelConfig);
    LPIT_SetTimerPeriod(DEMO_LPIT_BASE, kLPIT_Chnl_0, USEC_TO_COUNT(100U, LPIT_SOURCECLOCK));
    LPIT_EnableInterrupts(DEMO_LPIT_BASE, kLPIT_Channel0TimerInterruptEnable);
    LPIT_StartTimer(DEMO_LPIT_BASE, kLPIT_Chnl_0);
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint8_t i = 0;
    qtmr_config_t qtmrConfig;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("\r\n*********QUADTIMER EXAMPLE START*********");

    XBAR_Init(kXBAR_DSC1);
    BLK_CTRL_WAKEUPMIX->LPIT_TRIG_SEL |= BLK_CTRL_WAKEUPMIX_LPIT_TRIG_SEL_LPIT1_TRIG0_INPUT_SEL(1);
    XBAR_SetSignalsConnection(kXBAR1_InputLpit1LpitTrigOut0, kXBAR1_OutputQtimer1Tmr0Input);
    PRINTF("\r\nIPSYNC trigger signal connected! \r\n");

    lpit_init();

    /*
     * qtmrConfig.debugMode = kQTMR_RunNormalInDebug;
     * qtmrConfig.enableExternalForce = false;
     * qtmrConfig.enableMasterMode = false;
     * qtmrConfig.faultFilterCount = 0;
     * qtmrConfig.faultFilterPeriod = 0;
     * qtmrConfig.primarySource = kQTMR_ClockDivide_2;
     * qtmrConfig.secondarySource = kQTMR_Counter0InputPin;
     */
    QTMR_GetDefaultConfig(&qtmrConfig);
    qtmrConfig.primarySource = QTMR_PRIMARY_SOURCE;

    PRINTF("\r\n****Timer use-case, 50 millisecond tick.****\n");
    QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, &qtmrConfig);

    /* Set timer period to be 50 millisecond */
    QTMR_SetTimerPeriod(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, MSEC_TO_COUNT(50U, QTMR_SOURCE_CLOCK));

    /* Enable at the NVIC */
    EnableIRQ(QTMR_IRQ_ID);

    /* Enable timer compare interrupt */
    QTMR_EnableInterrupts(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, kQTMR_CompareInterruptEnable);

    /* Start the second channel to count on rising edge of the primary source clock */
    QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, kQTMR_PriSrcRiseEdge);

    for (i = 0; i < 10; i++)
    {
        /* Check whether compare interrupt occurs */
        while (!(qtmrIsrFlag))
        {
        }
        PRINTF("\r\n Timer interrupt has occurred !");
        qtmrIsrFlag = false;
    }
    QTMR_StopTimer(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL);

    PRINTF("\r\n****Chain Timer use-case, 20 second tick.****\n");

    qtmrConfig.primarySource = QTMR_PRIMARY_SOURCE;
    QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_FIRST_QTMR_CHANNEL, &qtmrConfig);

    /* Init the second channel to use output of the first channel as we are chaining the first channel and the second
     * channel */
    qtmrConfig.primarySource = QTMR_ClockCounterOutput;
    QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, &qtmrConfig);

    /* Set the first channel period to be 2 millisecond */
    QTMR_SetTimerPeriod(BOARD_QTMR_BASEADDR, BOARD_FIRST_QTMR_CHANNEL, MSEC_TO_COUNT(1U, QTMR_SOURCE_CLOCK));

    /* Set the second channel count which increases every millisecond, set compare event for 20 second */
    QTMR_SetTimerPeriod(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, 10000);

    /* Enable the second channel compare interrupt */
    QTMR_EnableInterrupts(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, kQTMR_CompareInterruptEnable);

    /* Start the second channel in cascase mode, chained to the first channel as set earlier via the primary source
     * selection */
    QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, kQTMR_CascadeCount);

    /* Start the first channel to count on rising edge of the primary source clock */
    QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_FIRST_QTMR_CHANNEL, kQTMR_SecSrcTrigPriCnt);

    for (i = 0; i < 5; i++)
    {
        /* Check whether compare interrupt occurs*/
        while (!(qtmrIsrFlag))
        {
        }
        PRINTF("\r\n Timer interrupt has occurred !");
        qtmrIsrFlag = false;
    }

    QTMR_StopTimer(BOARD_QTMR_BASEADDR, BOARD_FIRST_QTMR_CHANNEL);
    QTMR_StopTimer(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL);

    /* ERRATA050194: Overflow flag and related interrupt cannot be generated successfully in upward count mode.
     * Workaround: using compare interrupt instead of overflow interrupt by setting compare value to 0xFFFF.
     * The compare interrupt has the same timing effect as overflow interrupt in this way.
     */
    PRINTF("\r\n****Timer use-case, about 131s Over flow Test.****\n");

    qtmrConfig.primarySource = QTMR_PRIMARY_SOURCE;
    QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_FIRST_QTMR_CHANNEL, &qtmrConfig);

    /* Init the second channel to use output of the first channel as we are chaining the first channel and the second
     * channel */
    qtmrConfig.primarySource = QTMR_ClockCounterOutput;
    QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, &qtmrConfig);

    /* Set the first channel period to be 1 millisecond */
    QTMR_SetTimerPeriod(BOARD_QTMR_BASEADDR, BOARD_FIRST_QTMR_CHANNEL, MSEC_TO_COUNT(1U, QTMR_SOURCE_CLOCK));

    /* Set the second channel count which increases every millisecond, set compare event for 131 second */
    QTMR_SetTimerPeriod(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, 0xFFFF);

    /* Enable timer compare interrupt */
    QTMR_EnableInterrupts(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, kQTMR_CompareInterruptEnable);

    /* Start the second channel in cascase mode, chained to the first channel as set earlier via the primary source
     * selection */
    QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, kQTMR_CascadeCount);

    /* Start the second channel to count on rising edge of the primary source clock */
    QTMR_StartTimer(BOARD_QTMR_BASEADDR, BOARD_FIRST_QTMR_CHANNEL, kQTMR_SecSrcTrigPriCnt);

    /* Check whether compare(overflow) interrupt occurs*/
    while (!(qtmrIsrFlag))
    {
    }
    PRINTF("\r\n Timer Overflow has occurred !");
    qtmrIsrFlag = false;

    QTMR_StopTimer(BOARD_QTMR_BASEADDR, BOARD_FIRST_QTMR_CHANNEL);
    QTMR_Deinit(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL);

    PRINTF("\r\n*********QUADTIMER EXAMPLE END.*********");

    while (1)
    {
    }
}
