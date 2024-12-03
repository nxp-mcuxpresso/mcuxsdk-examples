/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_lpadc.h"
#include "app.h"
#include "fsl_reset.h"
#include "fsl_debug_console.h"
/*${header:end}*/

extern const uint32_t g_LpadcFullRange;

/*${function:start}*/
/*!
 * brief Measure Temperature sensor.
 */
float DEMO_MeasureTemperature(ADC_Type *base, uint32_t commandId, uint32_t index)
{
    lpadc_conv_result_t convResultStruct;

    float Vtemp              = 0.0f;
    uint32_t convResultShift = 0U;
    float temperature        = -273.15f; /* Absolute zero degree as the incorrect return value. */

    if (true == LPADC_GetConvResult(base, &convResultStruct))
    {
        Vtemp       = (((convResultStruct.convValue >> convResultShift) *
                  ((DEMO_LPADC_VREEFP - DEMO_LPADC_VREFN) * 1000.0f / g_LpadcFullRange)));
        temperature = 500.94832f - (0.6190881f * Vtemp);
    }

    return temperature;
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    CLOCK_SetIpSrc(kCLOCK_Adc1, kCLOCK_Pcc1BusIpSrcCm33Bus);
    RESET_PeripheralReset(kRESET_Adc1);

    if (BOARD_IsLowPowerBootType() != true) /* not low power boot type */
    {
        BOARD_HandshakeWithUboot(); /* Must handshake with uboot, unless will get issues(such as: SoC reset all the
                                       time) */
    }
    else                            /* low power boot type */
    {
        BOARD_SetTrdcGlobalConfig();
    }
}
/*${function:end}*/
