/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
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

    /* Init the LPCMP module. */
    LPCMP_Init(DEMO_LPCMP_BASE, &mLpcmpConfigStruct);

    /* Configure the internal DAC to output half of reference voltage. */
    mLpcmpDacConfigStruct.enableLowPowerMode     = false;
    mLpcmpDacConfigStruct.referenceVoltageSource = kLPCMP_VrefSourceVin2;
    mLpcmpDacConfigStruct.DACValue =
        ((LPCMP_DCR_DAC_DATA_MASK >> LPCMP_DCR_DAC_DATA_SHIFT) >> 1U); /* Half of reference voltage. */
    LPCMP_SetDACConfig(DEMO_LPCMP_BASE, &mLpcmpDacConfigStruct);

    /* Configure the roundrobin mode. */
    mLpcmpRoundRobinConfigStruct.roundrobinTriggerSource = kLPCMP_TriggerSourceInternally;
    mLpcmpRoundRobinConfigStruct.sampleClockNumbers      = DEMO_LPCMP_ROUND_ROBIN_SAMPLE_CLOCK_NUMBERS;
    mLpcmpRoundRobinConfigStruct.initDelayModules        = DEMO_LPCMP_ROUND_ROBIN_INIT_DELAY_MODULES;
    mLpcmpRoundRobinConfigStruct.channelSampleNumbers    = 2U;

    /* The sampleTimeThreshhold can't bigger than channelSampleNumbers. */
    mLpcmpRoundRobinConfigStruct.sampleTimeThreshhold  = 1U;
    mLpcmpRoundRobinConfigStruct.fixedMuxPort          = DEMO_LPCMP_ROUND_ROBIN_FIXED_MUX_PORT;
    mLpcmpRoundRobinConfigStruct.roundrobinClockSource = kLPCMP_RoundRobinClockSource3;
    mLpcmpRoundRobinConfigStruct.fixedChannel          = DEMO_LPCMP_ROUND_ROBIN_FIXED_CHANNEL;
    mLpcmpRoundRobinConfigStruct.checkerChannelMask    = DEMO_LPCMP_ROUND_ROBIN_CHANNELS_CHECKER_MASK;

    /* Disable roundrobin mode before configure related registers. */
    LPCMP_EnableRoundRobinMode(DEMO_LPCMP_BASE, false);
    LPCMP_EnableRoundRobinInternalTimer(DEMO_LPCMP_BASE, false);

    LPCMP_SetRoundRobinConfig(DEMO_LPCMP_BASE, &mLpcmpRoundRobinConfigStruct);
    LPCMP_SetRoundRobinInternalTimer(DEMO_LPCMP_BASE, DEMO_LPCMP_ROUND_ROBIN_INTERAL_TIMER_RATE);
    LPCMP_SetPreSetValue(DEMO_LPCMP_BASE, DEMO_LPCMP_ROUND_ROBIN_CHANNELS_PRE_STATE_MASK);

    LPCMP_EnableRoundRobinInternalTimer(DEMO_LPCMP_BASE, true);
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
