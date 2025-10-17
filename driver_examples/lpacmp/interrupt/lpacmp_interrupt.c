/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_lpacmp.h"
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
volatile bool matched = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*! @brief LPACMP IRQ handler */
void DEMO_LPACMP_IRQ_HANDLER_FUNC(void)
{
    LPACMP_EnableInterrupt(DEMO_LPACMP_BASE, DEMO_LPACMP_CHANNEL, false);
    LPACMP_EnableComparator(DEMO_LPACMP_BASE, false);
    LPACMP_ClearInterruptStatusFlags(DEMO_LPACMP_BASE, DEMO_LPACMP_CHANNEL);
    matched = true;
}

/*! @brief Main function */
int main(void)
{
    lpacmp_config_t config;
    lpacmp_channel_config_t chnlConfig;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    chnlConfig.enableWakeupToSmm = false;
    chnlConfig.enableTriggerOutput = false;
    chnlConfig.enableInterrupt = false;
    chnlConfig.comparePolarityHigher = true;
    chnlConfig.channelIndex = DEMO_LPACMP_CHANNEL;
    chnlConfig.triggerOutputWidth = 0U;
    chnlConfig.delay = DEMO_LPACMP_DELAY;
    chnlConfig.positiveInput = DEMO_LPACMP_POSITIVE_PORT;
    chnlConfig.negativeInput = DEMO_LPACMP_NEGATIVE_PORT;

    LPACMP_GetDefaultConfig(&config);
    config.mode = DEMO_LPACMP_MODE;
    config.intervalTime = DEMO_LPACMP_INTERVAL_TIME;
    config.channelConfig = &chnlConfig;
    config.channelCount  = 1U;
    LPACMP_Init(DEMO_LPACMP_BASE, &config);

    PRINTF("\r\nLpacmp interrupt example");

    while (1)
    {
        PRINTF("\r\nPress any key to start the comparsion");
        GETCHAR();

        LPACMP_EnableComparator(DEMO_LPACMP_BASE, true);
        
        /* When the voltage of the negative port comes from the internal, a delay
         * of 56us is required, because the internal voltage comes from the internal
         * DAC, and the establishment of the DAC takes 56us.
         */
        if(chnlConfig.negativeInput <= kLPACMP_Negative_Input_14Avdd_Divide_15)
        {
            SDK_DelayAtLeastUs(56U, DEMO_SYSTEM_CLOCK);
            LPACMP_ClearInterruptStatusFlags(DEMO_LPACMP_BASE, DEMO_LPACMP_CHANNEL);
        }

        LPACMP_EnableInterrupt(DEMO_LPACMP_BASE, DEMO_LPACMP_CHANNEL, true);
        EnableIRQ(DEMO_LPACMP_IRQ);

        while (!matched)
        {
        }

        PRINTF("\r\nLPACMP positive port voltage is higher than the negative port voltage");

        matched = false;
    }
}
