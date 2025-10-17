/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_emios.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_PWM_FREQ    (10000U)    /* PWM frequency 10kHz */

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
    emios_config_t emiosConfig;
    emios_uc_mc_config_t ucMCConfig;
    emios_uc_pwm_config_t ucPWMConfig;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("\r\neMIOS simple PWM Example");

    EMIOS_GetDefaultConfig(&emiosConfig);

    emiosConfig.prescale = EXAMPLE_EMIOS_CLK_FREQ / EXAMPLE_COUNTERBUS_FREQ - 1U;
    emiosConfig.useGlobalTimeBase = true;
    EMIOS_Init(EXAMPLE_EMIOS, &emiosConfig);

    /* In OPWMB mode, PWM period is counter bus Modulus Counter period. */
    ucMCConfig.period = EXAMPLE_COUNTERBUS_FREQ / EXAMPLE_PWM_FREQ;
    ucMCConfig.prescale = 0U;
    ucMCConfig.ucMode = kEMIOS_MCB_UpCount;
    ucMCConfig.clockSource = kEMIOS_ClkSrcInternal;
    ucMCConfig.prescalerSource = kEMIOS_Prescaler_PrescaledClock;
    ucMCConfig.counterResetTiming = kEMIOS_ResetMatchEnd;
    ucMCConfig.reloadOutputDelay = 0U;
    ucMCConfig.enableFreeze = false;
    EMIOS_ConfigModulusCounter(EXAMPLE_EMIOS, &ucMCConfig, EXAMPLE_COUNTERBUS_CHANNEL);

    ucPWMConfig.phaseShift = 0U;
    ucPWMConfig.dutyCycle = (EXAMPLE_COUNTERBUS_FREQ / EXAMPLE_PWM_FREQ) / 4U;
    ucPWMConfig.prescale = 0U;
    ucPWMConfig.ucMode = kEMIOS_OPWMB_Flag;
    ucPWMConfig.counterBus = kEMIOS_CounterBus_A;
    ucPWMConfig.polarity = kEMIOS_Output_ActiveHigh;
    ucPWMConfig.prescalerSource = kEMIOS_Prescaler_PrescaledClock;
    ucPWMConfig.outputDisableSource = kEMIOS_OutputDisable_Source_None;
    ucPWMConfig.enableFreeze = false;
    EMIOS_ConfigPWM(EXAMPLE_EMIOS, &ucPWMConfig, EXAMPLE_PWM_CHANNEL_0);

    EMIOS_EnableUCPrescaler(EXAMPLE_EMIOS, EXAMPLE_COUNTERBUS_CHANNEL);
    EMIOS_EnableUCPrescaler(EXAMPLE_EMIOS, EXAMPLE_PWM_CHANNEL_0);

    EMIOS_EnableGlobalPrescaler(EXAMPLE_EMIOS);

    while (1)
    {
    }
}
