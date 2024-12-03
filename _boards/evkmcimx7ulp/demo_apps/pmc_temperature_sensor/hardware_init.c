/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_lpadc.h"
#include "app.h"
#include "fsl_pmc0.h"
/*${header:end}*/
#include "fsl_debug_console.h"
extern volatile bool g_LpadcConversionCompletedFlag;
extern volatile uint16_t g_LpadcConvValue;
/*${variable:start}*/
uint32_t tsensorCal = 0UL;
/*${variable:end}*/

/*${function:start}*/
status_t DEMO_TempsenorInit(void)
{
    PMC0_EnableTemperatureSensor(true);
    /* Read TSENS_CAL calibration constant value from OTP Fuse. */
    tsensorCal = OCOTP_CTRL->HW_OCOTP_ANA1;
    return kStatus_Success;
}

float DEMO_GetTempsenorValue(void)
{
    /* TSENSOR[TSENSM] value set. */
    uint8_t tsensmSel[15]     = {0U, 1U, 3U, 2U, 6U, 7U, 5U, 4U, 5U, 7U, 6U, 2U, 3U, 1U, 0U};
    uint16_t tsensorValue[15] = {0U};
    float t, cm_vref, cm_ctat, cm_temp;
    int8_t calibration = 0;
    uint8_t i;

    for (i = 0; i < 15U; i++)
    {
        PMC0_SetTemperatureSensorMode(tsensmSel[i]);
        /* Do ADC convert */
        LPADC_DoSoftwareTrigger(DEMO_LPADC_BASE, 1U); /* 1U is trigger0 mask. */
        while (!g_LpadcConversionCompletedFlag)
        {
        }
        tsensorValue[i]                = g_LpadcConvValue;
        g_LpadcConversionCompletedFlag = false;
    }

    /* Calculate temperature. */
    /* CM_CTAT = (2*C1_001 - C1_011 + 2*C2_001 - C2_011 + 2*C1_101 - C1_111 + 2*C2_101 - C2_111)/4 */
    cm_ctat = (float)(2 * tsensorValue[1] - tsensorValue[2] + 2 * tsensorValue[13] - tsensorValue[12] +
                      2 * tsensorValue[6] - tsensorValue[5] + 2 * tsensorValue[8] - tsensorValue[9]) /
              (float)4.0;
    /* CM_TEMP = (2*C1_000 - C1_010 + 2*C2_000 - C2_010 + 4*CM_100 - C1_110 - C2_110)/4 */
    cm_temp = (float)(2 * tsensorValue[0] - tsensorValue[3] + 2 * tsensorValue[14] - tsensorValue[11] +
                      4 * tsensorValue[7] - tsensorValue[4] - tsensorValue[10]) /
              (float)4.0;
    calibration = (int8_t)(tsensorCal & 0xFF);
    cm_vref     = cm_ctat + (float)(953.36 + calibration) * cm_temp / (float)2048;
    t           = (float)370.98 * (cm_temp / cm_vref) - (float)273.15;
    return t;
}

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Set the source for the ADC1 module */
    SCG0->FIRCDIV &= ~SCG_FIRCDIV_FIRCDIV2_MASK;
    SCG0->FIRCDIV |= SCG_FIRCDIV_FIRCDIV2(2); /* 24MHZ. */
    CLOCK_SetIpSrc(kCLOCK_Adc1, kCLOCK_IpSrcFircAsync);
}
/*${function:end}*/
