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
volatile bool g_bScanCompleted    = false;
uint16_t g_u16ConversionResult[4] = {0UL};
/*******************************************************************************
 * Code
 ******************************************************************************/
void CADC_ConverterA_End_Scan_IRQHandler(void)
{
    CADC_ClearMiscStatusFlags(DEMO_CADC_BASEADDR, kCADC_ConverterAEndOfScanFlag);
    g_u16ConversionResult[0] = CADC_GetSampleSlotResultValue(DEMO_CADC_BASEADDR, kCADC_SampleSlot0Index);
    g_u16ConversionResult[1] = CADC_GetSampleSlotResultValue(DEMO_CADC_BASEADDR, kCADC_SampleSlot1Index);
    g_u16ConversionResult[2] = CADC_GetSampleSlotResultValue(DEMO_CADC_BASEADDR, kCADC_SampleSlot2Index);
    g_u16ConversionResult[3] = CADC_GetSampleSlotResultValue(DEMO_CADC_BASEADDR, kCADC_SampleSlot3Index);
    g_bScanCompleted         = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    cadc_config_t sCadcConfigStruct;
    uint8_t u8Index = 0U;

    BOARD_InitHardware();

    PRINTF("\r\nCADC sequential conversion example.\r\n");

    /* Initialize the CADC.
     * "kCADC_ScanModeTriggeredSequential" is used in this case. The sequence will be controlled by converter A's
     * control logic, including the trigger input.
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

    sCadcConfigStruct.eScanMode               = kCADC_ScanModeTriggeredSequential;
    sCadcConfigStruct.u32EnabledInterruptMask = kCADC_ConversionCompleteInterrupt0Enable;

    sCadcConfigStruct.uDisabledSampleSlot.eSequentialModeDisSample = kCADC_Sample4Disabled;

    sCadcConfigStruct.u32ChannelModeMask = DEMO_CADC_SAMPLE0_CHANNEL_MODE | DEMO_CADC_SAMPLE1_CHANNEL_MODE |
                                           DEMO_CADC_SAMPLE2_CHANNEL_MODE | DEMO_CADC_SAMPLE3_CHANNEL_MODE;

    sCadcConfigStruct.eSampleSlot[0] = DEMO_CADC_SAMPLE0_CHANNEL_NAME;
    sCadcConfigStruct.eSampleSlot[1] = DEMO_CADC_SAMPLE1_CHANNEL_NAME;
    sCadcConfigStruct.eSampleSlot[2] = DEMO_CADC_SAMPLE2_CHANNEL_NAME;
    sCadcConfigStruct.eSampleSlot[3] = DEMO_CADC_SAMPLE3_CHANNEL_NAME;

    sCadcConfigStruct.sConverterA.bPowerUp        = true;
    sCadcConfigStruct.sConverterA.u16ClockDivisor = 9U; /* So ConverterA clock = (peripheral clock / 10). */
    sCadcConfigStruct.sConverterB.bPowerUp        = true;
    sCadcConfigStruct.sConverterB.u16ClockDivisor = 9U; /* So ConverterB clock = (peripheral clock / 10). */

    CADC_Init(DEMO_CADC_BASEADDR, &sCadcConfigStruct);

    EnableIRQWithPriority(DEMO_ADC_CC0_IRQn, 1);

    /* The available sample slots are sample 0, 1, 2, 3. */

    PRINTF("Press any key to trigger the conversion ...\r\n");
    PRINTF("\r\nSample 0\tSample 1\tSample 2\tSample 3\r\n");
    while (1)
    {
        GETCHAR();
        PRINTF("\r\n");

        /* Trigger the converter.
         * Trigger converter A would execute the long sequential conversion when in
         * "kCADC_DualConverterWorkAsTriggeredSequential" work mode. */
        CADC_DoSoftwareTriggerConverter(DEMO_CADC_BASEADDR, kCADC_ConverterA);

        /* Wait the conversion to be done. */
        while (!g_bScanCompleted)
        {
        }
        g_bScanCompleted = false;
        for (u8Index = 0U; u8Index < 4U; u8Index++)
        {
            PRINTF("%d\t\t", g_u16ConversionResult[u8Index]);
        }
    }
}
