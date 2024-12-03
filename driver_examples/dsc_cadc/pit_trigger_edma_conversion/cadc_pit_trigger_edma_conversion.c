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
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "fsl_pit.h"
#include "fsl_xbara.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Initialize the CADC.
 */
static void CADC_Configuration(void);

/*!
 * @brief Process ADC values.
 */
static void ProcessSampleData(void);

/*!
 * @brief Initialize the XBARA.
 */
static void XBARA_Configuration(void);

/*!
 * @brief Initialize the PIT.
 */
static void PIT_Configuration(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_Transfer_Done = false; /* DMA transfer completion flag. */
AT_NONCACHEABLE_SECTION_ALIGN(uint16_t g_cadcSampleDataArray[DEMO_CADC_SAMPLE_COUNT], 2U);
uint32_t g_avgADCValue = 0U; /* Average ADC value .*/

/*******************************************************************************
 * Code
 ******************************************************************************/
void DMA_Channel0_IRQHandler(void)
{
    if ((EDMA_GetChannelStatusFlags(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL_0) &
         kEDMA_ChannelStatusMajorLoopCompleteFlag) != 0U)
    {
        EDMA_ClearChannelStatusFlags(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL_0, kEDMA_ChannelStatusMajorLoopCompleteFlag);
        ProcessSampleData();
        PRINTF("ADC value: %d\r\n", (uint16_t)(g_avgADCValue >> ADC_RSLT_RSLT_SHIFT));
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize board hardware. */
    BOARD_InitHardware();

    PRINTF("CADC PIT trigger EDMA conversion.\r\n");

    XBARA_Configuration();  /* Initialize XBARA. */
    PIT_Configuration();    /* Initialize PIT. */
    DMAMUX_Configuration(); /* Initialize DMAMUX. */
    CADC_Configuration();   /* Initialize CADC. */
    EDMA_Configuration();   /* Initialize EDMA. */

    while (1)
    {
    }
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

    cadcConfigStruct.eScanMode                                    = kCADC_ScanModeTriggeredSequential;
    cadcConfigStruct.eDMATriggerSource                            = kCADC_DMATrigSrcEndofScan;
    cadcConfigStruct.u32ChannelModeMask                           = CDAC_THERMISTOR_CHANNEL_MODE;
    cadcConfigStruct.eSampleSlot[0]                               = CADC_THERMISTOR_CHANNEL_NUMBER;
    cadcConfigStruct.uDisabledSampleSlot.eSequentialModeDisSample = kCADC_Sample1Disabled;
    cadcConfigStruct.sConverterA.bEnableDMA                       = true;
    cadcConfigStruct.sConverterA.bPowerUp                         = true;
    cadcConfigStruct.sConverterA.u16ClockDivisor                  = 9U;

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

static void XBARA_Configuration(void)
{
    /* Init xbara module. */
    XBARA_Init(DEMO_XBARA_BASEADDR);

    /* Configure the XBARA signal connections. */
    XBARA_SetSignalsConnection(DEMO_XBARA_BASEADDR, DEMO_XBARA_USER_CHANNEL_INPUT, DEMO_XBARA_USER_CHANNEL_OUTPUT);
}

static void PIT_Configuration(void)
{
    pit_config_t pitConfig;
    /*
     *    psConfig->ePrescaler = kPIT_PrescalerDivBy1;
     *    psConfig->bEnableInterrupt = false;
     *    psConfig->bEnableSlaveMode = false;
     *    psConfig->bEnableTimer = false;
     *    psConfig->eClockSource = kPIT_CountClockSource0;
     *    psConfig->u16PeriodCount = 0xFFFFU;
     */
    PIT_GetDefaultConfig(&pitConfig);
    pitConfig.eClockSource = PIT_CLOCK_SOURCE;
    pitConfig.ePrescaler   = PIT_CLOCK_SOURCE_DIVIDER;
#if defined(FSL_FEATURE_PIT_32BIT_COUNTER) && FSL_FEATURE_PIT_32BIT_COUNTER
    pitConfig.u32PeriodCount = PIT_PERIOD_COUNT;
#else
    pitConfig.u16PeriodCount = PIT_PERIOD_COUNT;
#endif
    pitConfig.bEnableInterrupt = false;
    pitConfig.bEnableTimer     = true;

    /* Initialize pit module */
    PIT_Init(DEMO_PIT_BASEADDR, &pitConfig);
}
