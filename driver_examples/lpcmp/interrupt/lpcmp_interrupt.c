/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_lpcmp.h"

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
    lpcmp_config_t mLpcmpConfigStruct;
    lpcmp_dac_config_t mLpcmpDacConfigStruct;

    /* Initialize hardware. */
    BOARD_InitHardware();

    PRINTF("LPCMP Interrupt Example.\r\n");

    /*
     *   k_LpcmpConfigStruct->enableStopMode      = false;
     *   k_LpcmpConfigStruct->enableOutputPin     = false;
     *   k_LpcmpConfigStruct->useUnfilteredOutput = false;
     *   k_LpcmpConfigStruct->enableInvertOutput  = false;
     *   k_LpcmpConfigStruct->hysteresisMode      = kLPCMP_HysteresisLevel0;
     *   k_LpcmpConfigStruct->powerMode           = kLPCMP_LowSpeedPowerMode;
     *   k_LpcmpConfigStruct->functionalSourceClock = kLPCMP_FunctionalClockSource0;
     */
    LPCMP_GetDefaultConfig(&mLpcmpConfigStruct);
    /* Init the LPCMP module. */
    LPCMP_Init(DEMO_LPCMP_BASE, &mLpcmpConfigStruct);

    /* Configure the internal DAC to output half of reference voltage. */
    mLpcmpDacConfigStruct.enableLowPowerMode = false;
#ifdef DEMO_LPCMP_REFERENCE
    mLpcmpDacConfigStruct.referenceVoltageSource = DEMO_LPCMP_REFERENCE;
#else
    mLpcmpDacConfigStruct.referenceVoltageSource = kLPCMP_VrefSourceVin2;
#endif
    mLpcmpDacConfigStruct.DACValue =
        ((LPCMP_DCR_DAC_DATA_MASK >> LPCMP_DCR_DAC_DATA_SHIFT) >> 1U); /* Half of reference voltage. */
    LPCMP_SetDACConfig(DEMO_LPCMP_BASE, &mLpcmpDacConfigStruct);

    /* Configure LPCMP input channels. */
    LPCMP_SetInputChannels(DEMO_LPCMP_BASE, DEMO_LPCMP_USER_CHANNEL, DEMO_LPCMP_DAC_CHANNEL);

    /* Init the LED. */
    LED_INIT();

    /* Enable the interrupt. */
    EnableIRQ(DEMO_LPCMP_IRQ_ID);
    LPCMP_EnableInterrupts(DEMO_LPCMP_BASE, kLPCMP_OutputRisingInterruptEnable | kLPCMP_OutputFallingInterruptEnable);

    while (1)
    {
    }
}

/*!
 * @brief ISR for LPCMP interrupt function.
 */
void DEMO_LPCMP_IRQ_HANDLER_FUNC(void)
{
    LPCMP_ClearStatusFlags(DEMO_LPCMP_BASE, kLPCMP_OutputRisingEventFlag | kLPCMP_OutputFallingEventFlag);
    if (kLPCMP_OutputAssertEventFlag == (kLPCMP_OutputAssertEventFlag & LPCMP_GetStatusFlags(DEMO_LPCMP_BASE)))
    {
        LED_ON(); /* Turn on the led. */
    }
    else
    {
        LED_OFF(); /* Turn off the led. */
    }
    SDK_ISR_EXIT_BARRIER;
}
