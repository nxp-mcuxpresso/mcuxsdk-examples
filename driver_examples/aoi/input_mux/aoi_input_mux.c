/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "app.h"
#include "board.h"
#include "fsl_aoi.h"
#include "fsl_cmp.h"
#include "fsl_sctimer.h"
#include "fsl_debug_console.h"
/*
 * This example use the  AOI peripheral to combine
 * from CMP and SCTIMER.
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Initialize the CMP.
 *
 */
static void CMP_Configuration(void);
/*!
 * @brief Initialize the SCTIMER.
 *
 */
static void SCTIMER_configuration(void);
/*!
 * @brief Initialize the AOI.
 *
 */
static void AOI_Configuration(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    /* Init board hardware */
    BOARD_InitHardware();
    /* Init SCTIMER */
    SCTIMER_configuration();
    /* Init CMP */
    CMP_Configuration();
    /* Init INPUTMUX */
    Inputmux_Configuration();
    /* Init AOI */
    AOI_Configuration();

    PRINTF("\r\n aoi_input_mux project.\r\n");

    while (1)
    {
    }
}

/*
 * Function description:
 * This function is initialize the SCTIMER module
 */
static void SCTIMER_configuration(void)
{
    sctimer_config_t sctimerInfo;
    sctimer_pwm_signal_param_t pwmParam;
    uint32_t event;
    uint32_t sctimerClock;

    sctimerClock = SCTIMER_CLK_FREQ;

    SCTIMER_GetDefaultConfig(&sctimerInfo);

    /* Initialize SCTimer module */
    SCTIMER_Init(SCT0, &sctimerInfo);

    /* Configure first PWM with frequency 24kHZ from first output */
    pwmParam.output           = DEMO_FIRST_SCTIMER_OUT;
    pwmParam.level            = kSCTIMER_HighTrue;
    pwmParam.dutyCyclePercent = 50;

    if (SCTIMER_SetupPwm(SCT0, &pwmParam, kSCTIMER_CenterAlignedPwm, 24000U, sctimerClock, &event) == kStatus_Fail)
    {
        return;
    }

    /* Start the 32-bit unify timer */
    SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_U);
}

/*
 * Function description:
 * This function is initialize the CMP module
 */
static void CMP_Configuration(void)
{
    cmp_config_t mCmpConfigStruct;
    cmp_vref_config_t mCmpVrefConfigStruct;
    /*
     * config->enableHysteresis    = true;
     * config->enableLowPower      = true;
     * config->filterClockDivider  = kCMP_FilterClockDivide1;
     * config->filterSampleMode    = kCMP_FilterSampleMode0;
     */
    CMP_GetDefaultConfig(&mCmpConfigStruct);
    /* Init CMP module. */
    CMP_Init(&mCmpConfigStruct);

    /* Set VREF source. */
    mCmpVrefConfigStruct.vrefSource = KCMP_VREFSourceVDDA;
    mCmpVrefConfigStruct.vrefValue  = 15U; /* Select half of the reference voltage. */
    CMP_SetVREF(&mCmpVrefConfigStruct);

    /* Set P-side and N-side input channels. */
    CMP_SetInputChannels(DEMO_CMP_P_CHANNEL, DEMO_CMP_N_CHANNEL);
}

/*
 * Function description:
 * This function is initialize the AOI module
 */
static void AOI_Configuration(void)
{
    aoi_event_config_t aoiEventLogicStruct;

    /* Configure the AOI event */
    aoiEventLogicStruct.PT0AC = kAOI_InputSignal;    /*SCTIMER output*/
    aoiEventLogicStruct.PT0BC = kAOI_InvInputSignal; /*CMP output*/
    aoiEventLogicStruct.PT0CC = kAOI_LogicOne;
    aoiEventLogicStruct.PT0DC = kAOI_LogicOne;

    aoiEventLogicStruct.PT1AC = kAOI_LogicZero;
    aoiEventLogicStruct.PT1BC = kAOI_LogicOne;
    aoiEventLogicStruct.PT1CC = kAOI_LogicOne;
    aoiEventLogicStruct.PT1DC = kAOI_LogicOne;

    aoiEventLogicStruct.PT2AC = kAOI_LogicZero;
    aoiEventLogicStruct.PT2BC = kAOI_LogicOne;
    aoiEventLogicStruct.PT2CC = kAOI_LogicOne;
    aoiEventLogicStruct.PT2DC = kAOI_LogicOne;

    aoiEventLogicStruct.PT3AC = kAOI_LogicZero;
    aoiEventLogicStruct.PT3BC = kAOI_LogicOne;
    aoiEventLogicStruct.PT3CC = kAOI_LogicOne;
    aoiEventLogicStruct.PT3DC = kAOI_LogicOne;

    /* Init AOI module. */
    AOI_Init(DEMO_AOI_BASEADDR);
    AOI_SetEventLogicConfig(DEMO_AOI_BASEADDR, kAOI_Event0, &aoiEventLogicStruct);
}
