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
volatile uint32_t count;

/*******************************************************************************
 * Code
 ******************************************************************************/
void EXAMPLE_EMIOS_IRQHandler(void)
{
    uint32_t status;

    status = EMIOS_GetUCStatusFlag(EXAMPLE_EMIOS, EXAMPLE_PWM_CHANNEL);

    if ((status & kEMIOS_EventFlag) != 0U)
    {
        EMIOS_ClearUCStatusFlag(EXAMPLE_EMIOS, kEMIOS_EventFlag, EXAMPLE_PWM_CHANNEL);
        count++;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    emios_config_t emiosConfig;
    emios_uc_pwm_config_t ucPWMConfig;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("\r\neMIOS frequency modulated PWM Example");

    EMIOS_GetDefaultConfig(&emiosConfig);

    emiosConfig.prescale = EXAMPLE_EMIOS_CLK_FREQ / EXAMPLE_COUNTERBUS_FREQ - 1U;
    emiosConfig.useGlobalTimeBase = true;
    EMIOS_Init(EXAMPLE_EMIOS, &emiosConfig);

    /* In OPWFMB mode, PWM period is determined by UC An register. */
    ucPWMConfig.period = EXAMPLE_COUNTERBUS_FREQ / EXAMPLE_PWM_FREQ;
    ucPWMConfig.dutyCycle = (EXAMPLE_COUNTERBUS_FREQ / EXAMPLE_PWM_FREQ) / 4U;
    ucPWMConfig.prescale = 0U;
    ucPWMConfig.ucMode = kEMIOS_OPWFMB_Flag;
    ucPWMConfig.counterBus = kEMIOS_CounterBus_Internal;
    ucPWMConfig.polarity = kEMIOS_Output_ActiveHigh;
    ucPWMConfig.prescalerSource = kEMIOS_Prescaler_PrescaledClock;
    ucPWMConfig.outputDisableSource = kEMIOS_OutputDisable_Source_None;
    ucPWMConfig.enableFreeze = false;
    EMIOS_ConfigPWM(EXAMPLE_EMIOS, &ucPWMConfig, EXAMPLE_PWM_CHANNEL);

    EMIOS_EnableUCInterrupt(EXAMPLE_EMIOS, EXAMPLE_PWM_CHANNEL);
    (void)EnableIRQ(EXAMPLE_EMIOS_IRQn);

    EMIOS_EnableUCPrescaler(EXAMPLE_EMIOS, EXAMPLE_PWM_CHANNEL);

    EMIOS_EnableGlobalPrescaler(EXAMPLE_EMIOS);

    while (1)
    {
        if (ucPWMConfig.period == EXAMPLE_COUNTERBUS_FREQ / EXAMPLE_PWM_FREQ)
        {
            if (count == EXAMPLE_PWM_FREQ * 2U)
            {
                count = 0;
                ucPWMConfig.period = (EXAMPLE_COUNTERBUS_FREQ / EXAMPLE_PWM_FREQ) / 2U;
                ucPWMConfig.dutyCycle = (EXAMPLE_COUNTERBUS_FREQ / EXAMPLE_PWM_FREQ) / 8U;
                EMIOS_UpdatePWM(EXAMPLE_EMIOS, &ucPWMConfig, EXAMPLE_PWM_CHANNEL);
                PRINTF("\r\nUpdate PWM frequency to 20kHz");
            }
        }
        else if (ucPWMConfig.period == (EXAMPLE_COUNTERBUS_FREQ / EXAMPLE_PWM_FREQ) / 2U)
        {
            if (count == EXAMPLE_PWM_FREQ * 4U)
            {
                count = 0;
                ucPWMConfig.period = EXAMPLE_COUNTERBUS_FREQ / EXAMPLE_PWM_FREQ;
                ucPWMConfig.dutyCycle = (EXAMPLE_COUNTERBUS_FREQ / EXAMPLE_PWM_FREQ) / 4U;
                EMIOS_UpdatePWM(EXAMPLE_EMIOS, &ucPWMConfig, EXAMPLE_PWM_CHANNEL);
                PRINTF("\r\nUpdate PWM frequency to 10kHz");
            }
        }
        else
        {
        }
    }
}
