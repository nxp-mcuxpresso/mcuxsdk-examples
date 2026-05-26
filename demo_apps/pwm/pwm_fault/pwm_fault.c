/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017, 2022, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_flexpwm.h"
#include "fsl_cmp.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Definition for default PWM frequence in hz. */
#ifndef APP_DEFAULT_PWM_FREQUENCY
#define APP_DEFAULT_PWM_FREQUENCY (12000U)
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Sets up the PWM signals for a PWM submodule */
static void PWM_InitPhasePwm(void);

/* Sets up the PWM fault protection */
static void PWM_SetupFaultPwm(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

static void PWM_InitPhasePwm(void)
{
    flexpwm_submodule_config_t submoduleConfig;
    flexpwm_pwm_config_t pwmConfig;
    uint16_t deadTimeVal;
    uint32_t pwmSourceClockInHz, pwmFrequencyInHz;
    uint16_t modValue;

    pwmSourceClockInHz = PWM_SRC_CLK_FREQ;
    pwmFrequencyInHz = APP_DEFAULT_PWM_FREQUENCY;

    /* Calculate modulo value */
    modValue = (uint16_t)(pwmSourceClockInHz / pwmFrequencyInHz);

    /* Get default submodule configuration */
    FLEXPWM_GetDefaultSubmoduleConfig(&submoduleConfig);

    submoduleConfig.prescaler = kFLEXPWM_Prescale_Divide_1;
    submoduleConfig.clockSource = kFLEXPWM_ClockSource_IPBusClock;
    submoduleConfig.enableDebugMode = true;

    submoduleConfig.counterConfig.initValue = 0U;
    submoduleConfig.counterConfig.modValue = modValue - 1U;
    submoduleConfig.counterConfig.initSource = kFLEXPWM_InitSource_LocalSync;

    submoduleConfig.reloadConfig.loadMode = kFLEXPWM_LoadMode_Opportunity;
    submoduleConfig.reloadConfig.enableFullCycleReload = true;
    submoduleConfig.reloadConfig.enableHalfCycleReload = false;
    submoduleConfig.reloadConfig.loadFrequency = 0U;
    submoduleConfig.reloadConfig.reloadSource = kFLEXPWM_ReloadSource_LocalReload;

    FLEXPWM_ConfigSubmodule(DEMO_PWM_BASEADDR, DEMO_PWM_SUBMODULE, &submoduleConfig);

    /* Configure PWM signal */
    pwmConfig.complementary = false;
    pwmConfig.ipolSource = kFLEXPWM_IPOL_PWM23;

    /* Set 50% duty cycle for PWMA */
    pwmConfig.pwma.compareValue_ON = 0;
    pwmConfig.pwma.compareValue_OFF = modValue / 2U;
    pwmConfig.pwma.polarity = kFLEXPWM_Polarity_ActiveHigh;

    /* PWMB not used */
    pwmConfig.pwmb.compareValue_ON = 0;
    pwmConfig.pwmb.compareValue_OFF = 0;
    pwmConfig.pwmb.polarity = kFLEXPWM_Polarity_ActiveHigh;

    FLEXPWM_ConfigPWM(DEMO_PWM_BASEADDR, DEMO_PWM_SUBMODULE, &pwmConfig);

    /* Set deadtime count */
    deadTimeVal = (uint16_t)((((uint64_t)pwmSourceClockInHz * DEMO_DEADTIME_VAL) / 1000000000UL) & 0xFFFFU);
    FLEXPWM_SetDTCNT0(DEMO_PWM_BASEADDR, DEMO_PWM_SUBMODULE, deadTimeVal);
    FLEXPWM_SetDTCNT1(DEMO_PWM_BASEADDR, DEMO_PWM_SUBMODULE, deadTimeVal);

    /* Set the load okay bit for all submodules to load registers from their buffer */
    FLEXPWM_SetLoadOkay(DEMO_PWM_BASEADDR, DEMO_PWM_SUBMODULE_MASK);
}

static void PWM_SetupFaultPwm(void)
{
    flexpwm_fault_config_t faultConfig;
    flexpwm_fault_submodule_config_t faultSubmoduleConfig;

    /* Get default fault configuration */
    FLEXPWM_GetDefaultFaultConfig(&faultConfig);

    /* Logic 1 on the fault input pin indicates fault */
    faultConfig.faultInputActiveLevel = true;
    /*
     * Automatic fault clearing
     * If use Manual fault clearing mode, then the user must clear fault flags
     */
    faultConfig.faultClearingMode = kFLEXPWM_FaultClearingMode_Automatic;
    faultConfig.enableSafetyMode = false;
    faultConfig.enableFullCycleRecovery = true;
    faultConfig.enableHalfCycleRecovery = false;
    faultConfig.enableCombinatorialPath = false;

    /* Configure fault protection for the fault input */
    FLEXPWM_ConfigFaultProtection(DEMO_PWM_BASEADDR, 0U, &faultConfig, DEMO_PWM_FAULT_INPUT_PIN_MASK);

#if defined(FSL_FEATURE_PWM_FAULT_CH_COUNT) && (FSL_FEATURE_PWM_FAULT_CH_COUNT > 1)
    FLEXPWM_ConfigFaultProtection(DEMO_PWM_BASEADDR, 1U, &faultConfig, DEMO_PWM_FAULT_INPUT_PIN_MASK);
#endif

    /* Get default fault submodule configuration */
    FLEXPWM_GetDefaultFaultSubmoduleConfig(&faultSubmoduleConfig);

    /* Configure PWMA to be disabled by fault input */
    faultSubmoduleConfig.pwma.disableMask_ch0 = DEMO_PWM_FAULT_INPUT_PIN_MASK;
    faultSubmoduleConfig.pwma.outputBehavior = kFLEXPWM_FaultOutput_Force0;

    faultSubmoduleConfig.pwmb.disableMask_ch0 = 0U;
    faultSubmoduleConfig.pwmx.disableMask_ch0 = 0U;

#if defined(FSL_FEATURE_PWM_FAULT_CH_COUNT) && (FSL_FEATURE_PWM_FAULT_CH_COUNT > 1)
    faultSubmoduleConfig.pwma.disableMask_ch1 = 0U;
    faultSubmoduleConfig.pwmb.disableMask_ch1 = 0U;
    faultSubmoduleConfig.pwmx.disableMask_ch1 = 0U;
#endif

    FLEXPWM_ConfigFaultSubmodule(DEMO_PWM_BASEADDR, DEMO_PWM_SUBMODULE, &faultSubmoduleConfig);
}

int main(void)
{
    flexpwm_fault_filter_config_t faultFilterConfig;
    cmp_config_t mCmpConfigStruct;
    cmp_dac_config_t mCmpDacConfigStruct;
    uint32_t pwmVal = 0U;
    uint16_t modValue;

    BOARD_InitHardware();

    /* Calculate modulo value for duty cycle updates */
    modValue = (uint16_t)(PWM_SRC_CLK_FREQ / APP_DEFAULT_PWM_FREQUENCY);

    /*
     * mCmpConfigStruct.enableCmp = true;
     * mCmpConfigStruct.hysteresisMode = kCMP_HysteresisLevel0;
     * mCmpConfigStruct.enableHighSpeed = false;
     * mCmpConfigStruct.enableInvertOutput = false;
     * mCmpConfigStruct.useUnfilteredOutput = false;
     * mCmpConfigStruct.enablePinOut = false;
     * mCmpConfigStruct.enableTriggerMode = false;
     */
    CMP_GetDefaultConfig(&mCmpConfigStruct);
    /* Init the CMP comparator. */
    CMP_Init(DEMO_CMP_BASE, &mCmpConfigStruct);

    /* Configure the DAC channel. */
    mCmpDacConfigStruct.referenceVoltageSource = kCMP_VrefSourceVin2; /* VCC. */
    mCmpDacConfigStruct.DACValue               = 32U;                 /* Half voltage of logic high level. */
    CMP_SetDACConfig(DEMO_CMP_BASE, &mCmpDacConfigStruct);
    CMP_SetInputChannels(DEMO_CMP_BASE, DEMO_CMP_USER_CHANNEL, DEMO_CMP_DAC_CHANNEL);

    /* Initialize FlexPWM */
    (void)FLEXPWM_Init(DEMO_PWM_BASEADDR);

    /* Configure fault filter */
    FLEXPWM_GetDefaultFaultFilterConfig(&faultFilterConfig);
    /* Fault filter count */
    faultFilterConfig.filterCount = 0x07U;
    /* Fault filter period; value of 0 will bypass the filter */
    faultFilterConfig.filterPeriod = 0x14U;
    /* Disable fault glitch stretch */
    faultFilterConfig.enableGlitchStretch = false;
    FLEXPWM_ConfigFaultFilter(DEMO_PWM_BASEADDR, 0U, &faultFilterConfig);

#if defined(FSL_FEATURE_PWM_FAULT_CH_COUNT) && (FSL_FEATURE_PWM_FAULT_CH_COUNT > 1)
    FLEXPWM_ConfigFaultFilter(DEMO_PWM_BASEADDR, 1U, &faultFilterConfig);
#endif

    PWM_InitPhasePwm();

    PWM_SetupFaultPwm();

    PRINTF("\r\n\r\nWelcome to PWM Fault demo");
    PRINTF("\r\nUse oscilloscope to see PWM signal at probe pin: %s", DEMO_PWM_CHANNEL_LOCATION_ON_BOARD);
    PRINTF("\r\nConnect pin %s to high level and ground to see change.", DEMO_CMP_INPUT_PIN_LOCATION_ON_BOARD);

    /* Enable PWM output */
    FLEXPWM_EnablePWMOutput(DEMO_PWM_BASEADDR, DEMO_PWM_SUBMODULE_MASK, 0U, 0U);

    /* Start the PWM counter */
    FLEXPWM_EnableSubmoduleCounter(DEMO_PWM_BASEADDR, DEMO_PWM_SUBMODULE_MASK);
    CMP_Enable(DEMO_CMP_BASE, true);

    while (1)
    {
        /* Delay at least 1000 PWM periods. */
        SDK_DelayAtLeastUs((1000000U / APP_DEFAULT_PWM_FREQUENCY) * 1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        pwmVal = pwmVal + 4;

        /* Reset the duty cycle percentage */
        if (pwmVal > 100)
        {
            pwmVal = 0;
        }

        /* 
         * Update duty cycles for PWM signals
         * For edge-aligned mode, calculate VAL3 value for the duty cycle
         * 
         * Special case for 100% duty cycle:
         * VAL3 = modValue (counter end point + 1, never matches)
         * This ensures the output stays high for the entire period
         */
        uint16_t dutyValue;

        if (pwmVal == 100U)
        {
            dutyValue = modValue;  /* Never matches, always high */
        }
        else
        {
            dutyValue = (uint16_t)((modValue * pwmVal) / 100U);
        }
        
        FLEXPWM_SetVAL3(DEMO_PWM_BASEADDR, DEMO_PWM_SUBMODULE, dutyValue);

        /* Set the load okay bit for all submodules to load registers from their buffer */
        FLEXPWM_SetLoadOkay(DEMO_PWM_BASEADDR, DEMO_PWM_SUBMODULE_MASK);
    }
}
