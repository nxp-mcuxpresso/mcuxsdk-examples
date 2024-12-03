/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_tempsensor.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
bool lowThrsholdIntFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void DEMO_TMPSNS_LOW_IRQ_HANDLER_FUNC(void)
{
    if (0 != (TMPSNS_NonSecure_GetTmpsnsStatus(DEMO_TMPSNS_BASE) & kTMPSNS_NonSecure_Threshold2IntStatus))
    {
        lowThrsholdIntFlag = true; 
        TMPSNS_NonSecure_DisableTmpsnsInt(DEMO_TMPSNS_BASE, kTMPSNS_NonSecure_Threshold2CmpIntEnable);
        __ISB();
        __DSB();
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    tmpsns_config_t tmpsnsConfig;
    tmpsns_threshold_config_t tmpsnsThresholdConfig;

    /* Initialize hardware. */
    BOARD_InitHardware();

    PRINTF("\r\n Tempsensor example.");

    /* Configure the tempsensor. */
    TMPSNS_GetDefaultConfig(&tmpsnsConfig);
    tmpsnsConfig.measurementMode = kTMPSNS_ContinuousMode;
    TMPSNS_Init(DEMO_TMPSNS_BASE, &tmpsnsConfig);
    TMPSNS_Secure_SetTrimVal(DEMO_TMPSNS_BASE, 0xA57A, 0xBA0D, 0x6D48, 0);

    /* Enable the tempsensor. */
    TMPSNS_EnableTmpsns(DEMO_TMPSNS_BASE);

    /* Configure the threshold comparator 2 as the low-temperature threshold. */
    tmpsnsThresholdConfig.thresholdIndex   = kTMPSNS_ThresholdCmp2;
    tmpsnsThresholdConfig.thresholdCmpMode = kTMPSNS_ThresholdCmpMode0;
    tmpsnsThresholdConfig.thresholdVal     = DEMO_TMPSNS_LOW_TEMPERATURE;
    TMPSNS_NonSecure_DoThresholdConfig(DEMO_TMPSNS_BASE, &tmpsnsThresholdConfig);

    /* Enable interrupts. */
    TMPSNS_NonSecure_EnableTmpsnsInt(DEMO_TMPSNS_BASE, kTMPSNS_NonSecure_Threshold2CmpIntEnable);
    EnableIRQ(DEMO_TMPSNS_LOW_IRQn);

    /* Start the measurement. */
    TMPSNS_StartMeasurement(DEMO_TMPSNS_BASE);

    PRINTF("\r\n Please press any key to get the current temperature.");

    while (1)
    {
        GETCHAR();
        PRINTF("\r\n The current temperature is %.3f degrees Celsius.",
               TMPSNS_NonSecure_GetCurrentTemperature(DEMO_TMPSNS_BASE));

        if (lowThrsholdIntFlag)
        {
            PRINTF("\r\n The current temperature is lower than the pre-setting threshold.");
            break;
        }
    }

    while (1)
    {
    }
}
