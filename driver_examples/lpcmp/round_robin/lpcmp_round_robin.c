/*
 * Copyright 2023, 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_lpcmp.h"

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
/*!
 * @brief Main function
 */
int main(void)
{
    lpcmp_config_t mLpcmpConfigStruct;
    lpcmp_dac_config_t mLpcmpDacConfigStruct;
    lpcmp_roundrobin_config_t mLpcmpRoundRobinConfigStruct;

    /* Initialize hardware. */
    BOARD_InitHardware();

    /*
     *   config->enableStopMode        = false;
     *   config->enableOutputPin       = false;
     *   config->useUnfilteredOutput   = false;
     *   config->enableInvertOutput    = false;
     *   config->enableNanoPowerMode   = false;
     *   config->enableHighSpeedMode   = false;
     *   config->hysteresisMode        = kLPCMP_HysteresisLevel0;
     *   config->functionalSourceClock = kLPCMP_FunctionalClockSource0;
     */
    LPCMP_GetDefaultConfig(&mLpcmpConfigStruct);
    
    /* Configure LPCMP input channels. */
#if defined(FSL_FEATURE_LPCMP_HAS_CCR2_INPSEL) && FSL_FEATURE_LPCMP_HAS_CCR2_INPSEL
    mLpcmpConfigStruct.plusInputSrc = kLPCMP_PlusInputSrcDac;
#endif  /* FSL_FEATURE_LPCMP_HAS_CCR2_INPSEL */
#if defined(FSL_FEATURE_LPCMP_HAS_CCR2_INMSEL) && FSL_FEATURE_LPCMP_HAS_CCR2_INMSEL
    mLpcmpConfigStruct.minusInputSrc = kLPCMP_MinusInputSrcMux;
#endif  /* FSL_FEATURE_LPCMP_HAS_CCR2_INMSEL */

    /* Init the LPCMP module. */
    LPCMP_Init(DEMO_LPCMP_BASE, &mLpcmpConfigStruct);

    /* Configure the internal DAC to output half of reference voltage. */
#if !(defined(FSL_FEATURE_LPCMP_HAS_DCR_DAC_HPMD) && (FSL_FEATURE_LPCMP_HAS_DCR_DAC_HPMD == 0U))
    mLpcmpDacConfigStruct.enableLowPowerMode     = false;
#endif /* FSL_FEATURE_LPCMP_HAS_DCR_DAC_HPMD */
    mLpcmpDacConfigStruct.referenceVoltageSource = kLPCMP_VrefSourceVin2;
    mLpcmpDacConfigStruct.DACValue =
        ((LPCMP_DCR_DAC_DATA_MASK >> LPCMP_DCR_DAC_DATA_SHIFT) >> 1U); /* Half of reference voltage. */
    LPCMP_SetDACConfig(DEMO_LPCMP_BASE, &mLpcmpDacConfigStruct);

    /* Configure the roundrobin mode. */
#if !(defined(FSL_FEATURE_LPCMP_HAS_RRCR0_RR_TRG_SEL) && (FSL_FEATURE_LPCMP_HAS_RRCR0_RR_TRG_SEL == 0U))
    mLpcmpRoundRobinConfigStruct.roundrobinTriggerSource = kLPCMP_TriggerSourceInternally;
#endif /* FSL_FEATURE_LPCMP_HAS_RRCR0_RR_TRG_SEL */
    mLpcmpRoundRobinConfigStruct.sampleClockNumbers      = DEMO_LPCMP_ROUND_ROBIN_SAMPLE_CLOCK_NUMBERS;
    mLpcmpRoundRobinConfigStruct.initDelayModules        = DEMO_LPCMP_ROUND_ROBIN_INIT_DELAY_MODULES;
#if !(defined(FSL_FEATURE_LPCMP_HAS_RRCR0_RR_SAMPLE_CNT) && (FSL_FEATURE_LPCMP_HAS_RRCR0_RR_SAMPLE_CNT == 0U))
    mLpcmpRoundRobinConfigStruct.channelSampleNumbers    = 2U;
#endif /* FSL_FEATURE_LPCMP_HAS_RRCR0_RR_SAMPLE_CNT */

    /* The sampleTimeThreshhold can't bigger than channelSampleNumbers. */
#if !(defined(FSL_FEATURE_LPCMP_HAS_RRCR0_RR_SAMPLE_THRESHOLD) && (FSL_FEATURE_LPCMP_HAS_RRCR0_RR_SAMPLE_THRESHOLD == 0U))
    mLpcmpRoundRobinConfigStruct.sampleTimeThreshhold  = 1U;
#endif /* FSL_FEATURE_LPCMP_HAS_RRCR0_RR_SAMPLE_THRESHOLD */
    mLpcmpRoundRobinConfigStruct.fixedMuxPort          = DEMO_LPCMP_ROUND_ROBIN_FIXED_MUX_PORT;
#if !(defined(FSL_FEATURE_LPCMP_HAS_RRCR0_RR_CLK_SEL) && (FSL_FEATURE_LPCMP_HAS_RRCR0_RR_CLK_SEL == 0U))
    mLpcmpRoundRobinConfigStruct.roundrobinClockSource = kLPCMP_RoundRobinClockSource3;
#endif /* FSL_FEATURE_LPCMP_HAS_RRCR0_RR_CLK_SEL */
    mLpcmpRoundRobinConfigStruct.fixedChannel          = DEMO_LPCMP_ROUND_ROBIN_FIXED_CHANNEL;
    mLpcmpRoundRobinConfigStruct.checkerChannelMask    = DEMO_LPCMP_ROUND_ROBIN_CHANNELS_CHECKER_MASK;

    /* Disable roundrobin mode before configure related registers. */
    LPCMP_EnableRoundRobinMode(DEMO_LPCMP_BASE, false);
#if !(defined(FSL_FEATURE_LPCMP_HAS_RRCR2) && (FSL_FEATURE_LPCMP_HAS_RRCR2 == 0U))
    LPCMP_EnableRoundRobinInternalTimer(DEMO_LPCMP_BASE, false);
#endif /* FSL_FEATURE_LPCMP_HAS_RRCR2 */

    LPCMP_SetRoundRobinConfig(DEMO_LPCMP_BASE, &mLpcmpRoundRobinConfigStruct);
#if !(defined(FSL_FEATURE_LPCMP_HAS_RRCR2) && (FSL_FEATURE_LPCMP_HAS_RRCR2 == 0U))
    LPCMP_SetRoundRobinInternalTimer(DEMO_LPCMP_BASE, DEMO_LPCMP_ROUND_ROBIN_INTERAL_TIMER_RATE);
#endif /* FSL_FEATURE_LPCMP_HAS_RRCR2 */
    LPCMP_SetPreSetValue(DEMO_LPCMP_BASE, DEMO_LPCMP_ROUND_ROBIN_CHANNELS_PRE_STATE_MASK);

#if !(defined(FSL_FEATURE_LPCMP_HAS_RRCR2) && (FSL_FEATURE_LPCMP_HAS_RRCR2 == 0U))
    LPCMP_EnableRoundRobinInternalTimer(DEMO_LPCMP_BASE, true);
#endif /* FSL_FEATURE_LPCMP_HAS_RRCR2 */
    LPCMP_EnableRoundRobinMode(DEMO_LPCMP_BASE, true);

    /* Enable the interrupt. */
    LPCMP_EnableInterrupts(DEMO_LPCMP_BASE, kLPCMP_RoundRobinInterruptEnable);
    EnableIRQ(DEMO_LPCMP_IRQ_ID);

    PRINTF("LPCMP RoundRobin Example.\r\n");

    while (1)
    {
    }
}

/*!
 * @brief ISR for LPCMP interrupt function.
 */
void DEMO_LPCMP_IRQ_HANDLER_FUNC(void)
{
    /* Get which channel is changed from pre-set value. */
    for (uint8_t index = 0U; index < 8U; index++)
    {
        if (0x01U == ((LPCMP_GetInputChangedFlags(DEMO_LPCMP_BASE) >> index) & 0x01U))
        {
            PRINTF("channel %d comparison result is different from preset value!\r\n", index);
        }
    }

    LPCMP_ClearStatusFlags(DEMO_LPCMP_BASE, LPCMP_GetStatusFlags(DEMO_LPCMP_BASE));
    LPCMP_ClearInputChangedFlags(DEMO_LPCMP_BASE, LPCMP_GetInputChangedFlags(DEMO_LPCMP_BASE));

    /* reset channel pre-set value, otherwise, the next interrupt can't take place. */
    LPCMP_SetPreSetValue(DEMO_LPCMP_BASE, DEMO_LPCMP_ROUND_ROBIN_CHANNELS_PRE_STATE_MASK);

    SDK_ISR_EXIT_BARRIER;
}
