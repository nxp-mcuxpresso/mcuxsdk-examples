/*
 * Copyright 2019-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "clock_config.h"
#include "fsl_inputmux.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_power.h"
#include "fsl_lpadc.h"
#include "app.h"
#include "fsl_anactrl.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/

/*
 * Porting Guild.
 *
 * To port LPADC Temperature measurement driver example to other platforms, please recode
 * DEMO_MeasureTemperature function based on the actual approximate transfer function of
 * the temperature sensor.
 *
 * If the actual calculation equation is similar to the following equation and the
 * temperature sensor need calibration, please take the DEMO_MeasureTemperature
 * function in this file as a reference.
 *          Temp = A * [alpha * (Vbe8 - Vbe1) / (Vbe8 + alpha * (Vbe8 - Vbe1))] - B.
 * In the above equation:
 *          A    --  Temperature sensor slope.
 *          B    --  Temperature sensor offset.
 */
/*${function:start}*/
/*!
 * brief Measure the temperature.
 */
float DEMO_MeasureTemperature(ADC_Type *base, uint32_t commandId, uint32_t index)
{
    lpadc_conv_result_t convResultStruct;
    uint16_t Vbe1            = 0U;
    uint16_t Vbe8            = 0U;
    uint32_t convResultShift = 0U;
    float parameterSlope     = DEMO_LPADC_TEMP_PARAMETER_A;
    float parameterOffset    = DEMO_LPADC_TEMP_PARAMETER_B;
    float parameterAlpha     = DEMO_LPADC_TEMP_PARAMETER_ALPHA;
    float temperature        = -273.15f; /* Absolute zero degree as the incorrect return value. */

    /* If the temperature sensor need calibration, then read the calibration from the flash. */
    uint32_t temperatureSlopeSolidifyValue  = (*((volatile uint32_t *)(FSL_FEATURE_FLASH_NMPA_TEMP_SLOPE_ADDRS)));
    uint32_t temperatureOffsetSolidifyValue = (*((volatile uint32_t *)(FSL_FEATURE_FLASH_NMPA_TEMP_OFFSET_ADDRS)));

    if (((temperatureSlopeSolidifyValue & 0x1UL) != 0UL) && ((temperatureOffsetSolidifyValue & 0x1UL) != 0UL))
    {
        /* Rejustify slope value and Offset value based on the calibration value. */
        parameterSlope  = ((float)(uint32_t)(temperatureSlopeSolidifyValue >> 1UL) / 1024.0f);
        parameterOffset = ((float)(uint32_t)(temperatureOffsetSolidifyValue >> 1UL) / 1024.0f);
    }

#if defined(FSL_FEATURE_LPADC_TEMP_SENS_BUFFER_SIZE) && (FSL_FEATURE_LPADC_TEMP_SENS_BUFFER_SIZE == 4U)
    /* For best temperature measure performance, the recommended LOOP Count should be 4, but the first two results is
     * useless. */
    /* Drop the useless result. */
    (void)LPADC_GetConvResult(base, &convResultStruct, (uint8_t)index);
    (void)LPADC_GetConvResult(base, &convResultStruct, (uint8_t)index);
#endif /* FSL_FEATURE_LPADC_TEMP_SENS_BUFFER_SIZE */

    /* Read the 2 temperature sensor result. */
    if (true == LPADC_GetConvResult(base, &convResultStruct, (uint8_t)index))
    {
        Vbe1 = convResultStruct.convValue >> convResultShift;
        if (true == LPADC_GetConvResult(base, &convResultStruct, (uint8_t)index))
        {
            Vbe8 = convResultStruct.convValue >> convResultShift;
            /* Final temperature = A*[alpha*(Vbe8-Vbe1)/(Vbe8 + alpha*(Vbe8-Vbe1))] - B. */
            temperature = parameterSlope * (parameterAlpha * ((float)Vbe8 - (float)Vbe1) /
                                            ((float)Vbe8 + parameterAlpha * ((float)Vbe8 - (float)Vbe1))) -
                          parameterOffset;
        }
    }

    return temperature;
}

void BOARD_InitHardware(void)
{
    /* set BOD VBAT level to 1.65V */
    POWER_SetBodVbatLevel(kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false);
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    ANACTRL->DUMMY_CTRL = (ANACTRL->DUMMY_CTRL & (~ANACTRL_DUMMY_CTRL_XO32M_ADC_CLK_MODE_MASK)) |
                          ANACTRL_DUMMY_CTRL_XO32M_ADC_CLK_MODE(0x1);
    CLOCK_SetClkDiv(kCLOCK_DivAdcAsyncClk, 0U, true);  /*!< Reset ADCCLKDIV divider counter and halt it */
    CLOCK_SetClkDiv(kCLOCK_DivAdcAsyncClk, 4U, false); /*!< Set ADCCLKDIV divider to value 4 */
    CLOCK_AttachClk(kEXT_CLK_to_ADC_CLK);

    /* Disable LDOGPADC power down */
    POWER_DisablePD(kPDRUNCFG_PD_LDOGPADC);

    ANACTRL_Init(ANACTRL);
    ANACTRL_EnableVref1V(ANACTRL, true);
}
/*${function:end}*/
