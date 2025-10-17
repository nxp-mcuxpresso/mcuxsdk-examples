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

    PRINTF("\r\neMIOS center aligned PWM Example");

    EMIOS_GetDefaultConfig(&emiosConfig);

    emiosConfig.prescale = EXAMPLE_EMIOS_CLK_FREQ / EXAMPLE_COUNTERBUS_FREQ - 1U;
    emiosConfig.useGlobalTimeBase = true;
    EMIOS_Init(EXAMPLE_EMIOS, &emiosConfig);

    /* 
     * In OPWMCB mode, PWM period is counter bus Modulus Counter period.
     * Structure member period will be written to register An directly.
     * Convert PWM period in ticks to An value in this example.
     */
    ucMCConfig.period = (EXAMPLE_COUNTERBUS_FREQ / EXAMPLE_PWM_FREQ) / 2U + 1U;
    ucMCConfig.prescale = 0U;
    ucMCConfig.ucMode = kEMIOS_MCB_UpDownCount_Flag;
    ucMCConfig.clockSource = kEMIOS_ClkSrcInternal;
    ucMCConfig.prescalerSource = kEMIOS_Prescaler_PrescaledClock;
    ucMCConfig.counterResetTiming = kEMIOS_ResetMatchEnd;
    ucMCConfig.reloadOutputDelay = 0U;
    ucMCConfig.enableFreeze = false;
    EMIOS_ConfigModulusCounter(EXAMPLE_EMIOS, &ucMCConfig, EXAMPLE_COUNTERBUS_CHANNEL);

    /* 
     * In OPWMCB mode, structure member period is PWM period in ticks, duty cycle is
     * PWM pulse width in ticks.
     * Period in OPWMCB mode do not be written to register An, it just verify if PWM
     * duty cycle is 100.
     */
    ucPWMConfig.period = EXAMPLE_COUNTERBUS_FREQ / EXAMPLE_PWM_FREQ;
    ucPWMConfig.dutyCycle = (EXAMPLE_COUNTERBUS_FREQ / EXAMPLE_PWM_FREQ) / 2U;
    ucPWMConfig.deadTime = 0U;  /* Do not insert dead time in this example. */
    ucPWMConfig.prescale = 0U;
    ucPWMConfig.ucMode = kEMIOS_OPWMCB_TrailEdge_Flag;
    ucPWMConfig.counterBus = kEMIOS_CounterBus_A;
    ucPWMConfig.polarity = kEMIOS_Output_ActiveHigh;
    ucPWMConfig.prescalerSource = kEMIOS_Prescaler_PrescaledClock;
    ucPWMConfig.outputDisableSource = kEMIOS_OutputDisable_Source_None;
    ucPWMConfig.enableFreeze = false;
    EMIOS_ConfigPWM(EXAMPLE_EMIOS, &ucPWMConfig, EXAMPLE_PWM_CHANNEL_0);

    ucPWMConfig.dutyCycle = (EXAMPLE_COUNTERBUS_FREQ / EXAMPLE_PWM_FREQ) / 4U;
    ucPWMConfig.deadTime = 0U;
    EMIOS_ConfigPWM(EXAMPLE_EMIOS, &ucPWMConfig, EXAMPLE_PWM_CHANNEL_1);

    ucPWMConfig.dutyCycle = (EXAMPLE_COUNTERBUS_FREQ / EXAMPLE_PWM_FREQ) / 8U;
    ucPWMConfig.deadTime = 0U;
    EMIOS_ConfigPWM(EXAMPLE_EMIOS, &ucPWMConfig, EXAMPLE_PWM_CHANNEL_2);

    EMIOS_EnableUCPrescaler(EXAMPLE_EMIOS, EXAMPLE_COUNTERBUS_CHANNEL);
    EMIOS_EnableUCPrescaler(EXAMPLE_EMIOS, EXAMPLE_PWM_CHANNEL_0);
    EMIOS_EnableUCPrescaler(EXAMPLE_EMIOS, EXAMPLE_PWM_CHANNEL_1);
    EMIOS_EnableUCPrescaler(EXAMPLE_EMIOS, EXAMPLE_PWM_CHANNEL_2);

    EMIOS_EnableGlobalPrescaler(EXAMPLE_EMIOS);

    while (1)
    {
    }
}
