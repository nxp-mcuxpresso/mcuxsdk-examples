/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_common.h"
#include "fsl_cadc.h"
#include "fsl_opamp.h"
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
 * @brief Initialize the OPAMP.
 */
static void OPAMP_Configuration(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_bScanCompleted = false;
uint16_t g_u16ConversionResult = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/
void CADC_ConverterA_End_Scan_IRQHandler(void)
{
    CADC_ClearMiscStatusFlags(DEMO_CADC_BASEADDR, kCADC_ConverterAEndOfScanFlag);
    g_u16ConversionResult = CADC_GetSampleSlotResultValue(DEMO_CADC_BASEADDR, kCADC_SampleSlot0Index);
    g_bScanCompleted      = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize board hardware. */
    BOARD_InitHardware();

    PRINTF("CADC expansion mux aux OPAMP example.\r\n");

    OPAMP_Configuration(); /*Initialize OPAMP.*/
    CADC_Configuration();  /*Initialize CADC.*/

    PRINTF("Press any key to trigger the conversion and toggle the OPAMP gain.\r\n");

    while (1)
    {
        GETCHAR();

        CADC_DoSoftwareTriggerConverter(DEMO_CADC_BASEADDR, kCADC_ConverterA);

        while (!g_bScanCompleted)
        {
        }
        g_bScanCompleted = false;

        PRINTF("ADC Value: %d\r\n", g_u16ConversionResult);
    }
}

/*!
 * @brief Initialize the CADC.
 */
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
    cadcConfigStruct.u32EnabledInterruptMask                      = kCADC_ConversionCompleteInterrupt0Enable;
    cadcConfigStruct.u32ChannelModeMask                           = CDAC_THERMISTOR_CHANNEL_MODE;
    cadcConfigStruct.eSampleSlot[0]                               = CADC_THERMISTOR_CHANNEL_NUMBER;
    cadcConfigStruct.uDisabledSampleSlot.eSequentialModeDisSample = kCADC_Sample1Disabled;
    cadcConfigStruct.sConverterA.bPowerUp                         = true;
    cadcConfigStruct.sConverterA.u16ClockDivisor                  = 10U;

    cadcConfigStruct.sConverterA.muxAuxConfig.expMuxOperateMode = kCADC_ExpMuxScanMode0;
    cadcConfigStruct.sConverterA.muxAuxConfig.disabledMuxSlot   = kCADC_ExpMux3Disable;
    cadcConfigStruct.sConverterA.muxAuxConfig.u32MuxChannelMask =
        kCADC_MuxSel0_Channel3 | kCADC_MuxSel1_Channel3 | kCADC_MuxSel2_Channel3 | kCADC_MuxSel3_Channel3;
    cadcConfigStruct.sConverterA.muxAuxConfig.u16AuxControl =
        kCADC_AuxSel0_Config0 | kCADC_AuxSel1_Config1 | kCADC_AuxSel2_Config2 | kCADC_AuxSel3_Config3;

    CADC_Init(DEMO_CADC_BASEADDR, &cadcConfigStruct);
    EnableIRQWithPriority(DEMO_ADC_CC0_IRQn, 1);
}

/*!
 * @brief Initialize the OPAMP.
 */
static void OPAMP_Configuration(void)
{
    opamp_config_t sConfig;

    /*
     *   psConfig->bEnableLoadCompletionInterrupt = false;
     *   psConfig->bEnableWriteProtection = false;
     *   psConfig->eLoadMode = kOPAMP_LoadModeDelayLoad;
     *   psConfig->ePowerMode = kOPAMP_PowerModeLowPower;
     *   psConfig->eConfigRegSel   = kOPAMP_ConfigRegSelCFG0;
     *
     *   psConfig->sConfigSet0.eWorkMode = kOPAMP_WorkModeBufferMode;
     *   psConfig->sConfigSet0.eNegChannel = kOPAMP_NegChannel0;
     *   psConfig->sConfigSet0.ePosChannel = kOPAMP_PosChannel0;
     *
     *   psConfig->sConfigSet1.eWorkMode = kOPAMP_WorkModeBufferMode;
     *   psConfig->sConfigSet1.eNegChannel = kOPAMP_NegChannel0;
     *   psConfig->sConfigSet1.ePosChannel = kOPAMP_PosChannel0;
     *
     *   psConfig->sConfigSet2.eWorkMode = kOPAMP_WorkModeBufferMode;
     *   psConfig->sConfigSet2.eNegChannel = kOPAMP_NegChannel0;
     *   psConfig->sConfigSet2.ePosChannel = kOPAMP_PosChannel0;
     *
     *   psConfig->sConfigSet3.eWorkMode = kOPAMP_WorkModeBufferMode;
     *   psConfig->sConfigSet3.eNegChannel = kOPAMP_NegChannel0;
     *   psConfig->sConfigSet3.ePosChannel = kOPAMP_PosChannel0;
     */

    OPAMP_GetDefaultConfig(&sConfig);

    sConfig.eConfigRegSel = kOPAMP_ConfigRegSelA1OrA0;
    sConfig.eLoadMode     = kOPAMP_LoadModeImmediatelyLoad;

    sConfig.sConfigSet0.eWorkMode   = kOPAMP_WorkModeBufferMode;
    sConfig.sConfigSet0.ePosChannel = kOPAMP_PosChannel0;

    sConfig.sConfigSet1.eWorkMode   = kOPAMP_WorkModeInternalGain2X;
    sConfig.sConfigSet1.ePosChannel = kOPAMP_PosChannel0;

    sConfig.sConfigSet2.eWorkMode   = kOPAMP_WorkModeInternalGain4X;
    sConfig.sConfigSet2.ePosChannel = kOPAMP_PosChannel0;

    sConfig.sConfigSet3.eWorkMode   = kOPAMP_WorkModeInternalGain8X;
    sConfig.sConfigSet3.ePosChannel = kOPAMP_PosChannel0;

    OPAMP_Init(DEMO_OPAMP, &sConfig);
}
