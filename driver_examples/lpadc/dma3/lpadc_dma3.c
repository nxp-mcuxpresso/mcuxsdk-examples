/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_edma.h"
#include "fsl_lpadc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LPADC_RESULT_MASK ADC_RESFIFO_D_MASK

#if (defined(DEMO_LPADC_USE_HIGH_RESOLUTION) && DEMO_LPADC_USE_HIGH_RESOLUTION)
#define LPADC_FULL_RANGE        65536U
#define LPADC_RESULT_SHIFT      0U
#else
#define LPADC_FULL_RANGE        4096U
#define LPADC_RESULT_SHIFT      3U
#endif /* DEMO_LPADC_USE_HIGH_RESOLUTION */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void ADC_Configuration(void);
static void DMA_Configuration(void);
static void ProcessSampleData(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint32_t s_avgAdcValue;
static edma_handle_t s_edmaHandle;
static volatile bool s_dmaTransferDoneFlag;
static lpadc_conv_command_config_t s_lpadcCommandConfig;
AT_NONCACHEABLE_SECTION_ALIGN_INIT(uint32_t s_adcConvResult[DEMO_LPADC_SAMPLE_COUNT], 32U) = {0U};

/*******************************************************************************
 * Code
 ******************************************************************************/
static void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    if (transferDone)
    {
        ProcessSampleData();
        s_dmaTransferDoneFlag = true;
    }
}

int main(void)
{
    edma_transfer_config_t transferConfig;
    status_t status;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
    PRINTF("LPADC DMA3 Example\r\n");

    DMA_Configuration();
    ADC_Configuration();

    PRINTF("ADC Full Range: %d\r\n", LPADC_FULL_RANGE);
#if defined(FSL_FEATURE_LPADC_HAS_CMDL_CSCALE) && FSL_FEATURE_LPADC_HAS_CMDL_CSCALE
    if (kLPADC_SampleFullScale == s_lpadcCommandConfig.sampleScaleMode)
    {
        PRINTF("Full channel scale (Factor of 1).\r\n");
    }
    else if (kLPADC_SamplePartScale == s_lpadcCommandConfig.sampleScaleMode)
    {
        PRINTF("Divided input voltage signal. (Factor of 30/64).\r\n");
    }
#endif

    PRINTF("Please press any key to trigger the conversion.\r\n");

    while (1)
    {
        GETCHAR();

        s_dmaTransferDoneFlag = false;

        EDMA_PrepareTransfer(&transferConfig, (void *)DEMO_LPADC_RESFIFO_REG_ADDR, sizeof(uint32_t),
                             (void *)s_adcConvResult, sizeof(s_adcConvResult[0]), sizeof(s_adcConvResult[0]),
                             sizeof(s_adcConvResult), kEDMA_PeripheralToMemory);
        status = EDMA_SubmitTransfer(&s_edmaHandle, &transferConfig);
        if (status != kStatus_Success)
        {
            PRINTF("EDMA submit transfer failed.\r\n");
            continue;
        }
        EDMA_StartTransfer(&s_edmaHandle);

        LPADC_DoSoftwareTrigger(DEMO_LPADC_BASE, 1UL);

        while (!s_dmaTransferDoneFlag)
        {
        }

        PRINTF("ADC conversion word: 0x%X\r\n", s_adcConvResult[DEMO_LPADC_SAMPLE_COUNT - 1U]);
        PRINTF("ADC conversion value: %d\r\n", s_avgAdcValue);
    }
}

