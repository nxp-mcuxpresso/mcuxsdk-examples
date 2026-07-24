/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_phd.h"
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
    phd_config_t phdConfig;
    phd_comparator_config_t phdcmpConfig;
    hscmp_config_t hscmpConfig;
    hscmp_dac_config_t hscmpDacConfig;

    /* Initialize hardware. */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("PHD Interrupt Example.\r\n");
    PRINTF("The example uses PHD comparator 0 to detect zero-crossing events.\r\n");
    PRINTF("LED turns on when the input rises above the virtual neutral, ");
    PRINTF("and turns off when it falls below.\r\n");

    /*
     * Initialize HSCMP to provide the internal DAC reference voltage for PHD.
     * The HSCMP comparator itself is not used; only its DAC output is routed
     * to PHD as the virtual neutral reference via PHASECTRL[INTSEL].
     */
    /*
     * hscmpConfig.enableStopMode      = false;
     * hscmpConfig.enableOutputPin     = false;
     * hscmpConfig.useUnfilteredOutput = false;
     * hscmpConfig.enableInvertOutput  = false;
     * hscmpConfig.hysteresisMode      = kHSCMP_HysteresisLevel0;
     * hscmpConfig.powerMode           = kHSCMP_LowSpeedPowerMode;
     */
    HSCMP_GetDefaultConfig(&hscmpConfig);

    hscmpConfig.enableComparator = false;
#if (defined(FSL_FEATURE_HSCMP_HAS_FUNC_CLK_SEL) && FSL_FEATURE_HSCMP_HAS_FUNC_CLK_SEL)
    hscmpConfig.funcClockSel = DEMO_HSCMP_FUNC_CLK_SEL;
#endif
    /* Init the HSCMP module. */
    HSCMP_Init(DEMO_HSCMP_BASE, &hscmpConfig);

    /* Configure the HSCMP internal DAC to output half of the reference voltage. */
    hscmpDacConfig.DACValue =
        ((HSCMP_DCR_DAC_DATA_MASK >> HSCMP_DCR_DAC_DATA_SHIFT) >> 1U); /* Half of reference voltage. */
    hscmpDacConfig.enableDacOutput = false;
#if (defined(FSL_FEATURE_HSCMP_HAS_DAC_STOP_EN) && FSL_FEATURE_HSCMP_HAS_DAC_STOP_EN)
    hscmpDacConfig.enableDacStopMode = false;
#endif
    HSCMP_SetDACConfig(DEMO_HSCMP_BASE, &hscmpDacConfig);

    /* Initialize PHD module. */
    /*
     * phdConfig.enableComparatorInStopMode = false;
     * phdConfig.hysteresisLevel            = kPHD_HysteresisLevel0;
     * phdConfig.clockSource                = kPHD_FuncClockSource0;
     * phdConfig.enableExternalNeutral      = false;
     * phdConfig.enableInternalNeutral      = false;
     * phdConfig.enableVirtualNetwork       = false;
     * phdConfig.ibiasTrim                  = kPHD_IbiasTrimValue0;
     * phdConfig.filterCount                = kPHD_FilterCountBypass;
     * phdConfig.filterPeriod               = 0U;
     */
    PHD_GetDefaultConfig(&phdConfig);

    /*
     * Enable virtual resistor network and connect virtual neutral to the
     * HSCMP internal DAC output (INTSEL = 1).
     */
    phdConfig.enableVirtualNetwork  = true;
    phdConfig.enableInternalNeutral = true;
    phdConfig.clockSource = DEMO_PHD_FUNC_CLK_SEL;
    PHD_Init(DEMO_PHD_BASE, &phdConfig);

    /* Configure comparator 0. */
    /*
     * phdcmpConfig.enableOutputPin  = false;
     * phdcmpConfig.outputSelect     = kPHD_OutputFiltered;
     * phdcmpConfig.invertOutput     = false;
     * phdcmpConfig.enableWindowMode = false;
     * phdcmpConfig.enableSampleMode = false;
     */
    PHD_GetDefaultComparatorConfig(&phdcmpConfig);
    PHD_ConfigComparator0(DEMO_PHD_BASE, &phdcmpConfig);

    PHD_SetPhaseSelect(DEMO_PHD_BASE, kPHD_Phase0);

    /* Init the LED. */
    LED_INIT();

    /* Enable comparator 0 interrupt request for both edges. */
    EnableIRQ(DEMO_PHD_IRQ_ID);
    PHD_EnableComparator0Interrupts(DEMO_PHD_BASE, kPHD_ComparatorRisingInterruptEnable |
                                                   kPHD_ComparatorFallingInterruptEnable);

    /* Enable the comparator analog block as the last step. */
    PHD_EnableComparator(DEMO_PHD_BASE);

    while (1)
    {
    }
}

/*!
 * @brief ISR for PHD comparator 0 interrupt.
 */
void DEMO_PHD_IRQ_HANDLER(void)
{
    /* Use COUT to determine the current comparator output level. */
    if (0U != (PHD_GetComparator0StatusFlags(DEMO_PHD_BASE) & kPHD_ComparatorRisingFlag))
    {
        PHD_ClearComparator0StatusFlags(DEMO_PHD_BASE, kPHD_ComparatorRisingFlag);
        LED_ON(); /* INP > virtual neutral: rising zero-crossing detected. */
    }

    if (0U != (PHD_GetComparator0StatusFlags(DEMO_PHD_BASE) & kPHD_ComparatorFallingFlag))
    {
        PHD_ClearComparator0StatusFlags(DEMO_PHD_BASE, kPHD_ComparatorFallingFlag);
        LED_OFF(); /* INP < virtual neutral: falling zero-crossing detected. */
    }

    SDK_ISR_EXIT_BARRIER;
}
