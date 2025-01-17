/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_tmu.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile tmu_interrupt_status_t g_tmuInterruptStausStruct;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief TMU ISR.
 */
void DEMO_TMU_IRQ_HANDLER_FUNC(void)
{
    uint32_t temp;

    TMU_GetInterruptStatusFlags(DEMO_TMU_BASE, (tmu_interrupt_status_t *)&g_tmuInterruptStausStruct);
    TMU_ClearInterruptStatusFlags(DEMO_TMU_BASE, g_tmuInterruptStausStruct.interruptDetectMask);

    TMU_GetAverageTemperature(TMU, &temp);
    if (temp < 31U || temp > 146U)
    {
        PRINTF("Temperature out of scope!\r\n");
    }
    else
    {
        PRINTF("Threshold reached, average temperature is %d celsius degree\r\n", temp - 21U);
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    tmu_config_t k_tmuConfig;
    tmu_thresold_config_t k_tmuThresoldConfig;

    BOARD_InitHardware();

    PRINTF("TMU monitor threshold example.\r\n");

    /* Initialize the TMU mode. */
    k_tmuConfig.averageLPF = kTMU_AverageLowPassFilter0_5;
    TMU_Init(DEMO_TMU_BASE, &k_tmuConfig);

    /* Set the temperature threshold. */
    k_tmuThresoldConfig.immediateThresoldEnable       = false;
    k_tmuThresoldConfig.AverageThresoldEnable         = true;
    k_tmuThresoldConfig.AverageCriticalThresoldEnable = false;
    k_tmuThresoldConfig.immediateThresoldValue        = DEMO_TMU_IMMEDIATE_THRESOLD + 21U;
    k_tmuThresoldConfig.averageThresoldValue          = DEMO_TMU_AVERAGE_THRESOLD + 21U;
    k_tmuThresoldConfig.averageCriticalThresoldValue  = DEMO_TMU_AVERAGE_CRITICAL_THRESOLD + 21U;
    TMU_SetHighTemperatureThresold(DEMO_TMU_BASE, &k_tmuThresoldConfig);

    /* Enable the Average temperature threshold exceeded interrupt. */
    TMU_EnableInterrupts(DEMO_TMU_BASE, kTMU_AverageTemperatureInterruptEnable);
    EnableIRQ(DEMO_TMU_IRQ);

    /* Enable the monitor mode. */
    TMU_Enable(DEMO_TMU_BASE, true);

    while (true)
    {
    }
}
