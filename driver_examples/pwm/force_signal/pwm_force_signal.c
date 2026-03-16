/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_gpio.h"
#include "fsl_pwm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_PWM_FREQUENCY_HZ  (1000U)   /* 1 kHz PWM */
#define DEMO_PWM_DUTY_PERCENT  (30U)     /* 30 % duty for PwmA path (VAL2/VAL3) */
#define DEMO_PHASE_DELAY_US    (2000000U) /* 2 s between phase transitions */

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
 * The FORCE trigger source is set to local software (CTRL2[FORCE_SEL] = 0)
 * so that PWM_EnableLocalForce() and PWM_UpdateCurrentPolarity() work correctly.
 */
static void PWM_InitComplementaryPair(void)
{
    pwm_config_t       pwmConfig;
    pwm_signal_param_t pwmSignal[2];
    pwm_fault_param_t  faultConfig;
    uint32_t           srcClkHz    = PWM_SRC_CLK_FREQ;
    uint16_t           deadTimeVal = (uint16_t)(((uint64_t)srcClkHz * 650U) / 1000000000U);

    /* --- Submodule configuration --- */
    PWM_GetDefaultConfig(&pwmConfig);
    pwmConfig.reloadLogic    = kPWM_ReloadPwmFullCycle;
    pwmConfig.pairOperation  = kPWM_ComplementaryPwmA; /* PWM_A drives the complementary pair */
    pwmConfig.enableDebugMode = true;
    /* forceTrigger stays at default kPWM_Force_Local so software FORCE_OUT works */

#ifdef DEMO_PWM_CLOCK_DIVIDER
    pwmConfig.prescale = DEMO_PWM_CLOCK_DIVIDER;
#endif

    if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_0, &pwmConfig) != kStatus_Success)
    {
        PRINTF("PWM_Init failed\r\n");
        while (1)
        {
        }
    }

    /* --- Fault configuration (disable all fault disable maps so faults don't affect output) --- */
    PWM_FaultDefaultConfig(&faultConfig);
#ifdef DEMO_PWM_FAULT_LEVEL
    faultConfig.faultLevel = DEMO_PWM_FAULT_LEVEL;
