/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_gpio.h"
#include "fsl_flexpwm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_PWM_FREQUENCY_HZ  (1000U)   /* 1 kHz PWM */
#define DEMO_PWM_DUTY_PERCENT  (30U)     /* 30 % duty for PwmA path (VAL2/VAL3) */
#define DEMO_PHASE_DELAY_US    (2000000U) /* 2 s between phase transitions */

/* Helper macro to calculate two's complement for signed center aligned mode */
#define FLEXPWM_GET_COMPLEMENT_U16(value) ((MCUX_MASK_INVERT_16(value) + 1U) & 0xFFFFU)

#define DEMO_FAULT_MASK (kFLEXPWM_FaultMask_0 | kFLEXPWM_FaultMask_1 | kFLEXPWM_FaultMask_2 | kFLEXPWM_FaultMask_3)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void PWM_InitComplementaryPair(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * Configure PWM0 submodule 0 as a complementary pair:
 *   PWM_A0 at DEMO_PWM_DUTY_PERCENT duty, PWM_B0 as complement with deadtime.
 * The FORCE trigger source is set to local software (forceSource = LocalForce)
 * so that FLEXPWM_SetLocalForceOut() works correctly to apply DTSRCSEL changes.
 */
static void PWM_InitComplementaryPair(void)
{
    flexpwm_submodule_config_t submoduleConfig;
    flexpwm_pwm_config_t pwmConfig;
    flexpwm_fault_config_t faultConfig;
    flexpwm_fault_submodule_config_t faultSubmoduleConfig;
    flexpwm_force_out_config_t forceOutConfig;
    uint32_t srcClkHz    = PWM_SRC_CLK_FREQ;
    uint16_t modValue;
    uint16_t deadTimeVal;

    deadTimeVal = (uint16_t)((((uint64_t)srcClkHz * 650UL) / 1000000000UL) & 0xFFFFU);

    /* Initialize FlexPWM module */
    (void)FLEXPWM_Init(BOARD_PWM_BASEADDR);

    /* Calculate modulo value for signed center-aligned mode */
#ifdef DEMO_PWM_CLOCK_DIVIDER
    modValue = srcClkHz / (1UL << (uint32_t)DEMO_PWM_CLOCK_DIVIDER) / DEMO_PWM_FREQUENCY_HZ;
#else
    modValue = srcClkHz / DEMO_PWM_FREQUENCY_HZ;
#endif

    /* --- Submodule configuration --- */
    FLEXPWM_GetDefaultSubmoduleConfig(&submoduleConfig);
    
#ifdef DEMO_PWM_CLOCK_DIVIDER
    submoduleConfig.prescaler = DEMO_PWM_CLOCK_DIVIDER;
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

    FLEXPWM_ConfigSubmodule(BOARD_PWM_BASEADDR, 0U, &submoduleConfig);

    /* --- Fault configuration (disable all fault disable maps so faults don't affect output) --- */
    FLEXPWM_GetDefaultFaultConfig(&faultConfig);
#ifdef DEMO_PWM_FAULT_LEVEL
    faultConfig.faultInputActiveLevel = DEMO_PWM_FAULT_LEVEL;
#endif
    FLEXPWM_ConfigFaultProtection(BOARD_PWM_BASEADDR, 0U, &faultConfig, DEMO_FAULT_MASK);

#if defined(FSL_FEATURE_PWM_FAULT_CH_COUNT) && (FSL_FEATURE_PWM_FAULT_CH_COUNT > 1)
    FLEXPWM_ConfigFaultProtection(BOARD_PWM_BASEADDR, 1U, &faultConfig, DEMO_FAULT_MASK);
#endif

    FLEXPWM_GetDefaultFaultSubmoduleConfig(&faultSubmoduleConfig);

    /* Disable fault disable mapping so faults do not suppress PWM output */
    faultSubmoduleConfig.pwma.disableMask_ch0 = 0U;
    faultSubmoduleConfig.pwmb.disableMask_ch0 = 0U;
    faultSubmoduleConfig.pwmx.disableMask_ch0 = 0U;

#if defined(FSL_FEATURE_PWM_FAULT_CH_COUNT) && (FSL_FEATURE_PWM_FAULT_CH_COUNT > 1)
    faultSubmoduleConfig.pwma.disableMask_ch1 = 0U;
    faultSubmoduleConfig.pwmb.disableMask_ch1 = 0U;
    faultSubmoduleConfig.pwmx.disableMask_ch1 = 0U;
#endif

    FLEXPWM_ConfigFaultSubmodule(BOARD_PWM_BASEADDR, 0U, &faultSubmoduleConfig);

    /* --- PWM signal configuration --- */
    pwmConfig.complementary = true;
    pwmConfig.ipolSource = kFLEXPWM_IPOL_PWM23;  /* PWM_A drives the complementary pair */

    /* Calculate duty cycle values for center-aligned mode */
    uint16_t dutyValue = ((uint32_t)(modValue / 2U) * DEMO_PWM_DUTY_PERCENT) / 100U;
    pwmConfig.pwma.compareValue_ON = FLEXPWM_GET_COMPLEMENT_U16(dutyValue);
    pwmConfig.pwma.compareValue_OFF = dutyValue;
    pwmConfig.pwma.polarity = kFLEXPWM_Polarity_ActiveHigh;

    /* Polarity needs to be set for the complementary output to work correctly */
    pwmConfig.pwmb.compareValue_ON = 0;
    pwmConfig.pwmb.compareValue_OFF = 0;
    pwmConfig.pwmb.polarity = kFLEXPWM_Polarity_ActiveHigh;

    FLEXPWM_ConfigPWM(BOARD_PWM_BASEADDR, 0U, &pwmConfig);

    /* PWMB compare values are used when PWM_B drives the complementary pair */
    FLEXPWM_SetVAL4(BOARD_PWM_BASEADDR, 0U, FLEXPWM_GET_COMPLEMENT_U16(dutyValue));
    FLEXPWM_SetVAL5(BOARD_PWM_BASEADDR, 0U, dutyValue);

    /* Set dead time */
    FLEXPWM_SetDTCNT0(BOARD_PWM_BASEADDR, 0U, deadTimeVal);
    FLEXPWM_SetDTCNT1(BOARD_PWM_BASEADDR, 0U, deadTimeVal);

    /* --- Force Out configuration --- */
    FLEXPWM_GetDefaultForceOutConfig(&forceOutConfig);
    
    /* Set force source to local software so FLEXPWM_SetLocalForceOut() works */
    forceOutConfig.forceSource = kFLEXPWM_ForceOutputSource_LocalForce;

    /* Start with normal generated PWM for both channels */
    forceOutConfig.pwma.source = kFLEXPWM_ForcePwmSource_Generated;
    forceOutConfig.pwma.softwareValue = false;
    forceOutConfig.pwmb.source = kFLEXPWM_ForcePwmSource_Generated;
    forceOutConfig.pwmb.softwareValue = false;

    FLEXPWM_ConfigForceOut(BOARD_PWM_BASEADDR, 0U, &forceOutConfig);

    /* Enable PWM output */
    FLEXPWM_EnablePWMOutput(BOARD_PWM_BASEADDR, kFLEXPWM_SubmoduleMask_0, kFLEXPWM_SubmoduleMask_0, 0U);

    /* Load all VALx registers and start the counter */
    FLEXPWM_SetLoadOkay(BOARD_PWM_BASEADDR, kFLEXPWM_SubmoduleMask_0);
    FLEXPWM_EnableSubmoduleCounter(BOARD_PWM_BASEADDR, kFLEXPWM_SubmoduleMask_0);
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("\r\n*** eFlexPWM Force Signal Update Example ***\r\n");
    PRINTF("PWM0_A0 / PWM0_B0 complementary pair, %u Hz, %u%% duty\r\n",
           DEMO_PWM_FREQUENCY_HZ, DEMO_PWM_DUTY_PERCENT);
    PRINTF("Observe PWM0_A0 and PWM0_B0 on an oscilloscope.\r\n\r\n");

    PWM_InitComplementaryPair();

    while (1)
    {
        /* ------------------------------------------------------------------ *
         * Phase 0: Normal operation                                           *
         *   DTSRCSEL = Generated (PWM23 feeds deadtime logic)                *
         *   IPOL     = PWM23      (PWM23 drives the complementary pair)      *
         * The initial PWM_InitComplementaryPair() already applied these.      *
         * ------------------------------------------------------------------ */
        PRINTF("Phase 0: Normal - DTSRCSEL=UsePwm, IPOL=PwmA (30%% duty on A0)\r\n");
        SDK_DelayAtLeastUs(DEMO_PHASE_DELAY_US, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* ------------------------------------------------------------------ *
         * Phase 1: Update DTSRCSEL via FORCE_OUT                             *
         *                                                                     *
         * FLEXPWM_SetPWMSource23() writes the new DTSRCSEL value into the    *
         * shadow register. The value does NOT take effect yet.                *
         * FLEXPWM_SetLocalForceOut() then issues a local FORCE_OUT event     *
         * which latches the shadow register into the active register.         *
         *                                                                     *
         * Result: the inverted PWM23 signal is fed into the deadtime logic,  *
         * so the effective duty cycle at the pins flips to ~70%.              *
         * ------------------------------------------------------------------ */
        PRINTF("Phase 1: DTSRCSEL update via FORCE_OUT -> InvertedPwm\r\n");
        FLEXPWM_SetPWMSource23(BOARD_PWM_BASEADDR, 0U, kFLEXPWM_ForcePwmSource_Inverted);
        DEMO_TOGGLE_PHASE_GPIO(); /* toggle before FORCE so the edge marks the exact transition */
        FLEXPWM_SetLocalForceOut(BOARD_PWM_BASEADDR, 0U);
        PRINTF("         DTSRCSEL=InvertedPwm active (observe duty 70%% on A0)\r\n");
        SDK_DelayAtLeastUs(DEMO_PHASE_DELAY_US, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);


        /* ------------------------------------------------------------------ *
         * Phase 2: Atomic DTSRCSEL + IPOL update via a single FORCE_OUT      *
         *                                                                     *
         * Both FLEXPWM_SetPWMSource23() and                                  *
         * FLEXPWM_SetComplementaryPWMSource45() write their respective       *
         * shadow registers without triggering force.                          *
         * A single FLEXPWM_SetLocalForceOut() call then applies both changes *
         * simultaneously in one FORCE_OUT event — a glitch-free atomic       *
         * update.                                                             *
         *                                                                     *
         * Result: DTSRCSEL restored to Generated AND IPOL switched to PWM45  *
         * (PWM45 now drives the complementary pair) atomically.              *
         * ------------------------------------------------------------------ */
        PRINTF("Phase 2: Atomic DTSRCSEL + IPOL update via single FORCE_OUT\r\n");
        /* Write both shadow registers before issuing force — both take effect atomically */
        FLEXPWM_SetPWMSource23(BOARD_PWM_BASEADDR, 0U, kFLEXPWM_ForcePwmSource_Generated);
        FLEXPWM_SetComplementaryPWMSource45(BOARD_PWM_BASEADDR, kFLEXPWM_SubmoduleMask_0);
        DEMO_TOGGLE_PHASE_GPIO(); /* toggle immediately before force so edge marks the transition */
        FLEXPWM_SetLocalForceOut(BOARD_PWM_BASEADDR, 0U);
        PRINTF("         DTSRCSEL=UsePwm, IPOL=PwmB active (30%% duty on A0)\r\n");
        SDK_DelayAtLeastUs(DEMO_PHASE_DELAY_US, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* ------------------------------------------------------------------ *
         * Phase 3: Restore initial state atomically                          *
         *                                                                     *
         * Same atomic technique: write both shadow registers first, then     *
         * call FLEXPWM_SetLocalForceOut() to apply both simultaneously.      *
         * ------------------------------------------------------------------ */
        PRINTF("Phase 3: Restore DTSRCSEL=UsePwm + IPOL=PwmA atomically\r\n");
        FLEXPWM_SetPWMSource23(BOARD_PWM_BASEADDR, 0U, kFLEXPWM_ForcePwmSource_Generated);
        FLEXPWM_SetComplementaryPWMSource23(BOARD_PWM_BASEADDR, kFLEXPWM_SubmoduleMask_0);
        DEMO_TOGGLE_PHASE_GPIO();
        FLEXPWM_SetLocalForceOut(BOARD_PWM_BASEADDR, 0U);
        PRINTF("         Back to initial state (30%% duty on A0)\r\n\r\n");
        SDK_DelayAtLeastUs(DEMO_PHASE_DELAY_US, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    }
}
