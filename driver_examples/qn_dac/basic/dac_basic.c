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
    uint8_t index;
    dac_config_t config;
    uint32_t dacValue;

    BOARD_InitHardware();
    POWER_DisablePD(kPDRUNCFG_PD_DAC);

    PRINTF("\r\nDAC basic example.\r\n");

    DAC_GetDefaultConfig(&config);
    config.trg_cfg.src  = kDAC_TriggerSelectSoftware;
    config.trg_cfg.mode = kDAC_TriggerModeSingleMode;
    config.trg_cfg.edge = kDAC_TriggerEdgeSelectBothEdge;
    DAC_Init(DEMO_DAC_BASE, &config);
    DAC_Enable(DEMO_DAC_BASE, true);
    while (1)
    {
        index    = 0;
        dacValue = 0;
        PRINTF("\r\nPlease input a value (0 - 255) to output with DAC: ");
        while (index != 0x0D)
        {
            index = GETCHAR();
            if ((index >= '0') && (index <= '9'))
            {
                PUTCHAR(index);
                dacValue = dacValue * 10 + (index - 0x30U);
            }
        }
        PRINTF("\r\nInput value is %d\r\n", dacValue);
        if (dacValue > 0xFFU)
        {
            PRINTF("Your value is output of range.\r\n");
            continue;
        }
        DAC_SetData(DEMO_DAC_BASE, dacValue);
        DAC_DoSoftwareTrigger(DEMO_DAC_BASE);
        PRINTF("DAC out: %d\r\n", dacValue);
        /*
         * The value in the first item would be converted. User can measure the output voltage from DACx_OUT pin.
         */
    }
}
