/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_bctu.h"
#include "fsl_sar_adc.h"
#include "fsl_debug_console.h"

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
static void DEMO_AdcConfig(void)
{
    adc_config_t adcConfig;
    adc_calibration_config_t calibrationConfig;

    ADC_GetDefaultConfig(&adcConfig);
    adcConfig.clockFrequency    = DEMO_ADC_CLOCK_FREQ;
    adcConfig.bctuMode          = DEMO_ADC_BCTU_MODE;
    adcConfig.convAvg           = DEMO_ADC_CONV_AVERAGE;
    ADC_Init(DEMO_ADC_BASE, &adcConfig);

    calibrationConfig.enableAverage        = DEMO_ADC_CAL_AVERAGE_ENABLE;
    calibrationConfig.sampleTime           = DEMO_ADC_CAL_SAMPLE_TIME;
    calibrationConfig.averageSampleNumbers = DEMO_ADC_CAL_AVERAGE_SAMPLE_NUM;

    if (!(ADC_DoCalibration(DEMO_ADC_BASE, &calibrationConfig)))
    {
        (void)PRINTF("Calibration failed\r\n");
    }
}

static void DEMO_CountConfig(void)
{
    CLOCK_EnableClock(kCLOCK_Emios0);
  
    /* MODE: Modulus Counter Buffered, Up Count Mode (0x50U), used for channel 1 TIMEBASE.
     * BUS: Internal counter (0x3U)
     * EDPOL: (0x0U)
     * EDSEL: (0x0U)
     * PRESCALE: (0x0U => 0x1U, no prescale)
     * An is used (count period => 16000, 10KHz), ALTAn and Bn is not used.
     */
    EMIOS_0->UC[0U].C = ((EMIOS_0->UC[0U].C & ~(EMIOS_C_MODE_MASK | EMIOS_C_EDPOL_MASK
                        | EMIOS_C_EDSEL_MASK | EMIOS_C_BSL_MASK | EMIOS_C_UCPRE_MASK))
                        | (EMIOS_C_MODE(0x50U) | EMIOS_C_EDPOL(0U) | EMIOS_C_EDSEL(0U)
                        | EMIOS_C_BSL(3U) | EMIOS_C_UCPRE(0U)));

    EMIOS_0->UC[0U].A = ((EMIOS_0->UC[0U].A & ~(EMIOS_A_A_MASK)) | EMIOS_A_A(DEMO_COUNTER_PWM_PERIOD));

    /* MODE: Output Pulse Width Modulation with Trigger (0x26U), used for generate PWM.
     * BUS: Counter bus B from channel 0 (1U)
     * EDPOL: (1U)
     * EDSEL: (0U)
     * PRESCALE: (0x0U => 0x1U, no prescale)
     * AS1 (not buffered) is used for leading edge (Therefore the beginning of the PWM period), can output flip-flop.
     * AS2 (not buffered) is used for trigger point set (accessed by ALTA), can output flip-flop.
     * BS1 (buffered) is used for trailing edge (Therefore the dutycycle of the PWM).
     * CNT not used.
     * FEN: Enabled to set flag when AS2 matched, then generate a trigger request. 
     *      This trigger request is connect to a BCTU trigger input. When BCTU detects the asserted trigger, it routes
     *      that request to the ADCs selected in the corresponding Trigger Configuration (TRGCFG_n[ADC_SELm]). The trigger
     *      remains asserted until the requested conversion begins, at which time BCTU clears the trigger.
     *      10KHz PWM, triggered at the 50% point, that is, starting from 50us, and triggering a conversion every 100us thereafter.
     *
     * AS1:10% period; AS2(ALTA):80% period; BS1:50% period;
     */
    EMIOS_0->UC[1U].C = ((EMIOS_0->UC[1U].C & ~(EMIOS_C_MODE_MASK | EMIOS_C_EDPOL_MASK
                        | EMIOS_C_EDSEL_MASK | EMIOS_C_BSL_MASK | EMIOS_C_UCPRE_MASK | EMIOS_C_FEN_MASK))
                        | (EMIOS_C_MODE(0x26U) | EMIOS_C_EDPOL(0U) | EMIOS_C_EDSEL(0U)
                        | EMIOS_C_BSL(1U) | EMIOS_C_UCPRE(0U) | EMIOS_C_FEN(1U)));
    
    EMIOS_0->UC[1U].A = ((EMIOS_0->UC[1U].A & ~(EMIOS_A_A_MASK)) | EMIOS_A_A((uint16_t)(DEMO_COUNTER_PWM_PERIOD * 0.1F)));
    EMIOS_0->UC[1U].B = ((EMIOS_0->UC[1U].B & ~(EMIOS_B_B_MASK)) | EMIOS_B_B((uint16_t)(DEMO_COUNTER_PWM_PERIOD * 0.8F)));
    EMIOS_0->UC[1U].ALTA = ((EMIOS_0->UC[1U].ALTA & ~(EMIOS_ALTA_ALTA_MASK)) | EMIOS_ALTA_ALTA((uint16_t)(DEMO_COUNTER_PWM_PERIOD * 0.5F)));

    /* Enable EMIOS. */
    EMIOS_0->UC[0U].C |= EMIOS_C_UCPREN_MASK;
    EMIOS_0->UC[1U].C |= EMIOS_C_UCPREN_MASK;
    EMIOS_0->MCR = ((EMIOS_0->MCR & ~(EMIOS_MCR_MDIS_MASK | EMIOS_MCR_GTBE_MASK | EMIOS_MCR_GPREN_MASK | EMIOS_MCR_GPRE_MASK))
                    | (EMIOS_MCR_MDIS(0U) | EMIOS_MCR_GTBE(1U) | EMIOS_MCR_GPREN(1U) | EMIOS_MCR_GPRE(0U)));
}

