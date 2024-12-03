/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_hscmp.h"

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
    hscmp_config_t mHscmpConfigStruct;
    hscmp_dac_config_t mHscmpDacConfigStruct;

    /* Initialize hardware. */
    BOARD_InitHardware();

    PRINTF("HSCMP Interrupt Example.\r\n");

    /*
     *   k_HscmpConfigStruct->enableStopMode      = false;
     *   k_HscmpConfigStruct->enableOutputPin     = false;
     *   k_HscmpConfigStruct->useUnfilteredOutput = false;
     *   k_HscmpConfigStruct->enableInvertOutput  = false;
     *   k_HscmpConfigStruct->hysteresisMode      = kHSCMP_HysteresisLevel0;
     *   k_HscmpConfigStruct->powerMode           = kHSCMP_LowSpeedPowerMode;
     */
    HSCMP_GetDefaultConfig(&mHscmpConfigStruct);
    /* Init the HSCMP module. */
    HSCMP_Init(DEMO_HSCMP_BASE, &mHscmpConfigStruct);

    /* Configure the internal DAC to output half of reference voltage. */
    mHscmpDacConfigStruct.enableLowPowerMode     = false;
    mHscmpDacConfigStruct.referenceVoltageSource = kHSCMP_VrefSourceVin2;
    mHscmpDacConfigStruct.DACValue =
        ((HSCMP_DCR_DAC_DATA_MASK >> HSCMP_DCR_DAC_DATA_SHIFT) >> 1U); /* Half of reference voltage. */
    HSCMP_SetDACConfig(DEMO_HSCMP_BASE, &mHscmpDacConfigStruct);

    /* Configure HSCMP input channels. */
    HSCMP_SetInputChannels(DEMO_HSCMP_BASE, DEMO_HSCMP_USER_CHANNEL, DEMO_HSCMP_DAC_CHANNEL);

    /* Init the LED. */
    LED_INIT();

    /* Enable the interrupt. */
    EnableIRQ(DEMO_HSCMP_IRQ_ID);
    HSCMP_EnableInterrupts(DEMO_HSCMP_BASE, kHSCMP_OutputRisingEventFlag | kHSCMP_OutputFallingEventFlag);

    while (1)
    {
    }
}

/*!
 * @brief ISR for HSCMP interrupt function.
 */
void DEMO_HSCMP_IRQ_HANDLER_FUNC(void)
{
    HSCMP_ClearStatusFlags(DEMO_HSCMP_BASE, kHSCMP_OutputRisingEventFlag | kHSCMP_OutputFallingEventFlag);
    if (kHSCMP_OutputAssertEventFlag == (kHSCMP_OutputAssertEventFlag & HSCMP_GetStatusFlags(DEMO_HSCMP_BASE)))
    {
        LED_ON(); /* Turn on the led. */
    }
    else
    {
        LED_OFF(); /* Turn off the led. */
    }
    SDK_ISR_EXIT_BARRIER;
}