#endif
    PWM_SetupFaults(BOARD_PWM_BASEADDR, kPWM_Fault_0, &faultConfig);
    PWM_SetupFaults(BOARD_PWM_BASEADDR, kPWM_Fault_1, &faultConfig);
    PWM_SetupFaults(BOARD_PWM_BASEADDR, kPWM_Fault_2, &faultConfig);
    PWM_SetupFaults(BOARD_PWM_BASEADDR, kPWM_Fault_3, &faultConfig);

    /* Disable fault disable mapping so faults do not suppress PWM output */
    PWM_SetupFaultDisableMap(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmA, kPWM_faultchannel_0, 0U);
    PWM_SetupFaultDisableMap(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmB, kPWM_faultchannel_0, 0U);

    /* --- PWM signal parameters --- */
    pwmSignal[0].pwmChannel       = kPWM_PwmA;
    pwmSignal[0].level            = kPWM_HighTrue;
    pwmSignal[0].dutyCyclePercent = DEMO_PWM_DUTY_PERCENT;
    pwmSignal[0].deadtimeValue    = deadTimeVal;
    pwmSignal[0].faultState       = kPWM_PwmFaultState0;
    pwmSignal[0].pwmchannelenable = true;

    pwmSignal[1].pwmChannel       = kPWM_PwmB;
    pwmSignal[1].level            = kPWM_HighTrue;
    pwmSignal[1].dutyCyclePercent = DEMO_PWM_DUTY_PERCENT;
    pwmSignal[1].deadtimeValue    = deadTimeVal;
    pwmSignal[1].faultState       = kPWM_PwmFaultState0;
    pwmSignal[1].pwmchannelenable = true;

    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_0, pwmSignal, 2U,
                 kPWM_SignedCenterAligned, DEMO_PWM_FREQUENCY_HZ, srcClkHz);

    /* Load all VALx registers and start the counter */
    PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_0, true);
    PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_0);
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
         *   DTSRCSEL = kPWM_UsePwm  (generated PWM23 feeds deadtime logic)   *
         *   IPOL     = PwmA         (PWM23 drives the complementary pair)     *
         * The initial call to PWM_Init() already applied these settings.      *
         * ------------------------------------------------------------------ */
        PRINTF("Phase 0: Normal - DTSRCSEL=UsePwm, IPOL=PwmA (30%% duty on A0)\r\n");
        SDK_DelayAtLeastUs(DEMO_PHASE_DELAY_US, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* ------------------------------------------------------------------ *
         * Phase 1: Update DTSRCSEL via FORCE_OUT                             *
         *                                                                     *
         * PWM_SetupForceSignal() writes the new DTSRCSEL value into the      *
         * shadow register. The value does NOT take effect yet.                *
         * PWM_EnableLocalForce() then issues a local FORCE_OUT event which  *
         * latches the shadow register into the active register immediately.   *
         *                                                                     *
         * Result: the inverted PWM23 signal is fed into the deadtime logic,  *
         * so the effective duty cycle at the pins flips to ~70%.              *
         * ------------------------------------------------------------------ */
        PRINTF("Phase 1: DTSRCSEL update via FORCE_OUT -> InvertedPwm\r\n");
        PWM_SetupForceSignal(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmA, kPWM_InvertedPwm);
        DEMO_TOGGLE_PHASE_GPIO(); /* toggle before FORCE so the edge marks the exact transition */
        PWM_EnableLocalForce(BOARD_PWM_BASEADDR, kPWM_Module_0);
        PRINTF("         DTSRCSEL=InvertedPwm active (observe duty 70%% on A0)\r\n");
        SDK_DelayAtLeastUs(DEMO_PHASE_DELAY_US, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);


        /* ------------------------------------------------------------------ *
         * Phase 2: Atomic DTSRCSEL + IPOL update via a single FORCE_OUT      *
         *                                                                     *
         * Both PWM_SetupForceSignal() and PWM_UpdateCurrentPolarity() write  *
         * their respective shadow registers without triggering force.         *
         * A single PWM_EnableLocalForce() call then applies both changes     *
         * simultaneously in one FORCE_OUT event — a glitch-free atomic       *
         * update.                                                             *
         *                                                                     *
         * Result: DTSRCSEL restored to normal AND IPOL switched to PwmB      *
         * (VAL4/VAL5 now drives the complementary pair) atomically.          *
         * ------------------------------------------------------------------ */
        PRINTF("Phase 2: Atomic DTSRCSEL + IPOL update via single FORCE_OUT\r\n");
        /* Write both shadow registers before issuing force — both take effect atomically */
        PWM_SetupForceSignal(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmA, kPWM_UsePwm);
        PWM_UpdateCurrentPolarity(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_ComplementaryPwmB);
        DEMO_TOGGLE_PHASE_GPIO(); /* toggle immediately before force so edge marks the transition */
        PWM_EnableLocalForce(BOARD_PWM_BASEADDR, kPWM_Module_0);
        PRINTF("         DTSRCSEL=UsePwm, IPOL=PwmB active (30%% duty on A0)\r\n");
        SDK_DelayAtLeastUs(DEMO_PHASE_DELAY_US, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* ------------------------------------------------------------------ *
         * Phase 3: Restore initial state atomically                          *
         *                                                                     *
         * Same atomic technique: write both shadow registers first, then     *
         * call PWM_EnableLocalForce() to apply both simultaneously.          *
         * ------------------------------------------------------------------ */
        PRINTF("Phase 3: Restore DTSRCSEL=UsePwm + IPOL=PwmA atomically\r\n");
        PWM_SetupForceSignal(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmA, kPWM_UsePwm);
        PWM_UpdateCurrentPolarity(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_ComplementaryPwmA);
        DEMO_TOGGLE_PHASE_GPIO();
        PWM_EnableLocalForce(BOARD_PWM_BASEADDR, kPWM_Module_0);
        PRINTF("         Back to initial state (30%% duty on A0)\r\n\r\n");
        SDK_DelayAtLeastUs(DEMO_PHASE_DELAY_US, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    }
}
