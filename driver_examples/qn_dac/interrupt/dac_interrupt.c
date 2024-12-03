/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include <stdlib.h>

#include "fsl_debug_console.h"
#include "fsl_dac.h"
#include "fsl_power.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_DAC_VALUE_ARRAY_SIZE 16U
#define DAC_DATA_FACTOR           10
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_DacIndex         = 0U;
volatile uint32_t g_DacInterruptDone = false;
volatile uint32_t g_dac_data         = 0;
/* User-defined for DAC output. */
const uint32_t g_DacValues[DEMO_DAC_VALUE_ARRAY_SIZE] = {1U,
                                                         DAC_DATA_FACTOR * 1U,
                                                         DAC_DATA_FACTOR * 2U,
                                                         DAC_DATA_FACTOR * 3U,
                                                         DAC_DATA_FACTOR * 4U,
                                                         DAC_DATA_FACTOR * 5U,
                                                         DAC_DATA_FACTOR * 6U,
                                                         DAC_DATA_FACTOR * 7U,
                                                         DAC_DATA_FACTOR * 8U,
                                                         DAC_DATA_FACTOR * 9U,
                                                         DAC_DATA_FACTOR * 10U,
                                                         DAC_DATA_FACTOR * 11U,
                                                         DAC_DATA_FACTOR * 12U,
                                                         DAC_DATA_FACTOR * 13U,
                                                         DAC_DATA_FACTOR * 14U,
                                                         DAC_DATA_FACTOR * 15U};
/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    dac_config_t config;

    BOARD_InitHardware();
    POWER_DisablePD(kPDRUNCFG_PD_DAC);
    EnableIRQ(DAC_IRQn); /* Enable interrupt in NVIC. */

    PRINTF("\r\nDAC interrupt example.\r\n");

    /* Configure the DAC. */
    DAC_GetDefaultConfig(&config);
    config.trg_cfg.src  = kDAC_TriggerSelectSoftware;
    config.trg_cfg.mode = kDAC_TriggerModeSingleMode;
    config.trg_cfg.edge = kDAC_TriggerEdgeSelectBothEdge;

    DAC_Init(DEMO_DAC_BASE, &config);
    DAC_Enable(DEMO_DAC_BASE, true);

    PRINTF("Press any key to trigger the DAC...\r\n");
    /* Enable DAC interrupts. */
    DAC_EnableInterrupts(DEMO_DAC_BASE, kDAC_BufferEmptyInterruptEnable);

    while (1)
    {
        PRINTF("DAC output: %d\r\n", g_DacValues[g_DacIndex]);
        g_DacIndex++;
        /* Trigger the buffer. */
        GETCHAR();
        DAC_DoSoftwareTrigger(DEMO_DAC_BASE);

        /* Wait  */
        while (!g_DacInterruptDone)
        {
        }
        g_DacInterruptDone = false;
    }
}

/*!
 * @brief IRQ function for DAC buffer interrupt
 */
void DAC_DriverIRQHandler(void)
{
    uint32_t flags = DAC_GetStatusFlags(DEMO_DAC_BASE);

    if (0U != (kDAC_BufferEmptyFlag & flags))
    {
        if (g_DacIndex > (DEMO_DAC_VALUE_ARRAY_SIZE - 1))
        {
            g_DacIndex = 0U;
        }
        DAC_SetData(DEMO_DAC_BASE, g_DacValues[g_DacIndex]);
    }
    g_DacInterruptDone = true;
    SDK_ISR_EXIT_BARRIER;
}
