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
volatile bool g_bConverterScanCompleted = false;
uint16_t g_u16ConversionResult[4]       = {0UL};
/*******************************************************************************
 * Code
 ******************************************************************************/
void CADC_ConverterA_End_Scan_IRQHandler(void)
{
    CADC_ClearMiscStatusFlags(DEMO_CADC_BASEADDR, kCADC_ConverterAEndOfScanFlag);
    g_u16ConversionResult[0]  = CADC_GetSampleSlotResultValue(DEMO_CADC_BASEADDR, kCADC_SampleSlot0Index);
    g_u16ConversionResult[1]  = CADC_GetSampleSlotResultValue(DEMO_CADC_BASEADDR, kCADC_SampleSlot1Index);
    g_u16ConversionResult[2]  = CADC_GetSampleSlotResultValue(DEMO_CADC_BASEADDR, kCADC_SampleSlot8Index);
    g_u16ConversionResult[3]  = CADC_GetSampleSlotResultValue(DEMO_CADC_BASEADDR, kCADC_SampleSlot9Index);
    g_bConverterScanCompleted = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    cadc_config_t sCadcConfigStruct;

    BOARD_InitHardware();

    PRINTF("\r\nCADC parallel simultaneous conversion example.\r\n");

    /* Initialize the CADC common digital control.
     * "kCADC_ScanModeTriggeredParallelSimultaneous" is used in this case. The two short conversion sequence would be
     * executed by each converter at the same time. Both converter shares the converter A control logic, including the
     * trigger input.
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

    sCadcConfigStruct.eScanMode = kCADC_ScanModeTriggeredParallelSimultaneous;

    /* Enables end of scan interrupt. */
    sCadcConfigStruct.u32EnabledInterruptMask = kCADC_ConversionCompleteInterrupt0Enable;

    sCadcConfigStruct.uDisabledSampleSlot.eSimultParallelModeDisSample = kCADC_Sample2_10Disabled;

    /* Sets channels' mode. */
    sCadcConfigStruct.u32ChannelModeMask = DEMO_CADC_SAMPLE0_CHANNEL_MODE | DEMO_CADC_SAMPLE1_CHANNEL_MODE |
                                           DEMO_CADC_SAMPLE8_CHANNEL_MODE | DEMO_CADC_SAMPLE9_CHANNEL_MODE;

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

    /* The conversion sequence for converter A includes sample 0 and sample 1, while the sequence for converter B
     * includes sample 8 and 9. */

    PRINTF("Press any key to trigger the conversion ...\r\n");
    PRINTF("\r\nSample 0\tSample 1\tSample 8\tSample 9\r\n");
    while (1)
    {
        PRINTF("\r\n");

        /* Trigger the converter.
         * Trigger converter A would execute both converter's conversion when in
         * "kCADC_DualConverterWorkAsTriggeredParallel" work mode. */
        GETCHAR();
        CADC_DoSoftwareTriggerConverter(DEMO_CADC_BASEADDR, kCADC_ConverterA);

        /* Wait the conversion to be done. */
        while (!g_bConverterScanCompleted)
        {
        }
        g_bConverterScanCompleted = false;
        PRINTF("%d\t\t", g_u16ConversionResult[0]);
        PRINTF("%d\t\t", g_u16ConversionResult[1]);
        PRINTF("%d\t\t", g_u16ConversionResult[2]);
        PRINTF("%d\t\t", g_u16ConversionResult[3]);
    }
}
