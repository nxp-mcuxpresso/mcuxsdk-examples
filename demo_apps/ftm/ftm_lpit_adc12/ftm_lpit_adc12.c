/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_lpit.h"
#include "fsl_adc12.h"
#include "fsl_ftm.h"
#include "fsl_trgmux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Initialize the ADC12
 */
static void DEMO_Init_ADC(void);

/*!
 * @brief Initialize the LPIT
 */
static void DEMO_Init_LPIT(void);

/*!
 * @brief Initialize the FTM
 */
static void DEMO_Init_FTM(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_adc12ConvValue;
volatile uint32_t g_adc12InterruptCounter;
volatile bool g_adc12InterruptFlag;
const uint32_t g_Adc12_8bitFullRange = 256U;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief ISR for ADC12 interrupt function
 */
void DEMO_ADC_IRQ_HANDLER(void)
{
    if (0U !=
        (kADC12_ChannelConversionCompletedFlag & ADC12_GetChannelStatusFlags(DEMO_ADC_BASE, DEMO_ADC_CHANNEL_GROUP)))
    {
        /* Read to clear COCO flag. */
        g_adc12ConvValue = ADC12_GetChannelConversionValue(DEMO_ADC_BASE, DEMO_ADC_CHANNEL_GROUP);
        g_adc12InterruptCounter++;
    }
    g_adc12InterruptFlag = true;
    FTM_StopTimer(DEMO_FTM_BASE); /* Stop the FTM counter. */
    SDK_ISR_EXIT_BARRIER;
}

/*
 * Configure the two ADC instance with two hardware trigger command for each instance.
 */
void static DEMO_Init_ADC(void)
{
    adc12_config_t adc12ConfigStruct;
    adc12_channel_config_t adc12ChannelConfigStruct;

    /* Initialize ADC12. */
    ADC12_GetDefaultConfig(&adc12ConfigStruct);
    ADC12_Init(DEMO_ADC_BASE, &adc12ConfigStruct);
    /* Set to hardware trigger mode. */
    ADC12_EnableHardwareTrigger(DEMO_ADC_BASE, true);

    /* Calibrate ADC12. */
    if (kStatus_Success != ADC12_DoAutoCalibration(DEMO_ADC_BASE))
    {
        PRINTF("ADC calibration failed!\r\n");
    }

    /* Configure the ADC12 conversion channel with interrupt when the conversion is done. */
    adc12ChannelConfigStruct.enableInterruptOnConversionCompleted = true;
    adc12ChannelConfigStruct.channelNumber                        = DEMO_ADC_USER_CHANNEL;
    ADC12_SetChannelConfig(DEMO_ADC_BASE, DEMO_ADC_CHANNEL_GROUP, &adc12ChannelConfigStruct);

    /* Configure the NVIC to accept the interrupt from ADC12 module. */
    EnableIRQ(DEMO_ADC_IRQ_ID);
}

static void DEMO_Init_LPIT(void)
{
    lpit_config_t lpitConfigStruct;
    lpit_chnl_params_t lpitChannelStruct;

    /* Initialize LPIT module. */
    LPIT_GetDefaultConfig(&lpitConfigStruct);
    LPIT_Init(DEMO_LPIT_BASE, &lpitConfigStruct);

    LPIT_GetDefaultConfig(&lpitConfigStruct);
    lpitChannelStruct.chainChannel          = false;
    lpitChannelStruct.enableReloadOnTrigger = false;
    lpitChannelStruct.enableStartOnTrigger  = true;
    lpitChannelStruct.enableStopOnTimeout   = true;
    lpitChannelStruct.timerMode             = kLPIT_PeriodicCounter;
    lpitChannelStruct.triggerSelect         = kLPIT_Trigger_TimerChn0;
    lpitChannelStruct.triggerSource         = kLPIT_TriggerSource_External;

    LPIT_SetupChannel(DEMO_LPIT_BASE, kLPIT_Chnl_0, &lpitChannelStruct);
    /* Init lpit channel 0 */
    LPIT_SetTimerPeriod(DEMO_LPIT_BASE, kLPIT_Chnl_0, USEC_TO_COUNT(5U, LPIT_SOURCECLOCK));
    LPIT_StartTimer(DEMO_LPIT_BASE, kLPIT_Chnl_0);
}

static void DEMO_Init_FTM(void)
{
    ftm_config_t ftmConfigStruct;
    ftm_chnl_pwm_signal_param_t ftmParam;
    ftm_pwm_level_select_t pwmLevel = kFTM_LowTrue;

    /* Initialize FTM module. */
    FTM_GetDefaultConfig(&ftmConfigStruct);
    ftmConfigStruct.extTriggers = kFTM_Chnl0Trigger; /* Enable to output the trigger. */
    FTM_Init(DEMO_FTM_BASE, &ftmConfigStruct);

    /* Configure ftm params with frequency 24kHz */
    ftmParam.chnlNumber            = kFTM_Chnl_0;
    ftmParam.level                 = pwmLevel;
    ftmParam.dutyCyclePercent      = 50U; /* Percent: 0 - 100. */
    ftmParam.firstEdgeDelayPercent = 0U;
    ftmParam.enableComplementary   = false;
    ftmParam.enableDeadtime        = false;
    FTM_SetupPwm(DEMO_FTM_BASE, &ftmParam, 1U, kFTM_CenterAlignedPwm, DEMO_FTM_PWM_HZ, DEMO_FTM_COUNTER_CLOCK_HZ);
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("\r\nftm_lpit_adc12 demo.\r\n");

    g_adc12InterruptCounter = 0U;
    g_adc12InterruptFlag    = false;

    /* Initialize the hardware modules. */
    DEMO_Init_ADC();
    DEMO_Init_LPIT();
    DEMO_Init_FTM();

    PRINTF("ADC Full Range: %d\r\n", g_Adc12_8bitFullRange);
    PRINTF("\r\nInput any key to trigger the ADC conversion.\r\n");
    while (1)
    {
        GETCHAR();
        /*
         * Reset FTM counter value and start the FTM counter, finally trigger the ADC12's conversion.
         * FTM_StartTimer() -> lpit channel 0 PreTrigger -> ADC conversion done interrupt -> FTM_StopTimer().
         */
        FTM_ClearQuadDecoderCounterValue(DEMO_FTM_BASE);
        FTM_StartTimer(DEMO_FTM_BASE, DEMO_FTM_COUNTER_CLOCK_SOURCE);
        while (false == g_adc12InterruptFlag)
        {
        }
        g_adc12InterruptFlag = false;

        PRINTF("g_adc12InterruptCounter: %d\r\n", g_adc12InterruptCounter);
    }
}