static void DEMO_BctuConfig(void)
{
    bctu_config_t config;
    bctu_trig_config_t trigConfig;

    BCTU_GetDefaultConfig(&config);
    config.writeProtect = kBCTU_ProtectDis_Permanent;
    BCTU_Init(DEMO_BCTU_BASE, &config);

    trigConfig.trigIndex = (uint8_t)DEMO_BCTU_TRIGGER_INDEX;
    trigConfig.chanAddr = DEMO_BCTU_TRIG_ADC_ADDR;
    trigConfig.dataDest = DEMO_BCTU_DATA_DEST;
    trigConfig.enableLoop = DEMO_BCTU_LOOP_ENABLE;
    trigConfig.targetAdc = (uint32_t)DEMO_BCTU_TRIG_ADC_INSTANCE;
    trigConfig.trigRes = DEMO_BCTU_TRIG_RESOLUTION;
    BCTU_SetTrigConfig(DEMO_BCTU_BASE, &trigConfig);
}

/*!
 * @brief Main function
 */
int main(void)
{
    bctu_adc_conv_result_t result;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    DEMO_AdcConfig();
    DEMO_BctuConfig();
    DEMO_CountConfig();

    BCTU_EnableModule(DEMO_BCTU_BASE, true);
    BCTU_EnableGlobalTrig(DEMO_BCTU_BASE, true);

    (void)PRINTF("BCTU HardWare Trigger Example\r\n");
    (void)PRINTF("Please press any key to get the ADC value\r\n");

    while (true)
    {
        (void)GETCHAR();

        BCTU_EnableHardwareTrig(DEMO_BCTU_BASE, DEMO_BCTU_TRIGGER_INDEX, true);

        while((BCTU_GetStatusFlags(DEMO_BCTU_BASE) & DEMO_BCTU_INT_MASK) != DEMO_BCTU_INT_MASK)
        {
        }

        BCTU_ClearStatusFlags(DEMO_BCTU_BASE, DEMO_BCTU_INT_MASK);
        BCTU_GetConvResult(DEMO_BCTU_BASE, DEMO_BCTU_TRIG_ADC_INSTANCE, &result);

        (void)PRINTF("ADC channel %d value: %d\r\n", result.chanNum, result.data);
    }
}
