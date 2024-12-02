/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "app.h"
#include "board.h"
#include "hal_clock.h"
#include "fsl_lpit.h"
#include "fsl_edma.h"
#include "fsl_common.h"
#include "fsl_sar_adc.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool timeOut                   = false;
volatile uint32_t conversionCount       = 0U;
AT_NONCACHEABLE_SECTION_ALIGN_INIT(uint32_t destAddr[1200], 32U) = {0x00U};
adc_channel_config_t adcChannelConfig[1] = {
    {
        .channelIndex      = DEMO_ADC_CHANNEL0,
        .enableInt         = false,
        .enablePresample   = false,
        .enableDmaTransfer = true,
        .enableWdg         = false,
        .wdgIndex          = 0U,
    },
};

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Timer IRQ handler
 */
void DEMO_LPIT_IRQ_HANDLER_FUNC(void)
{
    ADC_StopConvChain(DEMO_ADC_BASE);
    ADC_DisableDmaTransfer(DEMO_ADC_BASE);

    EDMA_DisableChannelRequest(DEMO_DMA_BASE, DEMO_DMA_CHANNEL);

    LPIT_StopTimer(DEMO_LPIT_BASE, DEMO_LPIT_CHANNEL);
    DisableIRQ(DEMO_LPIT_IRQn);
    LPIT_ClearStatusFlags(DEMO_LPIT_BASE, kLPIT_Channel0TimerFlag);

    timeOut = true;
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief ADC configuration function
 */
static void DEMO_AdcConfig(void)
{
    adc_config_t adcConfig;
    adc_chain_config_t chainConfig;
    adc_calibration_config_t calibrationConfig;

    /* Configure the ADC. */
    ADC_GetDefaultConfig(&adcConfig);
    adcConfig.enableOverWrite = false;
    adcConfig.dmaRequestClearSrc = kADC_DMARequestClearByAck;
    adcConfig.samplePhaseDuration[0U] = 0x16U;
    ADC_Init(DEMO_ADC_BASE, &adcConfig);

    ADC_SetPowerDownMode(DEMO_ADC_BASE, false);
    while (ADC_GetAdcState(DEMO_ADC_BASE) != kADC_AdcIdle);

    /* Configure the ADC conversion chain. */
    chainConfig.convMode                      = kADC_NormalConvScanMode;
    chainConfig.enableGlobalChannelConvEndInt = false;
    chainConfig.enableChainConvEndInt         = false;
    chainConfig.channelCount                  = DEMO_ADC_USED_CHANNELS;
    chainConfig.channelConfig                 = &(adcChannelConfig[0]);
    ADC_SetConvChainConfig(DEMO_ADC_BASE, &chainConfig);
      
    /* Do calibration to reduce or eliminate the various error contribution effects. */
    calibrationConfig.enableAverage        = true;
    calibrationConfig.sampleTime           = kADC_SampleTime32;
    calibrationConfig.averageSampleNumbers = kADC_AverageSampleNumbers512;

    ADC_SetPowerDownMode(DEMO_ADC_BASE, true);
    while (ADC_GetAdcState(DEMO_ADC_BASE) != kADC_AdcPowerdown);
    ADC_SetOperatingClock(DEMO_ADC_BASE, kADC_HalfBusFrequency);
    ADC_SetPowerDownMode(DEMO_ADC_BASE, false);
    while (ADC_GetAdcState(DEMO_ADC_BASE) != kADC_AdcIdle);

    if (!(ADC_DoCalibration(DEMO_ADC_BASE, &calibrationConfig)))
    {
        PRINTF("\r\n Calibration failed.");
    }

    ADC_SetPowerDownMode(DEMO_ADC_BASE, true);
    while (ADC_GetAdcState(DEMO_ADC_BASE) != kADC_AdcPowerdown);
    ADC_SetOperatingClock(DEMO_ADC_BASE, kADC_FullBusFrequency);
    ADC_SetPowerDownMode(DEMO_ADC_BASE, false);
    while (ADC_GetAdcState(DEMO_ADC_BASE) != kADC_AdcIdle);
}

/*!
 * @brief Timer configuration function
 */
static void DEMO_TimerConfig(void)
{
    lpit_config_t LpitConfig;

    LPIT_GetDefaultConfig(&LpitConfig);
    LPIT_Init(DEMO_LPIT_BASE, &LpitConfig);

    LPIT_SetTimerPeriod(DEMO_LPIT_BASE, DEMO_LPIT_CHANNEL, USEC_TO_COUNT(1000U, DEMO_LPIT_CLOCK_FREQ));
    LPIT_EnableInterrupts(DEMO_LPIT_BASE, kLPIT_Channel0TimerInterruptEnable);
}

/*!
 * @brief Edma configuration function
 */
static void DEMO_EdmaConfig(void)
{
    edma_config_t dmaConfig;
    edma_transfer_config_t transferConfig;

    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(DEMO_DMA_BASE, &dmaConfig);

    void *srcAddr = (uint32_t *)&(DEMO_ADC_BASE->PCDR[0]);
    EDMA_PrepareTransfer(&transferConfig, srcAddr, sizeof(uint32_t), destAddr, sizeof(destAddr[0]),
                         sizeof(destAddr[0]), sizeof(destAddr), kEDMA_PeripheralToMemory);
    transferConfig.dstMajorLoopOffset = (int32_t)((-1) * sizeof(destAddr));
    EDMA_SetTransferConfig(DEMO_DMA_BASE, DEMO_DMA_CHANNEL, &transferConfig, NULL);
    EDMA_EnableAutoStopRequest(DEMO_DMA_BASE, DEMO_DMA_CHANNEL, false);
    
    EDMA_EnableChannelRequest(DEMO_DMA_BASE, DEMO_DMA_CHANNEL);
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    DEMO_AdcConfig();
    DEMO_TimerConfig();

    PRINTF("\r\n ADC sample rate measurement demo.");

    while (1)
    {
        PRINTF("\r\n Please press any key to get the ADC sample rate.");
        (void)GETCHAR();

        DEMO_EdmaConfig();

        ADC_EnableDmaTransfer(DEMO_ADC_BASE);
        ADC_StartConvChain(DEMO_ADC_BASE, kADC_NormalConvScanMode);

        LPIT_StartTimer(DEMO_LPIT_BASE, DEMO_LPIT_CHANNEL);
        EnableIRQ(DEMO_LPIT_IRQn);

        while (1)
        {
            if (timeOut)
            {
                for (uint32_t index = 0U; index < sizeof(destAddr) / sizeof(*destAddr); ++index)
                {
                    if (((destAddr[index] & ADC_CDR_CDATA_MASK) >> ADC_CDR_CDATA_SHIFT) != 0U)
                    {
                        conversionCount++;
                        destAddr[index] = 0U;
                    }
                }
                
                PRINTF("\r\n Adc sample rate: %d sps.", (conversionCount * 1000U));
                conversionCount = 0U;
                timeOut  = false;
                break;
            }
        }
    }
}    
