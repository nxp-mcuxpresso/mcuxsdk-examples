/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"

#if defined(HAL_CLOCK_H_)
#include "hal_clock.h"
#endif

#if ((defined FSL_FEATURE_SOC_LPIT_COUNT) && (FSL_FEATURE_SOC_LPIT_COUNT != 0))
#include "fsl_lpit.h"
#endif /* FSL_FEATURE_SOC_LPIT_COUNT */

#if ((defined FSL_FEATURE_SOC_PIT_COUNT) && (FSL_FEATURE_SOC_PIT_COUNT != 0))
#include "fsl_pit.h"
#endif /* FSL_FEATURE_SOC_PIT_COUNT */

#if ((defined FSL_FEATURE_SOC_DMAMUX_COUNT) && (FSL_FEATURE_SOC_DMAMUX_COUNT != 0))
#include "fsl_dmamux.h"
#endif /* FSL_FEATURE_SOC_DMAMUX_COUNT */

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
AT_NONCACHEABLE_SECTION_ALIGN_INIT(uint32_t destAddr[DEMO_ADC_RES_ARRAY_SIZE], 32U) = {0x00U};
adc_channel_config_t adcChannelConfig[1] = {
    {
        .channelIndex      = DEMO_ADC_CHANNEL,
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
void DEMO_TIMER_IRQ_HANDLER_FUNC(void)
{
    ADC_StopConvChain(DEMO_ADC_BASE);
    ADC_DisableDmaTransfer(DEMO_ADC_BASE);

    EDMA_DisableChannelRequest(DEMO_DMA_BASE, DEMO_DMA_CHANNEL);

#if ((defined FSL_FEATURE_SOC_LPIT_COUNT) && (FSL_FEATURE_SOC_LPIT_COUNT != 0))
    LPIT_StopTimer(DEMO_LPIT_BASE, DEMO_LPIT_CHANNEL);
    DisableIRQ(DEMO_LPIT_IRQn);
    LPIT_ClearStatusFlags(DEMO_LPIT_BASE, kLPIT_Channel0TimerFlag);
#endif /* FSL_FEATURE_SOC_LPIT_COUNT */
    
#if ((defined FSL_FEATURE_SOC_PIT_COUNT) && (FSL_FEATURE_SOC_PIT_COUNT != 0))
    PIT_StopTimer(DEMO_PIT_BASE, DEMO_PIT_CHANNEL);
    DisableIRQ(DEMO_PIT_IRQn);
    PIT_ClearStatusFlags(DEMO_PIT_BASE, DEMO_PIT_CHANNEL, kPIT_TimerFlag);
#endif /* FSL_FEATURE_SOC_PIT_COUNT */

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
    adcConfig.enableOverWrite           = false;
    adcConfig.clockFrequency            = DEMO_ADC_CLOCK_FREQ;
    adcConfig.dmaRequestClearSrc        = kADC_DMARequestClearByAck;
    adcConfig.samplePhaseDuration[0U]   = 0x16U;
    ADC_Init(DEMO_ADC_BASE, &adcConfig);

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
#if (defined(FSL_FEATURE_ADC_HAS_CALBISTREG) && (FSL_FEATURE_ADC_HAS_CALBISTREG==1U))
    calibrationConfig.averageSampleNumbers = kADC_AverageSampleNumbers32;
#else
    calibrationConfig.averageSampleNumbers = kADC_AverageSampleNumbers512;
#endif /* FSL_FEATURE_ADC_HAS_CALBISTREG */

    if (!(ADC_DoCalibration(DEMO_ADC_BASE, &calibrationConfig)))
    {
        PRINTF("\r\n Calibration failed.");
    }
}

/*!
 * @brief Timer configuration function
 */
static void DEMO_TimerConfig(void)
{
#if ((defined FSL_FEATURE_SOC_LPIT_COUNT) && (FSL_FEATURE_SOC_LPIT_COUNT != 0))
    lpit_config_t LpitConfig;

    LPIT_GetDefaultConfig(&LpitConfig);
    LPIT_Init(DEMO_LPIT_BASE, &LpitConfig);

    LPIT_SetTimerPeriod(DEMO_LPIT_BASE, DEMO_LPIT_CHANNEL, USEC_TO_COUNT(1000U, DEMO_LPIT_CLOCK_FREQ));
    LPIT_EnableInterrupts(DEMO_LPIT_BASE, kLPIT_Channel0TimerInterruptEnable);
#endif /* FSL_FEATURE_SOC_LPIT_COUNT */

#if ((defined FSL_FEATURE_SOC_PIT_COUNT) && (FSL_FEATURE_SOC_PIT_COUNT != 0))
    pit_config_t pitConfig;

    PIT_GetDefaultConfig(&pitConfig);
    PIT_Init(DEMO_PIT_BASE, &pitConfig);

    PIT_SetTimerPeriod(DEMO_PIT_BASE, DEMO_PIT_CHANNEL, USEC_TO_COUNT(1000U, DEMO_PIT_CLOCK_FREQ));
    PIT_EnableInterrupts(DEMO_PIT_BASE, DEMO_PIT_CHANNEL, kPIT_TimerInterruptEnable);
#endif /* FSL_FEATURE_SOC_PIT_COUNT */
}

/*!
 * @brief Edma configuration function
 */
static void DEMO_EdmaConfig(void)
{
#if ((defined FSL_FEATURE_SOC_DMAMUX_COUNT) && (FSL_FEATURE_SOC_DMAMUX_COUNT != 0))
    /* Configure DMAMUX. */
    DMAMUX_Init(DMAMUX_0);
    DMAMUX_SetSource(DMAMUX_0, DEMO_DMA_CHANNEL, kDmaRequestMux0ADC0);
    DMAMUX_EnableChannel(DMAMUX_0, DEMO_DMA_CHANNEL);
#endif /* FSL_FEATURE_SOC_DMAMUX_COUNT */

    edma_config_t dmaConfig;
    edma_transfer_config_t transferConfig;

    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(DEMO_DMA_BASE, &dmaConfig);

    void *srcAddr = (uint32_t *)(&(DEMO_ADC_BASE->PCDR[0]) + DEMO_ADC_CHANNEL);
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

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

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

#if ((defined FSL_FEATURE_SOC_LPIT_COUNT) && (FSL_FEATURE_SOC_LPIT_COUNT != 0))
        LPIT_StartTimer(DEMO_LPIT_BASE, DEMO_LPIT_CHANNEL);
        EnableIRQ(DEMO_LPIT_IRQn);
#endif /* FSL_FEATURE_SOC_LPIT_COUNT */
#if ((defined FSL_FEATURE_SOC_PIT_COUNT) && (FSL_FEATURE_SOC_PIT_COUNT != 0))
        PIT_StartTimer(DEMO_PIT_BASE, DEMO_PIT_CHANNEL);
        EnableIRQ(DEMO_PIT_IRQn);
#endif /* FSL_FEATURE_SOC_PIT_COUNT */
        
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
