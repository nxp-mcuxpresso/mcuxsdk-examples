/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "board.h"
#include "app.h"
#include "fsl_opamp.h"
#include "fsl_debug_console.h"
#include "fsl_dac.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DAC_VALUE_TO_MVOLT(dacValue) ((float)(dacValue)*DEMO_DAC_VOLT_STEP)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

static void DEMO_InitDAC(void)
{
    dac_config_t dacConfig;

    DAC_GetDefaultConfig(&dacConfig);

    dacConfig.bEnableAnalogPortion = true;
    DAC_Init(DEMO_DAC, &dacConfig);
}

static void DEMO_SetDacValue(uint16_t dacValue)
{
    DAC_WriteDataFIFO(DEMO_DAC, dacValue);
}

/*
 * This example uses 2X internal gain, the DAC voltage is used as input.
 * Then the OPAMP output voltage is two times of DAC voltage. When changing
 * the DAC voltage, the OPAMP output changes accordingly.
 */
int main(void)
{
    int8_t ch;
    uint16_t dacValue = 0;

    BOARD_InitHardware();

    opamp_config_t sConfig;

    PRINTF("OPAMP example started:\r\n");

    DEMO_InitOPAMPWindowCounterClock();

    DEMO_InitDAC();

    DEMO_SetDacValue(0);

    OPAMP_GetDefaultConfig(&sConfig);

    /* Only use configuration set 0. */
    sConfig.eRotateNumber         = kOPAMP_RotateConfigSet0;
    sConfig.sConfigSet0.eWorkMode = kOPAMP_WorkModeInternalGain2X;
    sConfig.sConfigSet0.eVPOS     = kOPAMP_VPOS2;

    OPAMP_Init(DEMO_OPAMP, &sConfig);

    while (1)
    {
        ch       = 0;
        dacValue = 0;

        PRINTF("\r\nPlease input a value (0 - 2000) for DAC: ");

        while (ch != 0x0D)
        {
            ch = GETCHAR();
            if ((ch >= '0') && (ch <= '9'))
            {
                PUTCHAR(ch);
                dacValue = dacValue * 10 + (ch - '0');
            }
        }

        PRINTF("\r\nInput DAC value is %d\r\n", dacValue);
        if (dacValue > 2000)
        {
            PRINTF("DAC value is output of range.\r\n");
            continue;
        }

        /* Change DAC value, then the OPAMP output changes accordingly. */
        DEMO_SetDacValue(dacValue);

        /* OPAMP output is 2x DAC output. */
        PRINTF("Current OPAMP output is about %f mV\r\n", 2.0f * DAC_VALUE_TO_MVOLT(dacValue));
    }
}
