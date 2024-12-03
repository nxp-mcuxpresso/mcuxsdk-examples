/*
 * Copyright 2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_dac.h"

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
    uint32_t dacValue;
    dac_config_t dacConfigStruct;

    BOARD_InitHardware();

    PRINTF("\r\nDAC basic Example.\r\n");

    /* Configure the DAC. */
    DAC_GetDefaultConfig(&dacConfigStruct);
    DAC_Init(DEMO_DAC_BASE, &dacConfigStruct);

    while (1)
    {
        index    = 0;
        dacValue = 0;
        PRINTF("\r\nPlease input a value (0 - 1023) to output with DAC: ");
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
        if (dacValue > 1023U)
        {
            PRINTF("Your value is output of range.\r\n");
            continue;
        }
        DAC_SetBufferValue(DEMO_DAC_BASE, dacValue);
        PRINTF("DAC out: %d\r\n", dacValue);
    }
}
