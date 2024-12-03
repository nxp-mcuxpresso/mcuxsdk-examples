/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_dac.h"
#include "fsl_ctimer.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "dsp/fast_math_functions.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DEMO_CtimerConfig(ctimer_match_config_t *config);
static void DEMO_DacConfig(void);
static void ctimer_match0_callback(uint32_t flags);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static ctimer_callback_t ctimer_callback_table[] = {
    ctimer_match0_callback, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

/*******************************************************************************
 * Code
 ******************************************************************************/
static void DEMO_CtimerConfig(ctimer_match_config_t *config)
{
    ctimer_config_t ctimerConfig;
    ctimer_match_config_t *ctimerMatchConfig = config;

    CTIMER_GetDefaultConfig(&ctimerConfig);
    CTIMER_Init(DEMO_CTIMER_INSTANCE, &ctimerConfig);

    ctimerMatchConfig->enableCounterReset = true;
    ctimerMatchConfig->enableInterrupt    = true;
    ctimerMatchConfig->enableCounterStop  = false;
    ctimerMatchConfig->matchValue         = 0U;
    ctimerMatchConfig->outControl         = kCTIMER_Output_NoAction;
    ctimerMatchConfig->outPinInitState    = false;

    CTIMER_SetupMatch(DEMO_CTIMER_INSTANCE, kCTIMER_Match_0, ctimerMatchConfig);
}

static void DEMO_DacConfig(void)
{
    dac_config_t dacConfig;

    DAC_GetDefaultConfig(&dacConfig);
    dacConfig.referenceVoltageSource    = DEMO_DAC_VREF;
    DAC_Init(DEMO_DAC_INSTANCE, &dacConfig);
}

static void ctimer_match0_callback(uint32_t flags)
{
    static uint32_t index = 0U;

    if (index >= DEMO_SINEWAVE_POINTS)
    {
        index = 0U;
    }
    
    uint32_t loadVal = (uint32_t)((float)DEMO_DAC_MAX_VAL * (0.5F + 0.5F * arm_sin_f32((float)index * DEMO_PI / 500.0F)));
    
    DAC_SetData(DEMO_DAC_INSTANCE, loadVal);
    index++;
}

/*!
 * @brief Main function
 */
int main(void)
{
    ctimer_match_config_t ctimerMatchConfig;

    BOARD_InitHardware();

    DEMO_DacConfig();
    DEMO_CtimerConfig(&ctimerMatchConfig);

    CTIMER_RegisterCallBack(DEMO_CTIMER_INSTANCE, &ctimer_callback_table[0U], kCTIMER_SingleCallback);

    PRINTF("\r\n Dac sinewave example\r\n");

    do
    {
        uint8_t index = 0U;
        uint32_t frequency = 0U;

        PRINTF("\r\n Please input the sinewave frequency in Hz:");
        while (index != 0x0DU)
        {
            index = GETCHAR();
            if ((index >= '0') && (index <= '9'))
            {
                PUTCHAR(index);
                frequency = frequency * 10U + (index - 0x30U);
            }
        }
        PRINTF("\r\n Dac output sinewave frequency is %d Hz\r\n", frequency);

        ctimerMatchConfig.matchValue = (uint32_t)(DEMO_GET_CTIMER_CLK_FREQ / frequency / DEMO_SINEWAVE_POINTS);
        CTIMER_SetupMatch(DEMO_CTIMER_INSTANCE, kCTIMER_Match_0, &ctimerMatchConfig);

        DAC_Enable(DEMO_DAC_INSTANCE, true);
        CTIMER_StartTimer(DEMO_CTIMER_INSTANCE);
    }while (false);

    while (true);
}
