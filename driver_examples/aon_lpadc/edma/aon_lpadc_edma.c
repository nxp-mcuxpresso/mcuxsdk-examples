/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_port.h"
#include "fsl_edma.h"
#include "fsl_aon_lpadc.h"
#include "fsl_common.h"
#include "clock_config.h"
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
AT_NONCACHEABLE_SECTION_ALIGN_INIT(uint32_t destAddr[10U], 32U) = {0x00U};
volatile bool edmaTransDoneFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*! @brief EDMA IRQ handler */
void DEMO_EDMA_IRQ_HANDLER(void)
{
    if ((EDMA_GetChannelStatusFlags(DEMO_EDMA_BASE, DEMO_EDMA_CHANNEL) & kEDMA_InterruptFlag) != 0U)
    {
        edmaTransDoneFlag = true;
        EDMA_ClearChannelStatusFlags(DEMO_EDMA_BASE, DEMO_EDMA_CHANNEL, kEDMA_InterruptFlag);
        EDMA_EnableChannelRequest(DEMO_EDMA_BASE, DEMO_EDMA_CHANNEL);
        
        ADC_Enable(DEMO_ADC_BASE, false);
        ADC_DoFifoRst(DEMO_ADC_BASE, DEMO_ADC_FIFO_INDEX);
    }
}

/*! @brief EDMA configuration */
static void DEMO_DoEdmaConfig(void)
{
    edma_config_t dmaConfig;
    edma_channel_config_t chanConfig;
    edma_transfer_config_t transConfig;

    /* EDMA configuration */
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(DEMO_EDMA_BASE, &dmaConfig);

    /* EDMA channel configuration */
    chanConfig.channelRequestSource = DEMO_EDMA_REQUEST;
    EDMA_InitChannel(DEMO_EDMA_BASE, DEMO_EDMA_CHANNEL, &chanConfig);

    /* EDMA transfer configuration */
    void *srcAddr = (uint32_t *)(&(DEMO_ADC_BASE->RESFIFO0) + DEMO_ADC_FIFO_INDEX);
    EDMA_PrepareTransfer(&transConfig, srcAddr, sizeof(uint32_t), destAddr, sizeof(destAddr[0]),
                         sizeof(destAddr[0]), sizeof(destAddr), kEDMA_PeripheralToMemory);

    transConfig.dstMajorLoopOffset = (int32_t)((-1) * sizeof(destAddr));

    EDMA_SetTransferConfig(DEMO_EDMA_BASE, DEMO_EDMA_CHANNEL, &transConfig, NULL);

    EDMA_EnableAutoStopRequest(DEMO_EDMA_BASE, DEMO_EDMA_CHANNEL, false);
    EDMA_EnableChannelRequest(DEMO_EDMA_BASE, DEMO_EDMA_CHANNEL);

    /* EDMA interrupt */
    EnableIRQ(DEMO_EDMA_IRQ);
}

/*! @brief ADC configuration */
static void DEMO_DoAdcConfig(void)
{
    adc_config_t adcConfig;
    adc_cmd_config_t cmdConfig;
    adc_trig_config_t trigConfig;

    /* ADC configuration. */
    ADC_GetDefaultConfig(&adcConfig);
    adcConfig.dozeModeEn      = DEMO_ADC_DOZE_MODE_EN;
    adcConfig.calConvAvg      = DEMO_ADC_CAL_CONV_AVG;
    adcConfig.calConvSampTime = DEMO_ADC_CAL_CONV_SAMP_TIME;
    adcConfig.refVoltageSrc   = DEMO_ADC_VREF_SOURCE;
    adcConfig.fifo0Watermark  = DEMO_ADC_FIFO0_WATERMARK;
    ADC_Init(DEMO_ADC_BASE, &adcConfig);

    /* ADC calibration. */
    ADC_DoOffsetCal(DEMO_ADC_BASE);

    /* ADC command configration. */
    ADC_GetDefaultCmdConfig(&cmdConfig);
    cmdConfig.waitTrigEn        = DEMO_ADC_WAIT_TRIG_EN;
    cmdConfig.chanIndex         = DEMO_ADC_CHAN_INDEX;
    cmdConfig.convAvg           = DEMO_ADC_CONV_AVG;
    cmdConfig.convSampleTime    = DEMO_ADC_CONV_SAMP_TIME;
    cmdConfig.nextCmdIndex      = DEMO_ADC_CMD_INDEX;
    ADC_SetCmdConfig(DEMO_ADC_BASE, DEMO_ADC_CMD_INDEX, &cmdConfig);

    /* ADC trigger configration. */
    ADC_GetDefaultTrigConfig(&trigConfig);
    trigConfig.cmdIndex = DEMO_ADC_CMD_INDEX;
    ADC_SetTrigConfig(DEMO_ADC_BASE, DEMO_ADC_TRIG_INDEX, &trigConfig);

    ADC_EnableFifoWatermarkDma(DEMO_ADC_BASE, DEMO_ADC_FIFO_INDEX, true);
}

/*! @brief Main function */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("\r\nLpadc edma example");

    DEMO_DoAdcConfig();
    DEMO_DoEdmaConfig();

    while (1)
    {
        PRINTF("\r\nPress any key to get the ADC conversion result");
        GETCHAR();

        ADC_Enable(DEMO_ADC_BASE, true);
        ADC_DoSoftwareTrig(DEMO_ADC_BASE, DEMO_ADC_TRIG_INDEX);

        while (!edmaTransDoneFlag)
        {
        }

        PRINTF("\r\n10 times conversion result:");
        for(uint8_t index = 0U; index < 10U; index++)
        {
            PRINTF("\r\nValue[%d] = %d", index, ((destAddr[index] & LPADC_RESFIFO0_D_MASK) >>
                                                  LPADC_RESFIFO0_D_SHIFT));
        }

        edmaTransDoneFlag = false;
    }
}
