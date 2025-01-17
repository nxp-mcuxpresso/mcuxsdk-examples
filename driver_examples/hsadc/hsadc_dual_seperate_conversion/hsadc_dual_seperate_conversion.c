
/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_hsadc.h"

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
    hsadc_config_t hsadcConfigStruct;
    hsadc_converter_config_t hsadcConverterConfigStruct;
    hsadc_sample_config_t hsadcSampleConfigStruct;
    uint16_t sampleMask;
    uint16_t sampleConvAMask;
    uint16_t sampleConvBMask;

    BOARD_InitHardware();

    PRINTF("HSADC dual separate conversion example.\r\n");

    /* Initialize the HSADC common digital control.
     * "kHSADC_DualConverterWorkAsTriggeredParallel" is used and simultaneous mode is disabled in this case. The two
     * conversion sequence would be executed by each converter. Two converter has independent control logic such as
     * start, stop, DMA enable, sync input etc.
     */
    HSADC_GetDefaultConfig(&hsadcConfigStruct);
    hsadcConfigStruct.enableSimultaneousMode = false;
    HSADC_Init(DEMO_HSADC_BASEADDR, &hsadcConfigStruct);

    /* Configure each converter. */
    HSADC_GetDefaultConverterConfig(&hsadcConverterConfigStruct);
    HSADC_SetConverterConfig(DEMO_HSADC_BASEADDR, kHSADC_ConverterA | kHSADC_ConverterB, &hsadcConverterConfigStruct);
    /* Enable the power for each converter. */
    HSADC_EnableConverterPower(DEMO_HSADC_BASEADDR, kHSADC_ConverterA | kHSADC_ConverterB, true);
    while (
        (kHSADC_ConverterAPowerDownFlag | kHSADC_ConverterBPowerDownFlag) ==
        ((kHSADC_ConverterAPowerDownFlag | kHSADC_ConverterBPowerDownFlag) & HSADC_GetStatusFlags(DEMO_HSADC_BASEADDR)))
    {
    }
    /* Make each converter exit stop mode. */
    HSADC_EnableConverter(DEMO_HSADC_BASEADDR, kHSADC_ConverterA | kHSADC_ConverterB, true);

    /* Configure the samples. */
    HSADC_GetDefaultSampleConfig(&hsadcSampleConfigStruct);
    /* For converter A. */
    hsadcSampleConfigStruct.channelNumber          = DEMO_HSADC_CONVA_CHN_NUM1;
    hsadcSampleConfigStruct.channel67MuxNumber     = DEMO_HSADC_CONVA_CHN67_MUX_NUM1;
    hsadcSampleConfigStruct.enableDifferentialPair = DEMO_HSADC_CONVA_CHN_NUM1_ENABLE_DIFF;
    HSADC_SetSampleConfig(DEMO_HSADC_BASEADDR, 0U, &hsadcSampleConfigStruct);
    hsadcSampleConfigStruct.channelNumber          = DEMO_HSADC_CONVA_CHN_NUM2;
    hsadcSampleConfigStruct.channel67MuxNumber     = DEMO_HSADC_CONVA_CHN67_MUX_NUM2;
    hsadcSampleConfigStruct.enableDifferentialPair = DEMO_HSADC_CONVA_CHN_NUM2_ENABLE_DIFF;
    HSADC_SetSampleConfig(DEMO_HSADC_BASEADDR, 1U, &hsadcSampleConfigStruct);
    /* For converter B.
     * In HSADC_SetSampleConfig(), the channel number 0~7 represents input 0~7 of converter A and channel number 8~15
     * represents input 0~7 of converter B.
     */
    hsadcSampleConfigStruct.channelNumber          = (DEMO_HSADC_CONVB_CHN_NUM1 + 8U);
    hsadcSampleConfigStruct.channel67MuxNumber     = DEMO_HSADC_CONVB_CHN67_MUX_NUM1;
    hsadcSampleConfigStruct.enableDifferentialPair = DEMO_HSADC_CONVB_CHN_NUM1_ENABLE_DIFF;
    HSADC_SetSampleConfig(DEMO_HSADC_BASEADDR, 8U, &hsadcSampleConfigStruct);
    hsadcSampleConfigStruct.channelNumber          = (DEMO_HSADC_CONVB_CHN_NUM2 + 8U);
    hsadcSampleConfigStruct.channel67MuxNumber     = DEMO_HSADC_CONVB_CHN67_MUX_NUM2;
    hsadcSampleConfigStruct.enableDifferentialPair = DEMO_HSADC_CONVB_CHN_NUM2_ENABLE_DIFF;
    HSADC_SetSampleConfig(DEMO_HSADC_BASEADDR, 9U, &hsadcSampleConfigStruct);
    /* Enable the sample slot.
     * The conversion sequence for converter A includes sample slot 0 and 1, while the sequence for converter B
     * includes sample slot 8 and 9. Sample slot 0~7 can reference only to input 0~7 of converter A and sample slot
     * 8~15 can reference only to input 0~7 of converter B in parallel mode.
     */
    sampleConvAMask = HSADC_SAMPLE_MASK(0U) | HSADC_SAMPLE_MASK(1U);
    /* For converter B. */
    sampleConvBMask = HSADC_SAMPLE_MASK(8U) | HSADC_SAMPLE_MASK(9U);
    sampleMask      = sampleConvAMask | sampleConvBMask;
    HSADC_EnableSample(DEMO_HSADC_BASEADDR, sampleMask, true);
    HSADC_EnableSample(DEMO_HSADC_BASEADDR, (uint16_t)(~sampleMask), false); /* Disable other sample slots. */

    /* Calibrate the converter after power up period. */
    HSADC_DoAutoCalibration(DEMO_HSADC_BASEADDR, (kHSADC_ConverterA | kHSADC_ConverterB),
                            (kHSADC_CalibrationModeSingleEnded | kHSADC_CalibrationModeDifferential));
    /* Wait the calibration process complete. Both converter's End of Scan flag will be set after calibration process
    in parallel and simultaneous mode. */
    while ((kHSADC_ConverterAEndOfScanFlag | kHSADC_ConverterBEndOfScanFlag | kHSADC_ConverterAEndOfCalibrationFlag |
            kHSADC_ConverterBEndOfCalibrationFlag) !=
           ((kHSADC_ConverterAEndOfScanFlag | kHSADC_ConverterBEndOfScanFlag | kHSADC_ConverterAEndOfCalibrationFlag |
             kHSADC_ConverterBEndOfCalibrationFlag) &
            HSADC_GetStatusFlags(DEMO_HSADC_BASEADDR)))
    {
    }
    HSADC_ClearStatusFlags(DEMO_HSADC_BASEADDR, kHSADC_ConverterAEndOfScanFlag | kHSADC_ConverterBEndOfScanFlag |
                                                    kHSADC_ConverterAEndOfCalibrationFlag |
                                                    kHSADC_ConverterBEndOfCalibrationFlag);

    PRINTF("Press any key to trigger the conversion ...\r\n");
    PRINTF("\r\nSample 0\tSample 1\tSample 8\tSample 9\r\n");
    while (1)
    {
        PRINTF("\r\n");

        /* Trigger the converter A.
         * The two converters' conversion should be triggered individually when in
         * "kHSADC_DualConverterWorkAsTriggeredParallel" and simultaneous work mode.
         */
        GETCHAR();
        HSADC_DoSoftwareTriggerConverter(DEMO_HSADC_BASEADDR, kHSADC_ConverterA);
        /* Wait the conversion to be done. */
        while (kHSADC_ConverterAEndOfScanFlag !=
               (kHSADC_ConverterAEndOfScanFlag & HSADC_GetStatusFlags(DEMO_HSADC_BASEADDR)))
        {
        }
        /* Read the result value. */
        if (sampleConvAMask == (sampleConvAMask & HSADC_GetSampleReadyStatusFlags(DEMO_HSADC_BASEADDR)))
        {
            PRINTF("%d\t\t", (int16_t)HSADC_GetSampleResultValue(DEMO_HSADC_BASEADDR, 0U));
            PRINTF("%d\t\t", (int16_t)HSADC_GetSampleResultValue(DEMO_HSADC_BASEADDR, 1U));
        }
        HSADC_ClearStatusFlags(DEMO_HSADC_BASEADDR, kHSADC_ConverterAEndOfScanFlag);

        /* Trigger the converter B. */
        GETCHAR();
        HSADC_DoSoftwareTriggerConverter(DEMO_HSADC_BASEADDR, kHSADC_ConverterB);
        /* Wait the conversion to be done. */
        while (kHSADC_ConverterBEndOfScanFlag !=
               (kHSADC_ConverterBEndOfScanFlag & HSADC_GetStatusFlags(DEMO_HSADC_BASEADDR)))
        {
        }
        if (sampleConvBMask == (sampleConvBMask & HSADC_GetSampleReadyStatusFlags(DEMO_HSADC_BASEADDR)))
        {
            PRINTF("%d\t\t", (int16_t)HSADC_GetSampleResultValue(DEMO_HSADC_BASEADDR, 8U));
            PRINTF("%d", (int16_t)HSADC_GetSampleResultValue(DEMO_HSADC_BASEADDR, 9U));
        }
        HSADC_ClearStatusFlags(DEMO_HSADC_BASEADDR, kHSADC_ConverterBEndOfScanFlag);
    }
}
