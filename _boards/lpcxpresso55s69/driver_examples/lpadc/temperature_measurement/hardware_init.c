/*
 * Copyright 2020-2021 NXP
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
 * If the actual calculation equation is similar to the following equation, please take
 * the DEMO_MeasureTemperature function in this file as a reference.
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
    BOARD_BootClockPLL1_150M();
    BOARD_InitDebugConsole();

    /* Maximum advised frequency for temperature sensor measurement is 6MHz whether the source is XTAL or FRO.
     * However, the ADC clock divider value is in the range from 1 to 8, so the PLL0(attached to 16MHZ XTAL)
     * is selected as the ADC clock source with ADC clock divider value as 4.
     */
    /*!< Set up PLL */
    CLOCK_AttachClk(kEXT_CLK_to_PLL0);  /*!< Switch PLL0CLKSEL to EXT_CLK */
    POWER_DisablePD(kPDRUNCFG_PD_PLL0); /* Ensure PLL is on  */
    POWER_DisablePD(kPDRUNCFG_PD_PLL0_SSCG);
    const pll_setup_t pll0Setup = {
        .pllctrl = SYSCON_PLL0CTRL_CLKEN_MASK | SYSCON_PLL0CTRL_SELI(19U) | SYSCON_PLL0CTRL_SELP(9U),
        .pllndec = SYSCON_PLL0NDEC_NDIV(1U),
        .pllpdec = SYSCON_PLL0PDEC_PDIV(16U),
        .pllsscg = {0x0U, (SYSCON_PLL0SSCG1_MDIV_EXT(32U) | SYSCON_PLL0SSCG1_SEL_EXT_MASK)},
        .pllRate = 16000000U,
        .flags   = PLL_SETUPFLAG_WAITLOCK};
    CLOCK_SetPLL0Freq(&pll0Setup);

    CLOCK_SetClkDiv(kCLOCK_DivAdcAsyncClk, 0U, true);  /*!< Reset ADCCLKDIV divider counter and halt it */
    CLOCK_SetClkDiv(kCLOCK_DivAdcAsyncClk, 4U, false); /*!< Set ADCCLKDIV divider to value 4 */

    CLOCK_AttachClk(kPLL0_to_ADC_CLK);                 /*!< Switch ADC_CLK to PLL0 */

    /* Disable LDOGPADC power down */
    POWER_DisablePD(kPDRUNCFG_PD_LDOGPADC);
    /* Disable Temperature sensor power down. */
    POWER_DisablePD(kPDRUNCFG_PD_TEMPSENS);

    ANACTRL_Init(ANACTRL);
    ANACTRL_EnableVref1V(ANACTRL, true);
}
/*${function:end}*/
