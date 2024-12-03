/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017, 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_cmp.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    cmp_config_t sCmpConfig;

    /* Initialize hardware */
    BOARD_InitHardware();

    PRINTF("\r\nCMP polling Example.\r\n");
    PRINTF("\r\nPlease connect CMPA_IN0 to VDD firstly and then GND\r\n");

    /*
     *   config->eHysteresisLevel                       = kCMP_HysteresisLevel0;
     *   config->eComparasionSpeedMode                  = kCMP_ComparsionModeHighSpeed;
     *   config->eWorkMode                              = kCMP_WorkModeWindowBypassAndNoExternalSample;
     *   config->ePlusInput                             = kCMP_InputMux0;
     *   config->eMinusInput                            = kCMP_InputMux1;
     *   config->sDacConfig.eDACVrefSource              = kCMP_DACVrefSourceVin1;
     *   config->sDacConfig.u8DACOutputVoltageDivider   = 255;
     *   config->bInvertComparatorOutputPolarity        = false;
     *   config->eFilterCount                           = kCMP_FilterCountDisable;
     *   config->u8FilterPeriod                         = 0U;
     *   config->eOutputSource                          = kCMP_OutputSourceFromFilterCOUT;
     *   config->bEnableOutputPin                       = true;
     *   config->uDmaInterruptConfig                    = kCMP_InterrruptRequestEnableAll;
     *   config->bCMPEnable                             = false;
     */
    CMP_GetDefaultConfig(&sCmpConfig);
    sCmpConfig.sDacConfig.eDACVrefSource = kCMP_DACVrefSourceVin2; /* VCC. */
    sCmpConfig.sDacConfig.u8DACOutputVoltageDivider =
        ((CMP_DACCR_VOSEL_MASK >> CMP_DACCR_VOSEL_SHIFT) >> 1U); /* Half voltage of logic high level. */
    sCmpConfig.sDacConfig.bEnableInternalDAC = true;             /* enable internal DAC. */

    sCmpConfig.ePlusInput  = DEMO_CMP_USER_CHANNEL;
    sCmpConfig.eMinusInput = DEMO_CMP_DAC_CHANNEL;
    sCmpConfig.bCMPEnable  = true;
    /* Init the CMP comparator. */
    CMP_Init(DEMO_CMP_BASE, &sCmpConfig);

    /* Configure the LED. */
    LED_INIT();

    while (1)
    {
        if (kCMP_OutputFlagRisingEdge == (kCMP_OutputFlagRisingEdge & CMP_GetStatusFlags(DEMO_CMP_BASE)))
        {
            CMP_ClearStatusFlags(DEMO_CMP_BASE, kCMP_OutputFlagRisingEdge);
            LED_ON(); /* Turn on led. */
        }

        if (kCMP_OutputFlagFallingEdge == (kCMP_OutputFlagFallingEdge & CMP_GetStatusFlags(DEMO_CMP_BASE)))
        {
            CMP_ClearStatusFlags(DEMO_CMP_BASE, kCMP_OutputFlagFallingEdge);
            LED_OFF(); /* Turn off led. */
        }
    }
}
