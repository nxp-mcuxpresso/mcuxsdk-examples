/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_dac.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
uint8_t g_msg[] =
    "DAC demo\r\n"
    "DAC output to control the sound of speaker.\r\n";

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    dac_config_t dacConfigStruct;
    uint32_t dacData;
    uint8_t receiveBuff[8];
    uint8_t i, count;

    /* Init board hardware. */
    BOARD_InitHardware();

    /* Configure the DAC. */
    /*
     * dacConfigStruct.referenceVoltageSource = kDAC_ReferenceVoltageSourceVref2;
     * dacConfigStruct.enableLowPowerMode = false;
     */
    DAC_GetDefaultConfig(&dacConfigStruct);
    DAC_Init(DEMO_DAC_BASEADDR, &dacConfigStruct);
    DAC_Enable(DEMO_DAC_BASEADDR, true); /* Enable output. */
    DAC_SetBufferReadPointer(DEMO_DAC_BASEADDR, 0U);

    PRINTF("%s", g_msg);

    while (1)
    {
        PRINTF("\r\nInput value(0--4095): ");
        i     = 0;
        count = 0;
        while (1)
        {
            receiveBuff[i] = GETCHAR();

            if (receiveBuff[i] == 0x08) /* backspace */
            {
                PUTCHAR(0x08);
                PUTCHAR(0x20);
                PUTCHAR(0x08);
                count--;
                i--;
            }
            else if ((receiveBuff[i] >= '0') && (receiveBuff[i] <= '9'))
            {
                if (count < 4)
                {
                    PUTCHAR(receiveBuff[i]);
                    receiveBuff[i] -= '0';
                    i++;
                    count++;
                }
            }
            else if (receiveBuff[i] == 0x0D) /* Enter */
            {
                break;
            }
            else
            {
            }
        }

        dacData = 0;
        for (i = 0; i < count; i++)
        {
            dacData = dacData * 10 + receiveBuff[i];
        }

        if (dacData > 4095)
        {
            PRINTF("\r\nThe input value is invalid.");
            continue;
        }

        /* DAC output to the speaker */
        DAC_SetBufferValue(DEMO_DAC_BASEADDR, 0U, dacData);
    }
}
