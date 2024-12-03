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
 * @brief Initialize the CADC with Errata.
 */
static void CADC_Configuration_Errata(void);

/*!
 * @brief Initialize the OPAMP.
 */
static void OPAMP_Configuration(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_bScanCompleted  = false;
uint16_t g_u16ConversionResult  = 0U;
uint16_t g_u16ConversionResult1 = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/
void CADC_ConverterA_End_Scan_IRQHandler(void)
{
    CADC_ClearMiscStatusFlags(DEMO_CADC_BASEADDR, kCADC_ConverterAEndOfScanFlag);
    g_u16ConversionResult  = CADC_GetSampleSlotResultValue(DEMO_CADC_BASEADDR, kCADC_SampleSlot0Index);
    g_u16ConversionResult1 = CADC_GetSampleSlotResultValue(DEMO_CADC_BASEADDR, kCADC_SampleSlot3Index);
    g_bScanCompleted       = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    unsigned char uch = (uint8_t)'0';
    uint16_t ni       = 0u;

    /* Initialize board hardware. */
    BOARD_InitHardware();

    PRINTF("\r\nCADC expansion mux aux OPAMP example with CADC Errata ERR051396.\r\n");

    while (1)
    {
        PRINTF("Choose CADC setting if with Errata ERR051396:\r\n");
        PRINTF("    0-without workaround (failing to switch mux channel forward);\r\n");
        PRINTF("    1-with workaround (succeed to switch mux channel forward);\r\n");
        PRINTF("    Other inputs will be ignored.\r\n");
        while (1)
        {
            uch = (uint8_t)GETCHAR();
            if (uch == '0' || uch == '1')
            {
                (void)PUTCHAR((int16_t)uch);
                (void)PRINTF("\r\n");
                break;
            }
        }

        OPAMP_Configuration(); /*Initialize OPAMP.*/
        if (uch == '0')
        {
            CADC_Configuration(); /*Initialize CADC.*/
        }
        else if (uch == '1')
        {
            CADC_Configuration_Errata(); /*Initialize CADC with Errata.*/
        }
        else
        {
            /* leave as blank */
        }

        while (1)
        {
            // GETCHAR();
            SDK_DelayAtLeastUs(1000000, 100000000);

            CADC_DoSoftwareTriggerConverter(DEMO_CADC_BASEADDR, kCADC_ConverterA);

            while (!g_bScanCompleted)
            {
            }
            g_bScanCompleted = false;

            PRINTF("ADC Value: %d\r\n", g_u16ConversionResult);
            PRINTF("ADC Value1: %d --------- check if this value stay within small range or not\r\n",
                   g_u16ConversionResult1);

            ni++;
            ni = ni % 16;
            if (ni == 0u)
            {
                CADC_Deinit(DEMO_CADC_BASEADDR);
                OPAMP_Deinit(OPAMPA);
                OPAMP_Deinit(OPAMPB);
                break;
            }
        }
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

    cadcConfigStruct.eScanMode               = kCADC_ScanModeTriggeredSequential;
    cadcConfigStruct.u32EnabledInterruptMask = kCADC_ConversionCompleteInterrupt0Enable;

    cadcConfigStruct.u32ChannelModeMask =
        kCADC_ANB6_InternalTempSensor_ANB7_InternalAnalogInput | kCADC_ANA6_InternalTempSensor_ANA7_InternalAnalogInput;
    cadcConfigStruct.eSampleSlot[0]  = kCADC_SingleEndANA7_DiffANA6pANA7n;
    cadcConfigStruct.eSampleSlot[1]  = kCADC_SingleEndANA0_DiffANA0pANA1n;
    cadcConfigStruct.eSampleSlot[2]  = kCADC_SingleEndANA0_DiffANA0pANA1n;
    cadcConfigStruct.eSampleSlot[3]  = kCADC_SingleEndANB7_DiffANB6pANB7n;
    cadcConfigStruct.eSampleSlot[4]  = kCADC_SingleEndANA0_DiffANA0pANA1n;
    cadcConfigStruct.eSampleSlot[5]  = kCADC_SingleEndANA0_DiffANA0pANA1n;
    cadcConfigStruct.eSampleSlot[6]  = kCADC_SingleEndANB7_DiffANB6pANB7n;
    cadcConfigStruct.eSampleSlot[7]  = kCADC_SingleEndANA0_DiffANA0pANA1n;
    cadcConfigStruct.eSampleSlot[8]  = kCADC_SingleEndANA0_DiffANA0pANA1n;
    cadcConfigStruct.eSampleSlot[9]  = kCADC_SingleEndANA7_DiffANA6pANA7n;
    cadcConfigStruct.eSampleSlot[10] = kCADC_SingleEndANA0_DiffANA0pANA1n;
    cadcConfigStruct.eSampleSlot[11] = kCADC_SingleEndANA0_DiffANA0pANA1n;
    cadcConfigStruct.eSampleSlot[12] = kCADC_SingleEndANB7_DiffANB6pANB7n;
    cadcConfigStruct.eSampleSlot[13] = kCADC_SingleEndANA0_DiffANA0pANA1n;
    cadcConfigStruct.eSampleSlot[14] = kCADC_SingleEndANA0_DiffANA0pANA1n;
    cadcConfigStruct.eSampleSlot[15] = kCADC_SingleEndANA7_DiffANA6pANA7n;
    cadcConfigStruct.uDisabledSampleSlot.eSequentialModeDisSample =
        (cadc_sample_slot_sequential_mode_disabled_t)(kCADC_Sample16Disabled | kCADC_Sample16Disabled);

    cadcConfigStruct.sConverterA.bPowerUp        = true;
    cadcConfigStruct.sConverterA.u16ClockDivisor = 10U;

    cadcConfigStruct.sConverterA.muxAuxConfig.expMuxOperateMode = kCADC_ExpMuxScanMode2;
    cadcConfigStruct.sConverterA.muxAuxConfig.u32MuxChannelMask =
        kCADC_MuxSel0_Channel3 | kCADC_MuxSel1_Channel3 | kCADC_MuxSel2_Channel3 | kCADC_MuxSel3_Channel3;
    cadcConfigStruct.sConverterA.muxAuxConfig.u16AuxControl =
        kCADC_AuxSel0_Config0 | kCADC_AuxSel1_Config1 | kCADC_AuxSel2_Config2 | kCADC_AuxSel3_Config3 |
        kCADC_AuxSel4_Config0 | kCADC_AuxSel5_Config1 | kCADC_AuxSel6_Config2 | kCADC_AuxSel7_Config3;

    cadcConfigStruct.sConverterB.bPowerUp        = true;
    cadcConfigStruct.sConverterB.u16ClockDivisor = 10U;

    cadcConfigStruct.sConverterB.muxAuxConfig.expMuxOperateMode = kCADC_ExpMuxScanMode2;
    cadcConfigStruct.sConverterB.muxAuxConfig.u32MuxChannelMask =
        kCADC_MuxSel0_Channel3 | kCADC_MuxSel1_Channel3 | kCADC_MuxSel2_Channel3 | kCADC_MuxSel3_Channel3;
    cadcConfigStruct.sConverterB.muxAuxConfig.u16AuxControl =
        kCADC_AuxSel0_Config0 | kCADC_AuxSel1_Config1 | kCADC_AuxSel2_Config2 | kCADC_AuxSel3_Config3 |
        kCADC_AuxSel4_Config0 | kCADC_AuxSel5_Config1 | kCADC_AuxSel6_Config2 | kCADC_AuxSel7_Config3;

    CADC_Init(DEMO_CADC_BASEADDR, &cadcConfigStruct);
    EnableIRQWithPriority(DEMO_ADC_CC0_IRQn, 1);
}

/*!
 * @brief Initialize the CADC with Errata.
 */
static void CADC_Configuration_Errata(void)
{
    cadc_config_t cadcConfigStruct;

    /*
     *	 psConfig->eDMATriggerSource = kCADC_DMATrigSrcEndofScan;
     *	 psConfig->eIdleWorkMode = kCADC_IdleKeepNormal;
     *	 psConfig->u16PowerUpDelay = 26U;
     *	 psConfig->u32EnabledInterruptMask = 0U;
     *	 psConfig->eScanMode = kCADC_ScanModeTriggeredParallelSimultaneous;
     *	 psConfig->uDisabledSampleSlot.u32SampleDisVal = 0xFF0F0UL;
     *	 psConfig->uScanControl.u32ScanCtrlVal		   = 0x0UL;
     *	 psConfig->eChannelGain[x] = kCADC_SignalGainX1;
     *	 psConfig->sampleSlotScanInterruptEnableMask = kCADC_NonSampleSlotMask;
     *	 For the default setting of converter, please see CADC_GetConverterDefaultConfig().
     */

    CADC_GetDefaultConfig(&cadcConfigStruct);

    cadcConfigStruct.eScanMode               = kCADC_ScanModeTriggeredSequential;
    cadcConfigStruct.u32EnabledInterruptMask = kCADC_ConversionCompleteInterrupt0Enable;

    cadcConfigStruct.u32ChannelModeMask =
        kCADC_ANB6_InternalTempSensor_ANB7_InternalAnalogInput | kCADC_ANA6_InternalTempSensor_ANA7_InternalAnalogInput;
    cadcConfigStruct.eSampleSlot[0]  = kCADC_SingleEndANA7_DiffANA6pANA7n;
    cadcConfigStruct.eSampleSlot[1]  = kCADC_SingleEndANA0_DiffANA0pANA1n;
    cadcConfigStruct.eSampleSlot[2]  = kCADC_SingleEndANA0_DiffANA0pANA1n;
    cadcConfigStruct.eSampleSlot[3]  = kCADC_SingleEndANB7_DiffANB6pANB7n;
    cadcConfigStruct.eSampleSlot[4]  = kCADC_SingleEndANA0_DiffANA0pANA1n;
    cadcConfigStruct.eSampleSlot[5]  = kCADC_SingleEndANA0_DiffANA0pANA1n;
    cadcConfigStruct.eSampleSlot[6]  = kCADC_SingleEndANB7_DiffANB6pANB7n;
    cadcConfigStruct.eSampleSlot[7]  = kCADC_SingleEndANA0_DiffANA0pANA1n;
    cadcConfigStruct.eSampleSlot[8]  = kCADC_SingleEndANA0_DiffANA0pANA1n;
    cadcConfigStruct.eSampleSlot[9]  = kCADC_SingleEndANA7_DiffANA6pANA7n;
    cadcConfigStruct.eSampleSlot[10] = kCADC_SingleEndANA0_DiffANA0pANA1n;
    cadcConfigStruct.eSampleSlot[11] = kCADC_SingleEndANA0_DiffANA0pANA1n;
    cadcConfigStruct.eSampleSlot[12] = kCADC_SingleEndANB7_DiffANB6pANB7n;
    cadcConfigStruct.eSampleSlot[13] = kCADC_SingleEndANA0_DiffANA0pANA1n;
    cadcConfigStruct.eSampleSlot[14] = kCADC_SingleEndANA0_DiffANA0pANA1n;
    cadcConfigStruct.eSampleSlot[15] = kCADC_SingleEndANA7_DiffANA6pANA7n;
    cadcConfigStruct.uDisabledSampleSlot.eSequentialModeDisSample =
        (cadc_sample_slot_sequential_mode_disabled_t)(kCADC_Sample16Disabled | kCADC_Sample16Disabled);

    // workaround
    cadcConfigStruct.eSampleSlot[16]                              = kCADC_ADCATemperatureSensor;
    cadcConfigStruct.eSampleSlot[17]                              = kCADC_ADCBTemperatureSensor;
    cadcConfigStruct.uDisabledSampleSlot.eSequentialModeDisSample = kCADC_Sample18Disabled;

    cadcConfigStruct.sConverterA.bPowerUp        = true;
    cadcConfigStruct.sConverterA.u16ClockDivisor = 10U;

    cadcConfigStruct.sConverterA.muxAuxConfig.expMuxOperateMode = kCADC_ExpMuxScanMode2;
    cadcConfigStruct.sConverterA.muxAuxConfig.u32MuxChannelMask =
        kCADC_MuxSel0_Channel3 | kCADC_MuxSel1_Channel3 | kCADC_MuxSel2_Channel3 | kCADC_MuxSel3_Channel3;
    cadcConfigStruct.sConverterA.muxAuxConfig.u16AuxControl =
        kCADC_AuxSel0_Config0 | kCADC_AuxSel1_Config1 | kCADC_AuxSel2_Config2 | kCADC_AuxSel3_Config3 |
        kCADC_AuxSel4_Config0 | kCADC_AuxSel5_Config1 | kCADC_AuxSel6_Config2 | kCADC_AuxSel7_Config3;

    cadcConfigStruct.sConverterB.bPowerUp        = true;
    cadcConfigStruct.sConverterB.u16ClockDivisor = 10U;

    cadcConfigStruct.sConverterB.muxAuxConfig.expMuxOperateMode = kCADC_ExpMuxScanMode2;
    cadcConfigStruct.sConverterB.muxAuxConfig.u32MuxChannelMask =
        kCADC_MuxSel0_Channel3 | kCADC_MuxSel1_Channel3 | kCADC_MuxSel2_Channel3 | kCADC_MuxSel3_Channel3;
    cadcConfigStruct.sConverterB.muxAuxConfig.u16AuxControl =
        kCADC_AuxSel0_Config0 | kCADC_AuxSel1_Config1 | kCADC_AuxSel2_Config2 | kCADC_AuxSel3_Config3 |
        kCADC_AuxSel4_Config0 | kCADC_AuxSel5_Config1 | kCADC_AuxSel6_Config2 | kCADC_AuxSel7_Config3;

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

    sConfig.sConfigSet0.ePosChannel = kOPAMP_PosChannel3;
    sConfig.sConfigSet1.ePosChannel = kOPAMP_PosChannel3;
    sConfig.sConfigSet2.ePosChannel = kOPAMP_PosChannel3;
    sConfig.sConfigSet3.ePosChannel = kOPAMP_PosChannel3;
    OPAMP_Init(OPAMPB, &sConfig);
}
