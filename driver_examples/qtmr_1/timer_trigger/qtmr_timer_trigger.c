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
#include "fsl_tpm.h"
#include "fsl_xbar.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DEMO_PWM_FREQUENCY (24000U)

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

void tpm_init(void)
{
    tpm_config_t tpmInfo;
    tpm_chnl_pwm_signal_param_t tpmParam;
    int updatedDutycycle = 5;
    uint8_t control;

    TPM_GetDefaultConfig(&tpmInfo);
    tpmInfo.prescale = TPM_CalculateCounterClkDiv(BOARD_TPM_BASEADDR, DEMO_PWM_FREQUENCY, TPM_SOURCE_CLOCK);
    TPM_Init(BOARD_TPM_BASEADDR, &tpmInfo);
    tpmParam.chnlNumber = (tpm_chnl_t)BOARD_TPM_CHANNEL;
    tpmParam.level            = kTPM_HighTrue;
    tpmParam.dutyCyclePercent = updatedDutycycle;
    if (kStatus_Success != TPM_SetupPwm(BOARD_TPM_BASEADDR, &tpmParam, 1U, kTPM_CenterAlignedPwm, DEMO_PWM_FREQUENCY, TPM_SOURCE_CLOCK)) {
	    PRINTF("\r\nSetup PWM fail!\r\n");
	    return;
    }
    TPM_StartTimer(BOARD_TPM_BASEADDR, kTPM_SystemClock);
    control = TPM_GetChannelContorlBits(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL);
    TPM_EnableChannel(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_TPM_CHANNEL, control);
    TPM_EnableInterrupts(BOARD_TPM_BASEADDR, kTPM_TimeOverflowInterruptEnable);
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

    PRINTF("\r\n*********QUADTIMER EXAMPLE START*********");

    XBAR_Init(kXBAR_DSC1);
    XBAR_SetSignalsConnection(kXBAR1_InputTpm6LptpmChTrigger0, kXBAR1_OutputQtimer1Tmr0Input);
    PRINTF("\r\nIPSYNC trigger signal connected! \r\n");

    tpm_init();

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

    PRINTF("\r\n****Chain Timer use-case, 10 second tick.****\n");

    qtmrConfig.primarySource = QTMR_PRIMARY_SOURCE;
    QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_FIRST_QTMR_CHANNEL, &qtmrConfig);

    /* Init the second channel to use output of the first channel as we are chaining the first channel and the second
     * channel */
    qtmrConfig.primarySource = QTMR_ClockCounterOutput;
    QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, &qtmrConfig);

    /* Set the first channel period to be 1 millisecond */
    QTMR_SetTimerPeriod(BOARD_QTMR_BASEADDR, BOARD_FIRST_QTMR_CHANNEL, MSEC_TO_COUNT(1U, QTMR_SOURCE_CLOCK));

    /* Set the second channel count which increases every millisecond, set compare event for 10 second */
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
    PRINTF("\r\n****Timer use-case, about 65.5s Over flow Test.****\n");

    qtmrConfig.primarySource = QTMR_PRIMARY_SOURCE;
    QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_FIRST_QTMR_CHANNEL, &qtmrConfig);

    /* Init the second channel to use output of the first channel as we are chaining the first channel and the second
     * channel */
    qtmrConfig.primarySource = QTMR_ClockCounterOutput;
    QTMR_Init(BOARD_QTMR_BASEADDR, BOARD_SECOND_QTMR_CHANNEL, &qtmrConfig);

    /* Set the first channel period to be 1 millisecond */
    QTMR_SetTimerPeriod(BOARD_QTMR_BASEADDR, BOARD_FIRST_QTMR_CHANNEL, MSEC_TO_COUNT(1U, QTMR_SOURCE_CLOCK));

    /* Set the second channel count which increases every millisecond, set compare event for 65.5 second */
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
