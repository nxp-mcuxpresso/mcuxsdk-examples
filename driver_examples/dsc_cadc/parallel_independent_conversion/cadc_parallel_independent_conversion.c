/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_cadc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_bConverterAScanCompleted = false;
volatile bool g_bConverterBScanCompleted = false;

uint16_t g_u16ConverterAResult[2] = {0UL};
uint16_t g_u16ConverterBResult[2] = {0UL};
/*******************************************************************************
 * Code
 ******************************************************************************/

void CADC_ConverterA_End_Scan_IRQHandler(void)
{
    CADC_ClearMiscStatusFlags(DEMO_CADC_BASEADDR, kCADC_ConverterAEndOfScanFlag);
    g_u16ConverterAResult[0]   = CADC_GetSampleSlotResultValue(DEMO_CADC_BASEADDR, kCADC_SampleSlot0Index);
    g_u16ConverterAResult[1]   = CADC_GetSampleSlotResultValue(DEMO_CADC_BASEADDR, kCADC_SampleSlot1Index);
    g_bConverterAScanCompleted = true;
}

void CADC_ConverterB_End_Scan_IRQHandler(void)
{
    CADC_ClearMiscStatusFlags(DEMO_CADC_BASEADDR, kCADC_ConverterBEndOfScanFlag);
    g_u16ConverterBResult[0]   = CADC_GetSampleSlotResultValue(DEMO_CADC_BASEADDR, kCADC_SampleSlot8Index);
    g_u16ConverterBResult[1]   = CADC_GetSampleSlotResultValue(DEMO_CADC_BASEADDR, kCADC_SampleSlot9Index);
    g_bConverterBScanCompleted = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    cadc_config_t sCadcConfigStruct;

    BOARD_InitHardware();

    PRINTF("\r\nCADC parallel independent conversion example.\r\n");

    /* Initialize the CADC.
     * "kCADC_ScanModeTriggeredParallelIndependent" is used in this case. Dual converter will scan independently.
     */
    /*
     *   config->eDMATriggerSource = kCADC_EndofScanTrigDMA;
     *   config->eIdleWorkMode = kCADC_IdleKeepNormal;
     *   config->u16PowerUpDelay = 26U;
     *   config->u16EnabledInterruptMask = 0U;
     *   config->eScanMode = kCADC_ScanModeTriggeredParallelSimultaneous;
     *   config->uDisabledSampleSlot.u32_sampleDisVal = 0xFF0F0UL;
     *   config->uScanControl.u32_scanCtrlVal         = 0x0UL;
     *   config->eChannelGain[x] = kCADC_SignalGainX1;
     *   config->u32EnabledInterruptMask = 0UL;
     */
    CADC_GetDefaultConfig(&sCadcConfigStruct);

    /* Sets scan mode. */
    sCadcConfigStruct.eScanMode = kCADC_ScanModeTriggeredParallelIndependent;

    /* Enable end of scan interrupts. */
    sCadcConfigStruct.u32EnabledInterruptMask =
        kCADC_ConversionCompleteInterrupt0Enable | kCADC_ConversionCompleteInterrupt1Enable;

    /* Sets channels' mode. */
    sCadcConfigStruct.u32ChannelModeMask = DEMO_CADC_SAMPLE0_CHANNEL_MODE | DEMO_CADC_SAMPLE1_CHANNEL_MODE |
                                           DEMO_CADC_SAMPLE8_CHANNEL_MODE | DEMO_CADC_SAMPLE9_CHANNEL_MODE;

    /* Sets disabled sample slots for each converter. */
    sCadcConfigStruct.uDisabledSampleSlot.sIndependentParallelModeDisSample.eConverterA = kCADC_ConvASample2Disabled;
    sCadcConfigStruct.uDisabledSampleSlot.sIndependentParallelModeDisSample.eConverterB = kCADC_ConvBSample10Disabled;

    /* Routes channel to sample slots.  */
    sCadcConfigStruct.eSampleSlot[0] = DEMO_CADC_SAMPLE0_CHANNEL_NAME;
    sCadcConfigStruct.eSampleSlot[1] = DEMO_CADC_SAMPLE1_CHANNEL_NAME;
    sCadcConfigStruct.eSampleSlot[8] = DEMO_CADC_SAMPLE8_CHANNEL_NAME;
    sCadcConfigStruct.eSampleSlot[9] = DEMO_CADC_SAMPLE9_CHANNEL_NAME;

    /* Power up each converterA. */
    sCadcConfigStruct.sConverterA.bPowerUp = true;
    /* Set converterA clock divisor */
    sCadcConfigStruct.sConverterA.u16ClockDivisor = 9U; /* So ConverterA clock = (peripheral clock / 10). */

    /* Power up each converterB. */
    sCadcConfigStruct.sConverterB.bPowerUp = true;
    /* Set converterB clock divisor */
    sCadcConfigStruct.sConverterB.u16ClockDivisor = 9U; /* So ConverterB clock = (peripheral clock / 10). */

    CADC_Init(DEMO_CADC_BASEADDR, &sCadcConfigStruct);

    EnableIRQWithPriority(DEMO_ADC_CC0_IRQn, 1);
    EnableIRQWithPriority(DEMO_ADC_CC1_IRQn, 2);

    /* The conversion sequence for converter A includes sample 0 and sample 1, while the sequence for converter B
     * includes sample 8 and 9.
     */

    PRINTF("Press any key to trigger the conversion ...\r\n");
    PRINTF("\r\nSample 0\tSample 1\tSample 8\tSample 9\r\n");
    while (1)
    {
        PRINTF("\r\n");

        /* Trigger the converter A. */
        GETCHAR();
        CADC_DoSoftwareTriggerConverter(DEMO_CADC_BASEADDR, kCADC_ConverterA);
        /* Wait the conversion to be done. */
        while (!g_bConverterAScanCompleted)
        {
        }
        g_bConverterAScanCompleted = false;

        PRINTF("%d\t\t", g_u16ConverterAResult[0]);
        PRINTF("%d\t\t", g_u16ConverterAResult[1]);

        /* Trigger the converter B. */
        GETCHAR();
        CADC_DoSoftwareTriggerConverter(DEMO_CADC_BASEADDR, kCADC_ConverterB);
        /* Wait the conversion to be done. */
        while (!g_bConverterBScanCompleted)
        {
        }
        g_bConverterBScanCompleted = false;

        PRINTF("%d\t\t", g_u16ConverterBResult[0]);
        PRINTF("%d\t\t", g_u16ConverterBResult[1]);
    }
}
