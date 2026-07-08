/*
 * Copyright 2021, 2026 NXP
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

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

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

    mHscmpConfigStruct.enableComparator = false;
#if (defined(FSL_FEATURE_HSCMP_HAS_FUNC_CLK_SEL) && FSL_FEATURE_HSCMP_HAS_FUNC_CLK_SEL)
    mHscmpConfigStruct.funcClockSel = DEMO_HSCMP_FUNC_CLK_SEL;
#endif
    /* Init the HSCMP module. */
    HSCMP_Init(DEMO_HSCMP_BASE, &mHscmpConfigStruct);

    /* Configure the internal DAC to output half of reference voltage. */
#if (defined(FSL_FEATURE_HSCMP_HAS_DAC_PWR_MODE_SELECT) && FSL_FEATURE_HSCMP_HAS_DAC_PWR_MODE_SELECT)
    mHscmpDacConfigStruct.enableLowPowerMode     = false;
#endif
#if (defined(FSL_FEATURE_HSCMP_HAS_DAC_REF_VOL_SELECT) && FSL_FEATURE_HSCMP_HAS_DAC_REF_VOL_SELECT)
    mHscmpDacConfigStruct.referenceVoltageSource = kHSCMP_VrefSourceVin2;
#endif
#if (defined(FSL_FEATURE_HSCMP_HAS_DAC_STOP_EN) && FSL_FEATURE_HSCMP_HAS_DAC_STOP_EN)
    mHscmpDacConfigStruct.enableDacStopMode = false;
#endif
    mHscmpDacConfigStruct.DACValue =
        ((HSCMP_DCR_DAC_DATA_MASK >> HSCMP_DCR_DAC_DATA_SHIFT) >> 1U); /* Half of reference voltage. */
    mHscmpDacConfigStruct.enableDacOutput = false;
    HSCMP_SetDACConfig(DEMO_HSCMP_BASE, &mHscmpDacConfigStruct);

    /* Configure HSCMP input channels. */
    HSCMP_SetInputChannels(DEMO_HSCMP_BASE, DEMO_HSCMP_USER_CHANNEL, DEMO_HSCMP_DAC_CHANNEL);

    /* Init the LED. */
    LED_INIT();

    /* Enable the interrupt. */
    EnableIRQ(DEMO_HSCMP_IRQ_ID);
    HSCMP_EnableInterrupts(DEMO_HSCMP_BASE, kHSCMP_OutputRisingEventFlag | kHSCMP_OutputFallingEventFlag);

    /* Enable HSCMP after all configuration is done. */
    HSCMP_Enable(DEMO_HSCMP_BASE, true);

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
