/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_opamp.h"
#include "fsl_sctimer.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
void OPAMP_Configuration(void);
void SCTIMER_Configuration(void);
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*******************************************************************************
 * Variables
 ******************************************************************************/
/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    BOARD_InitHardware();
    OPAMP_Configuration();
    Inputmux_Configuration();
    SCTIMER_Configuration();

    PRINTF("OPAMP TRIGGER MODE EXAMPLE!\r\n");

    while (1)
    {
    }
}

void OPAMP_Configuration(void)
{
    opamp_config_t config;
    /*
     *  config->enable        = false;
     *  config->enablePosADCSw1 = false;
     *  config->mode          = kOPAMP_LowNoiseMode;
     *  config->trimOption    = kOPAMP_TrimOptionDefault;
     *  config->intRefVoltage = kOPAMP_IntRefVoltVddaDiv2;
     *  config->enablePosADCSw1 = false;
     *  config->enablePosADCSw2 = false;
     *  config->posRefVoltage = kOPAMP_PosRefVoltVrefh3;
     *  config->posGain       = kOPAMP_PosGainReserved;
     *  config->negGain       = kOPAMP_NegGainBufferMode;
     *  config->enableRefBuffer = false;
     *  config->PosInputChannelSelection  = kOPAMP_PosInputChannel0
     *  config->enableTriggerMode = false;
     *  config->enableOutputSwitch = true;
     */
    OPAMP_GetDefaultConfig(&config);
    config.posGain = kOPAMP_PosGainNonInvertDisableBuffer2X;
    config.negGain = kOPAMP_NegGainInvert1X;
    config.enable  = true;
    OPAMP_Init(DEMO_OPAMP_BASEADDR, &config);
    OPAMP_EnableTriggerMode(DEMO_OPAMP_BASEADDR, true);
}

void SCTIMER_Configuration(void)
{
    sctimer_config_t sctimerInfo;
    sctimer_pwm_signal_param_t pwmParam;
    uint32_t event;
    uint32_t sctimerClock;

    sctimerClock = SCTIMER_CLK_FREQ;
    SCTIMER_GetDefaultConfig(&sctimerInfo);
    /* Initialize SCTimer module */
    SCTIMER_Init(SCT0, &sctimerInfo);

    /* Configure first PWM with frequency 24kHZ from first output */
    pwmParam.output           = DEMO_FIRST_SCTIMER_OUT;
    pwmParam.level            = kSCTIMER_LowTrue;
    pwmParam.dutyCyclePercent = 10;
    if (SCTIMER_SetupPwm(SCT0, &pwmParam, kSCTIMER_CenterAlignedPwm, 24000U, sctimerClock, &event) == kStatus_Fail)
    {
        return;
    }
    SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_U);
}