static void ADC_Configuration(void)
{
    lpadc_config_t lpadcConfig;
    lpadc_conv_trigger_config_t lpadcTriggerConfig;

    LPADC_GetDefaultConfig(&lpadcConfig);

#if defined(FSL_FEATURE_LPADC_HAS_CFG_PWRSEL) && (FSL_FEATURE_LPADC_HAS_CFG_PWRSEL == 1U)
    lpadcConfig.powerLevelMode = kLPADC_PowerLevelAlt4;
#endif /* FSL_FEATURE_LPADC_HAS_CFG_PWRSEL */
    lpadcConfig.enableAnalogPreliminary = true;
#if defined(DEMO_LPADC_VREF_SOURCE)
    lpadcConfig.referenceVoltageSource = DEMO_LPADC_VREF_SOURCE;
#endif /* DEMO_LPADC_VREF_SOURCE */
#if (defined(FSL_FEATURE_LPADC_FIFO_COUNT) && (FSL_FEATURE_LPADC_FIFO_COUNT == 2U))
    lpadcConfig.FIFO0Watermark = 0U;
    lpadcConfig.FIFO1Watermark = 0U;
#else
    lpadcConfig.FIFOWatermark = 0U;
#endif /* FSL_FEATURE_LPADC_FIFO_COUNT */
    LPADC_Init(DEMO_LPADC_BASE, &lpadcConfig);

#if defined(FSL_FEATURE_LPADC_HAS_CTRL_CALOFSMODE) && FSL_FEATURE_LPADC_HAS_CTRL_CALOFSMODE
    LPADC_SetOffsetCalibrationMode(DEMO_LPADC_BASE, DEMO_LPADC_OFFSET_CALIBRATION_MODE);
#endif /* FSL_FEATURE_LPADC_HAS_CTRL_CALOFSMODE */

#if defined(FSL_FEATURE_LPADC_HAS_CTRL_CALOFS) && FSL_FEATURE_LPADC_HAS_CTRL_CALOFS
#if defined(DEMO_LPADC_DO_OFFSET_CALIBRATION) && DEMO_LPADC_DO_OFFSET_CALIBRATION
    LPADC_DoOffsetCalibration(DEMO_LPADC_BASE);
#else

#if defined(FSL_FEATURE_LPADC_HAS_OFSTRIM) && FSL_FEATURE_LPADC_HAS_OFSTRIM
#if defined(FSL_FEATURE_LPADC_OFSTRIM_COUNT) && (FSL_FEATURE_LPADC_OFSTRIM_COUNT == 2U)
    LPADC_SetOffsetValue(DEMO_LPADC_BASE, DEMO_LPADC_OFFSET_VALUE_A, DEMO_LPADC_OFFSET_VALUE_B);
#elif defined(FSL_FEATURE_LPADC_OFSTRIM_COUNT) && (FSL_FEATURE_LPADC_OFSTRIM_COUNT == 1U)
    LPADC_SetOffsetValue(DEMO_LPADC_BASE, DEMO_LPADC_OFFSET_VALUE);
#endif /* FSL_FEATURE_LPADC_OFSTRIM_COUNT */
#else
    if (DEMO_LPADC_OFFSET_CALIBRATION_MODE == kLPADC_OffsetCalibration12bitMode)
    {
        LPADC_SetOffset12BitValue(DEMO_LPADC_BASE, DEMO_LPADC_OFFSET_VALUE_A, DEMO_LPADC_OFFSET_VALUE_B);
    }
    else
    {
        LPADC_SetOffset16BitValue(DEMO_LPADC_BASE, DEMO_LPADC_OFFSET_VALUE_A, DEMO_LPADC_OFFSET_VALUE_B);
    }
#endif /* FSL_FEATURE_LPADC_HAS_OFSTRIM */

#endif /* DEMO_LPADC_DO_OFFSET_CALIBRATION */
#endif /* FSL_FEATURE_LPADC_HAS_CTRL_CALOFS */

#if defined(FSL_FEATURE_LPADC_HAS_CTRL_CAL_REQ) && FSL_FEATURE_LPADC_HAS_CTRL_CAL_REQ
    SDK_DelayAtLeastUs(1U, SystemCoreClock);
    LPADC_DoAutoCalibration(DEMO_LPADC_BASE);
#endif /* FSL_FEATURE_LPADC_HAS_CTRL_CAL_REQ */

#if (defined(FSL_FEATURE_LPADC_HAS_CFG_CALOFS) && FSL_FEATURE_LPADC_HAS_CFG_CALOFS)
    SDK_DelayAtLeastUs(1U, SystemCoreClock);
    LPADC_DoAutoCalibration(DEMO_LPADC_BASE);
#endif /* FSL_FEATURE_LPADC_HAS_CFG_CALOFS */

    LPADC_GetDefaultConvCommandConfig(&s_lpadcCommandConfig);
    s_lpadcCommandConfig.channelNumber = DEMO_LPADC_USER_CHANNEL;
    s_lpadcCommandConfig.loopCount     = DEMO_LPADC_SAMPLE_COUNT - 1U;
#if defined(DEMO_LPADC_USE_HIGH_RESOLUTION) && DEMO_LPADC_USE_HIGH_RESOLUTION
    s_lpadcCommandConfig.conversionResolutionMode = kLPADC_ConversionResolutionHigh;
#endif /* DEMO_LPADC_USE_HIGH_RESOLUTION */
    LPADC_SetConvCommandConfig(DEMO_LPADC_BASE, DEMO_LPADC_USER_CMDID, &s_lpadcCommandConfig);

    LPADC_GetDefaultConvTriggerConfig(&lpadcTriggerConfig);
    lpadcTriggerConfig.targetCommandId       = DEMO_LPADC_USER_CMDID;
    lpadcTriggerConfig.enableHardwareTrigger = false;
    LPADC_SetConvTriggerConfig(DEMO_LPADC_BASE, 0U, &lpadcTriggerConfig);

#if (defined(FSL_FEATURE_LPADC_FIFO_COUNT) && (FSL_FEATURE_LPADC_FIFO_COUNT == 2U))
    LPADC_EnableFIFO0WatermarkDMA(DEMO_LPADC_BASE, true);
#else
    LPADC_EnableFIFOWatermarkDMA(DEMO_LPADC_BASE, true);
#endif /* FSL_FEATURE_LPADC_FIFO_COUNT */
}

static void DMA_Configuration(void)
{
    edma_config_t edmaConfig;

    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(DEMO_DMA_BASE, &edmaConfig);

#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(DEMO_DMA_BASE, DEMO_DMA_CHANNEL, DEMO_DMA_REQUEST);
#endif /* FSL_FEATURE_EDMA_HAS_CHANNEL_MUX */

    EDMA_CreateHandle(&s_edmaHandle, DEMO_DMA_BASE, DEMO_DMA_CHANNEL);
    EDMA_SetCallback(&s_edmaHandle, EDMA_Callback, NULL);
}

static void ProcessSampleData(void)
{
    uint32_t i;

    s_avgAdcValue = 0U;

    for (i = 0U; i < DEMO_LPADC_SAMPLE_COUNT; i++)
    {
         s_avgAdcValue += ((s_adcConvResult[i] & LPADC_RESULT_MASK) >> LPADC_RESULT_SHIFT);
    }

    s_avgAdcValue /= DEMO_LPADC_SAMPLE_COUNT;
}
