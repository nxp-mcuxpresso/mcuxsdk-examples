/*
 * Copyright 2022-2023, 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

#include "fsl_sinc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void DEMO_SINC_IRQ_HANDLER(void)
{
    uint32_t fifoData;
    if ((SINC_GetInterruptStatus(DEMO_SINC) & DEMO_SINC_CONV_COMPLETE_INT_STATUS) != 0UL)
    {
        while (!SINC_CheckChannelResultDataReady(DEMO_SINC, DEMO_SINC_CHANNEL_ID))
        {
        }
        SINC_LatchChannelDebugProceduce(DEMO_SINC, DEMO_SINC_CHANNEL_ID);
        while (!SINC_CheckChannelDebugDataValid(DEMO_SINC, DEMO_SINC_CHANNEL_ID))
        {
        }
        fifoData = SINC_ReadChannelResultData(DEMO_SINC, DEMO_SINC_CHANNEL_ID);
        PRINTF("\r\nAdc Result: %d\r\n", fifoData);
        SINC_ClearInterruptStatus(DEMO_SINC, DEMO_SINC_CONV_COMPLETE_INT_STATUS);
    }
}

int main(void)
{
    sinc_config_t sincConfig;
    sinc_channel_config_t sincChannelConfig;
    sinc_channel_input_option_t sincChannelInputOption;
    sinc_channel_conv_option_t sincChannelConvOption;
    sinc_channel_protection_option_t sincChannelProtectionOption;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
    PRINTF("\r\nSINC ADC Example.\r\n");

    sincChannelInputOption.inputBitFormat = kSINC_InputBit_FormatExternalBitstream;
    sincChannelInputOption.inputBitDelay  = kSINC_InputBit_DelayDisabled;
    sincChannelInputOption.inputBitSource = kSINC_InputBit_SourceExternalBitstream;
    sincChannelInputOption.inputClkEdge   = kSINC_InputClk_EdgePositive;
    sincChannelInputOption.inputClkSource = DEMO_SINC_INPUT_CLK_SOURCE;

    sincChannelConvOption.convMode              = kSINC_ConvMode_Single;
    sincChannelConvOption.convTriggerSource     = kSINC_ConvTrig_SoftPosEdge;
    sincChannelConvOption.enableChPrimaryFilter = true;
    sincChannelConvOption.pfBiasSign            = kSINC_PF_BiasPositive;
    sincChannelConvOption.pfHpfAlphaCoeff       = kSINC_PF_HPFAlphaCoeff0;
    sincChannelConvOption.pfOrder               = kSINC_PF_ThirdOrder;
    sincChannelConvOption.pfShiftDirection      = kSINC_PF_ShiftRight;
    sincChannelConvOption.u16pfOverSampleRatio  = 127U; // The OSR for equation is 128.
    sincChannelConvOption.u32pfBiasValue        = 0U;
    sincChannelConvOption.u8pfShiftBitsNum      = 0U;

    sincChannelProtectionOption.bEnableCadBreakSignal = false;
    sincChannelProtectionOption.bEnableLmtBreakSignal = false;
    sincChannelProtectionOption.bEnableScdBreakSignal = false;
    sincChannelProtectionOption.cadLimitThreshold     = kSINC_Cad_Disabled;
    sincChannelProtectionOption.limitDetectorMode     = kSINC_Lmt_Disabled;
    sincChannelProtectionOption.scdOperateMode        = kSINC_Scd_OperateDisabled;
    sincChannelProtectionOption.scdOption             = kSINC_Scd_DetectRepeating0And1;
    sincChannelProtectionOption.u32HighLimitThreshold = 0XFFFFFFUL;
    sincChannelProtectionOption.u32LowLimitThreshold  = 0x0UL;
    sincChannelProtectionOption.u8ScdLimitThreshold   = 2U;
    sincChannelProtectionOption.zcdOperateMode        = kSINC_ZCD_Disabled;

    sincChannelConfig.bEnableChannel     = true;
    sincChannelConfig.bEnableFifo        = false;
    sincChannelConfig.bEnablePrimaryDma  = false;
    sincChannelConfig.chConvOption       = &sincChannelConvOption;
    sincChannelConfig.chInputOption      = &sincChannelInputOption;
    sincChannelConfig.chProtectionOption = &sincChannelProtectionOption;
    sincChannelConfig.dataFormat         = kSINC_LeftJustifiedSigned;
    sincChannelConfig.u8FifoWaterMark    = 1U;

    SINC_GetDefaultConfig(&sincConfig);

    sincConfig.modClkDivider                          = 8UL; // MCLK0 is 16.5 MHz
    sincConfig.clockPreDivider                        = kSINC_ClkPrescale1;
    sincConfig.channelsConfigArray[DEMO_SINC_CHANNEL] = &sincChannelConfig;
    sincConfig.enableMaster                           = true;
    sincConfig.disableDozeMode                        = false;
    sincConfig.disableModClk0Output                   = DEMO_SINC_DISABLE_MOD_CLK0_OUTPUT;
    sincConfig.disableModClk1Output                   = DEMO_SINC_DISABLE_MOD_CLK1_OUTPUT;
    sincConfig.disableModClk2Output                   = DEMO_SINC_DISABLE_MOD_CLK2_OUTPUT;
    SINC_Init(DEMO_SINC, &sincConfig);
    while (!SINC_CheckChannelReadyForConv(DEMO_SINC, DEMO_SINC_CHANNEL_ID))
    {
    }

    SINC_EnableInterrupts(DEMO_SINC, DEMO_SINC_CONV_COMPLETE_INT_ENABLE);
    EnableIRQ(DEMO_SINC_IRQn);
    SINC_SetChannelDebugOutput(DEMO_SINC, DEMO_SINC_CHANNEL_ID, kSINC_Debug_CicRawData);
    while (1)
    {
        PRINTF("\r\nPress any key to trigger conversion!\r\n");
        GETCHAR();
        SINC_AffirmChannelSoftwareTrigger(DEMO_SINC, (1UL << (uint32_t)DEMO_SINC_CHANNEL_ID));
    }
}
