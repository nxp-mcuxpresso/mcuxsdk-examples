/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_adc.h"
#include "fsl_dma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_ADC16_SAMPLE_COUNT 16U /* The ADC16 sample count. */
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Initialize the DMA.
 */
static void DMA_Configuration(void);

/*!
 * @brief Initialize the ADC16.
 */
static void ADC16_Configuration(void);

/*!
 * @brief Process ADC values.
 */
static void ProcessSampleData(void);

/*!
 * @brief Callback function for DMA.
 */
static void DMA_Callback(struct _dma_handle *handle, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint16_t g_adc16SampleDataArray[DEMO_ADC16_SAMPLE_COUNT]; /* ADC value array. */
const uint32_t g_Adc16_16bitFullRange = 65536U;
static uint32_t g_avgADCValue         = 0U; /* Average ADC value. */

volatile bool g_Transfer_Done = false; /* DMA transfer completion flag. */
dma_channel_transfer_config_t transferConfig;
dma_config_t dmaConfig;
dma_handle_t g_DMA_Handle;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */

int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("ADC16 CONTINUOUS DMA DEMO\r\n");

    ADC16_Configuration(); /* Initialize ADC16. */
    DMA_Configuration();   /* Initialize DMA. */

    Adc16_DoSoftwareTrigger(DEMO_ADC16_BASE, DEMO_ADC16_GROUPID);

    PRINTF("ADC Full Range: %ld\r\n", g_Adc16_16bitFullRange);
    PRINTF("Press any key to get user channel's ADC value ...\r\n");

    while (1)
    {
        GETCHAR();
        g_Transfer_Done = false;
        while (!g_Transfer_Done)
        {
        }
        ProcessSampleData();
        PRINTF("ADC value: %ld\r\n", g_avgADCValue);
    }
}

static void DMA_Configuration(void)
{
    /* Configure DMA one shot transfer */
    DMA_Init(DEMO_DMA_BASEADDR, &dmaConfig);
    DMA_SetChannelPeripheralRequest(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL_0, kDmaCH0_ADCC_EC);
    DMA_TransferChannelCreateHandle(DEMO_DMA_BASEADDR, &g_DMA_Handle, DEMO_DMA_CHANNEL_0, DMA_Callback, NULL);
    DMA_GetChannelDefaultTransferConfig(&transferConfig, SDK_GET_REGISTER_BYTE_ADDR(SADC_Type, DEMO_ADC16_BASE, R[0]),
                                        (uint32_t)(uint8_t *)g_adc16SampleDataArray,
                                        sizeof(uint16_t) * DEMO_ADC16_SAMPLE_COUNT, kDMA_ChannelTransferWidth16Bits,
                                        kDMA_ChannelTransferPeripheralToMemory);
    transferConfig.bEnableCycleSteal               = true;
    transferConfig.bEnablAutoStopPeripheralRequest = true;
    DMA_TransferChannelSubmit(&g_DMA_Handle, &transferConfig);
    DMA_TransferChannelStart(&g_DMA_Handle, true);
}

static void ADC16_Configuration(void)
{
    adc16_config_t adcUserConfig;

    /*
     * Initialization ADC for 16bit resolution, DMA mode, normal convert speed, VREFH/L as reference,
     * enable continuous convert mode.
     */

    ADC16_GetDefaultConfig(&adcUserConfig);

    adcUserConfig.eResolution                           = kADC16_Resolution16bit;
    adcUserConfig.bEnableContinuousConversion           = true;
    adcUserConfig.eClockSource                          = kADC16_ClockSourceAlt1;
    adcUserConfig.eSampleMode                           = kADC16_SampleCycle24;
    adcUserConfig.bEnableLowPower                       = true;
    adcUserConfig.eChannelInput                         = kADC16_Input17Channel;
    adcUserConfig.u32GroupId                            = DEMO_ADC16_GROUPID;
    adcUserConfig.bEnableInterruptOnConversionCompleted = false;
    adcUserConfig.bEnableDMA                            = true;

    ADC16_Init(DEMO_ADC16_BASE, &adcUserConfig);

    /* Auto calibration */
    if (kStatus_Success == ADC16_DoAutoCalibration(DEMO_ADC16_BASE))
    {
        PRINTF("ADC16_DoAutoCalibration() Done.\r\n");
    }
    else
    {
        PRINTF("ADC16_DoAutoCalibration() Failed.\r\n");
    }
}

static void ProcessSampleData(void)
{
    uint32_t i = 0U;

    g_avgADCValue = 0;
    /* Get average adc value. */
    for (i = 0; i < DEMO_ADC16_SAMPLE_COUNT; i++)
    {
        g_avgADCValue += g_adc16SampleDataArray[i];
    }
    g_avgADCValue = g_avgADCValue / DEMO_ADC16_SAMPLE_COUNT;

    /* Reset old value. */
    for (i = 0; i < DEMO_ADC16_SAMPLE_COUNT; i++)
    {
        g_adc16SampleDataArray[i] = 0U;
    }
}

static void DMA_Callback(struct _dma_handle *handle, void *userData)
{
    DMA_TransferChannelSubmit(&g_DMA_Handle, &transferConfig);
    DMA_TransferChannelStart(&g_DMA_Handle, true);

    g_Transfer_Done = true;
}
