/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_dac14.h"
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
    uint8_t ch;
    uint32_t dacValue;
    dac14_config_t dac14ConfigStruct;

    BOARD_InitHardware();
    PRINTF("\r\nDAC14 Basic Example.\r\n");

    /* Configure the DAC14. */
    DAC14_GetDefaultConfig(&dac14ConfigStruct);
    dac14ConfigStruct.enableOpampBuffer = true;
    dac14ConfigStruct.enableDAC         = true;
    DAC14_Init(DEMO_DAC14_BASEADDR, &dac14ConfigStruct);

    while (1)
    {
        ch       = 0;
        dacValue = 0;
        PRINTF("\r\nPlease input a value (0 - 16383) to output with DAC: ");
        while (ch != 0x0D)
        {
            ch = GETCHAR();
            if ((ch >= '0') && (ch <= '9'))
            {
                PUTCHAR(ch);
                dacValue = dacValue * 10 + (ch - 0x30U);
            }
        }
        PRINTF("\r\nInput value is %d\r\n", dacValue);
        if (dacValue > 0x3FFFU)
        {
            PRINTF("Your value is output of range.\r\n");
            continue;
        }
        DAC14_SetData(DEMO_DAC14_BASEADDR, dacValue);
        PRINTF("DAC out: %d\r\n", dacValue);
    }
}
