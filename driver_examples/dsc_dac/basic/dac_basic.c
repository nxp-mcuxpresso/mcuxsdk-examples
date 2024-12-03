/*
 * Copyright 2020 NXP
 * All rights reserved.
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
int main(void)
{
    uint8_t u8Input;
    dac_config_t sDacConfig;
    uint16_t u16DacValue;

    BOARD_InitHardware();

    PRINTF("\r\nDAC basic Example.\r\n");

    /* Gets default DAC config. */
    /*
     * psConfig->eOperationMode = kDAC_NormalMode;
     * psConfig->uOperationConfig.sNormalModeConfig.u16BufferedData = 0U;
     * psConfig->uOperationConfig.sNormalModeConfig.bEnableDMA = false;
     * psConfig->uOperationConfig.sNormalModeConfig.eWatermarkLevel = kDAC_WatermarkValue2;
     * psConfig->eSyncInputEdge                                   = kDAC_NoActiveEdge;
     * psConfig->eSpeedMode                                       = kDAC_HighSpeedMode;
     * psConfig->eDataFormat                                      = kDAC_DataWordRightJustified;
     * psConfig->eSyncSignal                                        = kDAC_InternalClockSignal;
     * psConfig->bEnableAnalogPortion                          = false;
     * psConfig->bEnableGlitchFilter                              = true;
     * psConfig->u8GlitchFilterCount                              = 29U;
     */
    DAC_GetDefaultConfig(&sDacConfig);

    sDacConfig.bEnableAnalogPortion = true;
    DAC_Init(DEMO_DAC_BASE, &sDacConfig);

    while (1)
    {
        u8Input     = 0U;
        u16DacValue = 0U;
        PRINTF("\r\nPlease input a value (0 - 4095) to output with DAC: ");
        while (u8Input != 0x0D)
        {
            u8Input = GETCHAR();
            if ((u8Input >= '0') && (u8Input <= '9'))
            {
                PUTCHAR(u8Input);
                u16DacValue = u16DacValue * 10 + (u8Input - 0x30U);
            }
        }
        PRINTF("\r\nInput value is %d\r\n", u16DacValue);
        if (u16DacValue > 0xFFFU)
        {
            PRINTF("Your value is out of range.\r\n");
            continue;
        }
        DAC_WriteDataFIFO(DEMO_DAC_BASE, u16DacValue);
        PRINTF("DAC out: %d\r\n", u16DacValue);
        /*
         * The value in would be converted. User can measure the output voltage from DAC_OUT pin.
         */
    }
}
