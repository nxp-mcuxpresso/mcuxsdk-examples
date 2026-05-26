/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2022, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_flexpwm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Definition for default PWM frequence in hz. */
#ifndef APP_DEFAULT_PWM_FREQUENCY
#define APP_DEFAULT_PWM_FREQUENCY (1000UL)
#endif

#ifndef DEMO_ENABLE_FAULT_TO_PWM_MAPPING
#define DEMO_ENABLE_FAULT_TO_PWM_MAPPING (1)
#endif

/* Helper macro to calculate two's complement for signed center aligned mode */
#define FLEXPWM_GET_COMPLEMENT_U16(value) ((MCUX_MASK_INVERT_16(value) + 1U) & 0xFFFFU)

#define DEMO_SUBMODULE_MASK (kFLEXPWM_SubmoduleMask_0 | kFLEXPWM_SubmoduleMask_1 | kFLEXPWM_SubmoduleMask_2)

#define DEMO_FAULT_MASK (kFLEXPWM_FaultMask_0 | kFLEXPWM_FaultMask_1 | kFLEXPWM_FaultMask_2 | kFLEXPWM_FaultMask_3)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
static void PWM_DRV_Init3PhPwm(uint16_t modValue, uint16_t deadTimeVal)
{
    flexpwm_pwm_config_t pwmConfig;

    /*********** PWMA_SM0 - phase A, configuration, setup complementary PWM ************/
    /* Initialize submodule 0 PWMA PWMB - Complementary Mode */
    pwmConfig.complementary = true;
    pwmConfig.ipolSource = kFLEXPWM_IPOL_PWM23;

    /* 
     * For signed center-aligned mode: 
     * Counter counts from -MOD/2 to +MOD/2
     * For 50% duty cycle, set VAL2/VAL3 symmetrically around zero
     */
    pwmConfig.pwma.compareValue_ON = FLEXPWM_GET_COMPLEMENT_U16(modValue / 4U); /* ~25% of period */
    pwmConfig.pwma.compareValue_OFF = modValue / 4U;                            /* +25% of period */
    pwmConfig.pwma.polarity = kFLEXPWM_Polarity_ActiveHigh;

    /* 
     * PWMB compare values don't matter in complementary mode
     * But polarity still needs to be set for the complementary output to work correctly
     */
    pwmConfig.pwmb.compareValue_ON = 0;
    pwmConfig.pwmb.compareValue_OFF = 0;
    pwmConfig.pwmb.polarity = kFLEXPWM_Polarity_ActiveHigh;

    /*********** PWMA_SM0 - phase A, configuration ************/
    FLEXPWM_ConfigPWM(BOARD_PWM_BASEADDR, 0U, &pwmConfig);
    FLEXPWM_SetDTCNT0(BOARD_PWM_BASEADDR, 0U, deadTimeVal);
    FLEXPWM_SetDTCNT1(BOARD_PWM_BASEADDR, 0U, deadTimeVal);

    /*********** PWMA_SM1 - phase B configuration ************/
    FLEXPWM_ConfigPWM(BOARD_PWM_BASEADDR, 1U, &pwmConfig);
    FLEXPWM_SetDTCNT0(BOARD_PWM_BASEADDR, 1U, deadTimeVal);
    FLEXPWM_SetDTCNT1(BOARD_PWM_BASEADDR, 1U, deadTimeVal);

    /*********** PWMA_SM2 - phase C configuration ************/
    FLEXPWM_ConfigPWM(BOARD_PWM_BASEADDR, 2U, &pwmConfig);
    FLEXPWM_SetDTCNT0(BOARD_PWM_BASEADDR, 2U, deadTimeVal);
    FLEXPWM_SetDTCNT1(BOARD_PWM_BASEADDR, 2U, deadTimeVal);
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Structure of initialize PWM */
    flexpwm_submodule_config_t submoduleConfig;
    flexpwm_fault_config_t faultConfig;
    flexpwm_fault_submodule_config_t faultSubmoduleConfig;
    uint32_t pwmSourceClockInHz;
    uint32_t pwmFrequencyInHz = APP_DEFAULT_PWM_FREQUENCY;
    uint16_t modValue;
    uint16_t deadTimeVal;
    uint32_t pwmVal = 0;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("FlexPWM driver example\r\n");

    /* Initialize FlexPWM module */
    (void)FLEXPWM_Init(BOARD_PWM_BASEADDR);

    /* Get PWM source clock */
    pwmSourceClockInHz = PWM_SRC_CLK_FREQ;

    /*
     * Calculate modulo value for signed center-aligned mode
     * For signed center-aligned: Counter goes from -MOD/2 to +MOD/2
     * So modValue = (clock / prescaler) / frequency
     */
#ifdef DEMO_PWM_CLOCK_DEVIDER
    modValue = pwmSourceClockInHz / (1UL << (uint32_t)DEMO_PWM_CLOCK_DEVIDER) / pwmFrequencyInHz;
#else
    modValue = pwmSourceClockInHz / pwmFrequencyInHz;
#endif

    /* Set deadtime count, we set this to about 650ns */
    deadTimeVal = (uint16_t)((((uint64_t)pwmSourceClockInHz * 650UL) / 1000000000UL) & 0xFFFFU);

    /*
     * Get default submodule configuration:
     * submoduleConfig.prescaler = kFLEXPWM_Prescale_Divide_1;
     * submoduleConfig.clockSource = kFLEXPWM_ClockSource_IPBusClock;
     * submoduleConfig.enableDebugMode = false;
     * submoduleConfig.counterConfig.initValue = 0;
     * submoduleConfig.counterConfig.modValue = 0xFFFFU;
     * submoduleConfig.counterConfig.initSource = kFLEXPWM_InitSource_LocalSync;
     * submoduleConfig.reloadConfig.loadMode = kFLEXPWM_LoadMode_Opportunity;
     * submoduleConfig.reloadConfig.loadFrequency = 0;
     * submoduleConfig.reloadConfig.enableHalfCycleReload = false;
     * submoduleConfig.reloadConfig.enableFullCycleReload = true;
     * submoduleConfig.reloadConfig.reloadSource = kFLEXPWM_ReloadSource_LocalReload;
     * submoduleConfig.reloadConfig.halfCycleValue = 0x7FFFU;
     */
    FLEXPWM_GetDefaultSubmoduleConfig(&submoduleConfig);

#ifdef DEMO_PWM_CLOCK_DEVIDER
    submoduleConfig.prescaler = DEMO_PWM_CLOCK_DEVIDER;
#endif

    submoduleConfig.clockSource = kFLEXPWM_ClockSource_IPBusClock;
    submoduleConfig.enableDebugMode = true;

    /* Configure for signed center-aligned mode */
    submoduleConfig.counterConfig.initValue = FLEXPWM_GET_COMPLEMENT_U16(modValue / 2U);
    submoduleConfig.counterConfig.modValue = modValue / 2U - 1U;
    submoduleConfig.counterConfig.initSource = kFLEXPWM_InitSource_LocalSync;

    /* Use full cycle reload */
    submoduleConfig.reloadConfig.loadMode = kFLEXPWM_LoadMode_Opportunity;
    submoduleConfig.reloadConfig.enableFullCycleReload = true;
    submoduleConfig.reloadConfig.enableHalfCycleReload = false;
    submoduleConfig.reloadConfig.halfCycleValue = 0U;
    submoduleConfig.reloadConfig.loadFrequency = 0U;
    submoduleConfig.reloadConfig.reloadSource = kFLEXPWM_ReloadSource_LocalReload;

    /* Initialize submodule 0 */
    FLEXPWM_ConfigSubmodule(BOARD_PWM_BASEADDR, 0U, &submoduleConfig);

    /* Initialize submodule 1, make it use same counter clock as submodule 0. */
    submoduleConfig.clockSource = kFLEXPWM_ClockSource_Submodule0Clock;
    submoduleConfig.prescaler = kFLEXPWM_Prescale_Divide_1;
    submoduleConfig.counterConfig.initSource = kFLEXPWM_InitSource_MasterSync;
    FLEXPWM_ConfigSubmodule(BOARD_PWM_BASEADDR, 1U, &submoduleConfig);

    /* Initialize submodule 2 the same way as submodule 1 */
    FLEXPWM_ConfigSubmodule(BOARD_PWM_BASEADDR, 2U, &submoduleConfig);

    /*
     * Get default fault configuration:
     * faultConfig.faultInputActiveLevel = true;
     * faultConfig.faultClearingMode = kFLEXPWM_FaultClearMode_Automatic;
     * faultConfig.enableSafetyMode = true;
     * faultConfig.enableFullCycleRecovery = true;
     * faultConfig.enableHalfCycleRecovery = false;
     * faultConfig.enableCombinationalPath = true;
     */
    FLEXPWM_GetDefaultFaultConfig(&faultConfig);

#ifdef DEMO_PWM_FAULT_LEVEL
    faultConfig.faultInputActiveLevel = DEMO_PWM_FAULT_LEVEL;
#else
    faultConfig.faultInputActiveLevel = false;
#endif

    /* Sets up the PWM fault protection */
    FLEXPWM_ConfigFaultProtection(BOARD_PWM_BASEADDR, 0U, &faultConfig, DEMO_FAULT_MASK);

#if defined(FSL_FEATURE_PWM_FAULT_CH_COUNT) && (FSL_FEATURE_PWM_FAULT_CH_COUNT > 1)
    FLEXPWM_ConfigFaultProtection(BOARD_PWM_BASEADDR, 1U, &faultConfig, DEMO_FAULT_MASK);
#endif

    /* Get default fault submodule configuration */
    FLEXPWM_GetDefaultFaultSubmoduleConfig(&faultSubmoduleConfig);

#if defined(DEMO_ENABLE_FAULT_TO_PWM_MAPPING) && (DEMO_ENABLE_FAULT_TO_PWM_MAPPING == 1)
    faultSubmoduleConfig.pwma.disableMask_ch0 = DEMO_FAULT_MASK;
    faultSubmoduleConfig.pwmb.disableMask_ch0 = DEMO_FAULT_MASK;
    faultSubmoduleConfig.pwmx.disableMask_ch0 = DEMO_FAULT_MASK;
#else
    /* Disable fault-to-PWM-output mapping for submodule 0/1/2 */
    faultSubmoduleConfig.pwma.disableMask_ch0 = 0U;
    faultSubmoduleConfig.pwmb.disableMask_ch0 = 0U;
    faultSubmoduleConfig.pwmx.disableMask_ch0 = 0U;
#endif

#if defined(FSL_FEATURE_PWM_FAULT_CH_COUNT) && (FSL_FEATURE_PWM_FAULT_CH_COUNT > 1)
    /* Disable fault-to-PWM-output mapping for submodule 0/1/2 */
    faultSubmoduleConfig.pwma.disableMask_ch1 = 0U;
    faultSubmoduleConfig.pwmb.disableMask_ch1 = 0U;
    faultSubmoduleConfig.pwmx.disableMask_ch1 = 0U;
#endif

    FLEXPWM_ConfigFaultSubmodule(BOARD_PWM_BASEADDR, 0U, &faultSubmoduleConfig);
    FLEXPWM_ConfigFaultSubmodule(BOARD_PWM_BASEADDR, 1U, &faultSubmoduleConfig);
    FLEXPWM_ConfigFaultSubmodule(BOARD_PWM_BASEADDR, 2U, &faultSubmoduleConfig);

    /*
     * Call the init function with demo configuration.
     * Recommend to invoke PWM configuration after submodule and fault configuration,
     * because reference manual advises to set OUTEN register after other PWM configurations.
     * But set OUTEN register before MCTRL register is okay.
     */
    PWM_DRV_Init3PhPwm(modValue, deadTimeVal);

    /* Enable PWM output for all submodules */
    FLEXPWM_EnablePWMOutput(BOARD_PWM_BASEADDR, DEMO_SUBMODULE_MASK, DEMO_SUBMODULE_MASK, 0U);

    /* Set the load okay bit for all submodules to load registers from their buffer */
    FLEXPWM_SetLoadOkay(BOARD_PWM_BASEADDR, DEMO_SUBMODULE_MASK);

    /* Start the PWM generation from Submodules 0, 1 and 2 */
    FLEXPWM_EnableSubmoduleCounter(BOARD_PWM_BASEADDR, DEMO_SUBMODULE_MASK);

    while (1U)
    {
        /* Delay at least 100 PWM periods. */
        SDK_DelayAtLeastUs((1000000U / APP_DEFAULT_PWM_FREQUENCY) * 100, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        pwmVal = pwmVal + 4;

        /* Reset the duty cycle percentage */
        if (pwmVal > 100)
        {
            pwmVal = 0;
        }

        /*
         * Update duty cycles for all 3 PWM signals 
         * For signed center-aligned mode, calculate VAL3 based on duty cycle:
         * VAL3 = (modValue / 2) * (dutyCycle / 100)
         * VAL2 = -VAL3 (two's complement)
         * 
         * Special case for 100% duty cycle (submodule 0 only):
         * VAL2 = initValue (counter start point)
         * VAL3 = modValue / 2 (counter end point + 1, never matches)
         */
        uint16_t val2_sm0, val3_sm0;
        uint16_t val3_sm1 = ((uint32_t)(modValue / 2U) * (pwmVal >> 1U)) / 100U;
        uint16_t val3_sm2 = ((uint32_t)(modValue / 2U) * (pwmVal >> 2U)) / 100U;

        /* Handle 100% duty cycle for submodule 0 */
        if (pwmVal == 100U)
        {
            val2_sm0 = FLEXPWM_GET_COMPLEMENT_U16(modValue / 2U);  /* initValue */
            val3_sm0 = modValue / 2U;                              /* modValue + 1 */
        }
        else
        {
            val3_sm0 = ((uint32_t)(modValue / 2U) * pwmVal) / 100U;
            val2_sm0 = FLEXPWM_GET_COMPLEMENT_U16(val3_sm0);
        }

        /* Update VAL2 (turn-on point) and VAL3 (turn-off point) for each submodule */
        FLEXPWM_SetVAL2(BOARD_PWM_BASEADDR, 0U, val2_sm0);
        FLEXPWM_SetVAL3(BOARD_PWM_BASEADDR, 0U, val3_sm0);

        FLEXPWM_SetVAL2(BOARD_PWM_BASEADDR, 1U, FLEXPWM_GET_COMPLEMENT_U16(val3_sm1));
        FLEXPWM_SetVAL3(BOARD_PWM_BASEADDR, 1U, val3_sm1);

        FLEXPWM_SetVAL2(BOARD_PWM_BASEADDR, 2U, FLEXPWM_GET_COMPLEMENT_U16(val3_sm2));
        FLEXPWM_SetVAL3(BOARD_PWM_BASEADDR, 2U, val3_sm2);

        /* Set the load okay bit for all submodules to load registers from their buffer */
        FLEXPWM_SetLoadOkay(BOARD_PWM_BASEADDR, DEMO_SUBMODULE_MASK);
    }
}
