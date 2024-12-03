/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_adc.h"
#include "fsl_dma.h"
#include "fsl_power.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ADC_BURST_EXAMPLE_NUMBER 10

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint32_t g_AdcVinn;
float g_AdcBandgap;
dma_handle_t g_adcDmaHandle;
uint32_t g_AdcConvResult;
volatile bool g_AdcConvSeqCpltFlag;
const uint32_t g_Adc_12bitFullRange = 4096U;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void ADC_Configuration(void);
static void ADC_ConvertDMACallback(dma_handle_t *handle, void *param, bool transferDone, uint32_t intmode);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    dma_transfer_config_t transferConfig;

    /* Power on ADC */
    POWER_EnableADC(true);

    /* Initialize board hardware. */
    BOARD_InitHardware();
    PRINTF("\r\nADC dma example.\r\n");

    /* Configure DMA. */
    DMA_Init(DMA0);
    DMA_EnableChannel(DMA0, DEMO_ADC_DMA_CHANNEL);
    DMA_CreateHandle(&g_adcDmaHandle, DMA0, DEMO_ADC_DMA_CHANNEL);
    DMA_SetCallback(&g_adcDmaHandle, ADC_ConvertDMACallback, NULL);
    DMA_PrepareTransfer(&transferConfig, (void *)&DEMO_ADC_BASE->DATA, &g_AdcConvResult, sizeof(uint32_t), 4,
                        kDMA_PeripheralToMemory, NULL);
    DMA_SubmitTransfer(&g_adcDmaHandle, &transferConfig);

    /* Configure the converter and work mode. */
    ADC_Configuration();
    PRINTF("Configuration Done.\r\n");
    PRINTF("ADC Full Range: %d\r\n", g_Adc_12bitFullRange);
    PRINTF("Press any key to get demo channel's ADC value ...\r\n");

    /**
     * ADC single mode
     * (Vext - Vinn) / Vref = Vreg / 2^22  ==> Vext = Vreg * Vref / 2^22 + Vinn
     */
    float fresult;
    while (1)
    {
        GETCHAR();

        g_AdcConvSeqCpltFlag = false;
        DMA_StartTransfer(&g_adcDmaHandle);

        /* Do software trigger */
        ADC_DoSoftwareTrigger(DEMO_ADC_BASE);

        /* Wait for complete */
        while (!g_AdcConvSeqCpltFlag)
        {
        }

        fresult = ADC_ConversionResult2Mv(DEMO_ADC_BASE, DEMO_ADC_CHANNEL, DEMO_ADC_CFG_IDX, g_AdcBandgap, g_AdcVinn,
                                          g_AdcConvResult);
        PRINTF("Original: 0x%x\t Ch: %d\t Result: %f(mv)\r\n", g_AdcConvResult, DEMO_ADC_CHANNEL, fresult);
    }
}

static void ADC_Configuration(void)
{
    adc_config_t adcConfigStruct;
    adc_sd_config_t adcSdConfigStruct;

    /**
     * Initial ADC to default configuration.
     */
    ADC_GetDefaultConfig(&adcConfigStruct);
    adcConfigStruct.channelEnable = (1U << DEMO_ADC_CHANNEL);
    adcConfigStruct.channelConfig = (DEMO_ADC_CFG_IDX << DEMO_ADC_CHANNEL);
    adcConfigStruct.triggerSource = DEMO_ADC_TRIGGER;
    adcConfigStruct.convMode      = kADC_ConvModeSingle;
    ADC_Init(DEMO_ADC_BASE, &adcConfigStruct);

    /* Initial ADC Sigma Delta(SD) configuration */
    ADC_GetSdDefaultConfig(&adcSdConfigStruct);
    ADC_SetSdConfig(DEMO_ADC_BASE, DEMO_ADC_CFG_IDX, &adcSdConfigStruct);

    /* Bandgap voltage */
    g_AdcBandgap = ADC_GetBandgapCalibrationResult(DEMO_ADC_BASE, DEMO_ADC_CFG_IDX);

    /* Calibration VINN value */
    g_AdcVinn = ADC_GetVinnCalibrationResult(DEMO_ADC_BASE, &adcConfigStruct);

    /* Enable ADC */
    ADC_Enable(DEMO_ADC_BASE, true);
}

static void ADC_ConvertDMACallback(dma_handle_t *handle, void *param, bool transferDone, uint32_t intmode)
{
    g_AdcConvSeqCpltFlag = true;
}
