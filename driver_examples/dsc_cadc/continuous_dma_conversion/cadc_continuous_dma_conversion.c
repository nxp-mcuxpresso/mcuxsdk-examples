/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_common.h"
#include "fsl_cadc.h"
#include "fsl_dma.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_CADC_SAMPLE_COUNT 16U /* The cadc sample count. */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Initialize the DMA.
 */
static void DMA_Configuration(void);

/*!
 * @brief Initialize the CADC.
 */
static void CADC_Configuration(void);

/*!
 * @brief Process ADC values.
 */
static void ProcessSampleData(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_Transfer_Done = false; /* DMA transfer completion flag. */
AT_NONCACHEABLE_SECTION_ALIGN(uint16_t g_cadcSampleDataArray[DEMO_CADC_SAMPLE_COUNT], 2U);
uint32_t g_avgADCValue = 0U; /* Average ADC value .*/

dma_channel_transfer_config_t transferConfig;
dma_handle_t g_DMA_Handle;

/*******************************************************************************
 * Code
 ******************************************************************************/
void DMA_Channel0_IRQHandler(void)
{
    if ((DMA_GetChannelStatusFlags(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL_0) & DMA_DSR_BCR_DONE_MASK) != 0U)
    {
        DMA_ClearChannelDoneStatus(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL_0);
        g_Transfer_Done = true;
    }
}
/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize board hardware. */
    BOARD_InitHardware();

    PRINTF("CADC continuous dma conversion.\r\n");

    CADC_Configuration(); /* Initialize CADC. */
    DMA_Configuration();  /*Initialize DMA.*/
    /* Trigger the converter.
     * Trigger converter A would execute the loop sequential conversion when in
     * "kCADC_ScanModeLoopSequential" work mode. */
    PRINTF("Press any key to get user channel's ADC value ...\r\n");

    CADC_DoSoftwareTriggerConverter(DEMO_CADC_BASEADDR, kCADC_ConverterA);
    while (1)
    {
        GETCHAR();
        while (!g_Transfer_Done)
        {
        }
        g_Transfer_Done = false;
        ProcessSampleData();
        PRINTF("ADC value: %d\r\n", (uint16_t)(g_avgADCValue >> ADC_RSLT_RSLT_SHIFT));

        DMA_TransferChannelSubmit(&g_DMA_Handle, &transferConfig);
        DMA_TransferChannelStart(&g_DMA_Handle, true);
    }
}

static void DMA_Configuration(void)
{
    dma_config_t dmaConfig;
    memset(&dmaConfig, 0U, sizeof(dmaConfig));
    /* Configure DMA one shot transfer */
    DMA_Init(DEMO_DMA_BASEADDR, &dmaConfig);
    DMA_SetChannelPeripheralRequest(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL_0, DEMO_DMA_CHANNEL_REQUEST_SOURCE);
    DMA_TransferChannelCreateHandle(DEMO_DMA_BASEADDR, &g_DMA_Handle, DEMO_DMA_CHANNEL_0, NULL, NULL);
    DMA_GetChannelDefaultTransferConfig(&transferConfig, CADC_RESULT_REG_BYTE_ADDR,
                                        (uint32_t)(uint8_t *)g_cadcSampleDataArray,
                                        sizeof(uint16_t) * DEMO_CADC_SAMPLE_COUNT, kDMA_ChannelTransferWidth16Bits,
                                        kDMA_ChannelTransferPeripheralToMemory);
    transferConfig.bEnableCycleSteal               = true;
    transferConfig.bEnablAutoStopPeripheralRequest = true;
    DMA_TransferChannelSubmit(&g_DMA_Handle, &transferConfig);
    DMA_TransferChannelStart(&g_DMA_Handle, true);
}

static void CADC_Configuration(void)
{
    cadc_config_t cadcConfigStruct;
    /*
     *   psConfig->eDMATriggerSource = kCADC_DMATrigSrcEndofScan;
     *   psConfig->eIdleWorkMode = kCADC_IdleKeepNormal;
     *   psConfig->u16PowerUpDelay = 26U;
     *   psConfig->u32EnabledInterruptMask = 0U;
     *   psConfig->eScanMode = kCADC_ScanModeTriggeredParallelSimultaneous;
     *   psConfig->uDisabledSampleSlot.u32SampleDisVal = 0xFF0F0UL;
     *   psConfig->uScanControl.u32ScanCtrlVal         = 0x0UL;
     *   psConfig->eChannelGain[x] = kCADC_SignalGainX1;
     *   psConfig->sampleSlotScanInterruptEnableMask = kCADC_NonSampleSlotMask;
     *   For the default setting of converter, please see CADC_GetConverterDefaultConfig().
     */
    CADC_GetDefaultConfig(&cadcConfigStruct);

    cadcConfigStruct.eScanMode          = kCADC_ScanModeLoopSequential;
    cadcConfigStruct.eDMATriggerSource  = kCADC_DMATrigSrcEndofScan;
    cadcConfigStruct.u32ChannelModeMask = CDAC_THERMISTOR_CHANNEL_MODE;

    for (uint8_t i = 0U; i < 16U; i++)
    {
        /* Routes channel to sample slots. */
        cadcConfigStruct.eSampleSlot[i] = CADC_THERMISTOR_CHANNEL_NUMBER;
    }
#if (defined(FSL_FEATURE_CADC_RSLT2_COUNT) && (FSL_FEATURE_CADC_RSLT2_COUNT == 4U))
    cadcConfigStruct.uDisabledSampleSlot.eSequentialModeDisSample = kCADC_Sample16Disabled;
#endif /* FSL_FEATURE_CADC_RSLT2_COUNT */

    cadcConfigStruct.sConverterA.bEnableDMA      = true;
    cadcConfigStruct.sConverterA.bPowerUp        = true;
    cadcConfigStruct.sConverterA.u16ClockDivisor = 10U;
    cadcConfigStruct.sConverterB.bEnableDMA      = true;
    cadcConfigStruct.sConverterB.bPowerUp        = true;
    cadcConfigStruct.sConverterB.u16ClockDivisor = 10U;

    CADC_Init(DEMO_CADC_BASEADDR, &cadcConfigStruct);
}

static void ProcessSampleData(void)
{
    uint32_t i = 0U;

    g_avgADCValue = 0;
    /* Get average adc value. */
    for (i = 0; i < DEMO_CADC_SAMPLE_COUNT; i++)
    {
        g_avgADCValue += g_cadcSampleDataArray[i];
    }
    g_avgADCValue = g_avgADCValue / DEMO_CADC_SAMPLE_COUNT;

    /* Reset old value. */
    for (i = 0; i < DEMO_CADC_SAMPLE_COUNT; i++)
    {
        g_cadcSampleDataArray[i] = 0U;
    }
}
