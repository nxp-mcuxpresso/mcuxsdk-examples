/*
 * Copyright 2020-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_pwm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Definition for default PWM frequency in HZ. */
#ifndef APP_DEFAULT_PWM_FREQUENCE
#define APP_DEFAULT_PWM_FREQUENCE (1000UL)
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

static uint16_t PWM_OutputFrequenceToCounterValue(uint32_t u32CounterFrequency, uint32_t u32OutputFrequency)
{
    assert(u32CounterFrequency > u32OutputFrequency);
    assert((u32CounterFrequency / u32OutputFrequency) < 0xFFFF);
    return (uint16_t)(u32CounterFrequency / u32OutputFrequency);
}
/*!
 * @brief Main function
 */
int main(void)
{
    uint16_t u16DeadTime;
    uint16_t u16PwmPeriod;
    uint16_t u16PwmPulse;

    /* PWM configure structure */
    pwm_config_t sPwmConfig = {0};
    /* PWM submodule configure structure */
    pwm_sm_config_t sPwmSm0Config;
    pwm_sm_config_t sPwmSm1Config;
    /* PWM fault protection channel configure structure */
    pwm_fault_protection_config_t sFaultConfig;

    /* Board pin, clock, debug console initial */
    BOARD_InitHardware();

    PRINTF("FlexPWM driver example\r\n");

    /*
     * sPwmSmConfig.sCounterConfig.eCountClockSource = kPWM_ClockSrcBusClock;
     * sPwmSmConfig.sCounterConfig.eCountClockPrescale = kPWM_ClockPrescaleDivide1;
     * sPwmSmConfig.sCounterConfig.eCountInitSource = kPWM_InitOnLocalSync;
     * sPwmSmConfig.sReloadConfig.eReloadSignalSelect = kPWM_LocalReloadSignal;
     * sPwmSmConfig.sReloadConfig.eLoclReloadEffectTime = kPWM_TakeEffectAtReloadOportunity;
     * sPwmSmConfig.sReloadConfig.eLocalReloadOportunity = kPWM_LoadEveryOportunity;
     * sPwmSmConfig.sReloadConfig.bEnableFullCycleReloadOportunity = true;
     */
    PWM_GetSMDefaultConfig(&sPwmSm0Config);
    PWM_GetSMDefaultConfig(&sPwmSm1Config);
    /*
     * psConfig->sFaultInput[i].eFaultActiveLevel = kPWM_Logic0;
     * psConfig->sFaultInput[i].bEnableAutoFaultClear = true;
     * psConfig->sFaultInput[i].bEnableFaultFullCycleRecovery = true;
     */
    PWM_GetFaultProtectionDefaultConfig(&sFaultConfig);

    /* Fill PWM configure structure, submodule 1/2 use the same submodule configure structure */
    sPwmConfig.psPwmSubmoduleConfig[0]                     = &sPwmSm0Config;
    sPwmConfig.psPwmSubmoduleConfig[1]                     = &sPwmSm1Config;
    sPwmConfig.psPwmSubmoduleConfig[2]                     = &sPwmSm1Config;
    sPwmConfig.psFaultProtectionConfig[DEMO_FAULT_CHANNEL] = &sFaultConfig;

    /* Make submodule 0 PWM_A be a 50% duty cycle center-aligned PWM signal */
    u16PwmPeriod = PWM_OutputFrequenceToCounterValue(PWM_SRC_CLK_FREQ, APP_DEFAULT_PWM_FREQUENCE);
    u16PwmPulse  = u16PwmPeriod / 2U;
    PWM_GetValueConfig(&sPwmSm0Config.sValRegisterConfig, kPWM_SignedCenterAligned, u16PwmPeriod, u16PwmPulse,
                       u16PwmPulse);
    /* Make submodule 1/2 PWM_A period and pulse width same as submodule 0 */
    PWM_GetValueConfig(&sPwmSm1Config.sValRegisterConfig, kPWM_SignedCenterAligned, u16PwmPeriod, u16PwmPulse,
                       u16PwmPulse);

    /* Set counter clock prescaler */
    sPwmSm0Config.sCounterConfig.eCountClockPrescaler = PWM_SRC_CLK_DIV;
    /* Configure deadtime logic to make PWM_A/B be complementary mode with dead time insert */
    sPwmSm0Config.sDeadTimeConfig.eMode = kPWM_ComplementaryWithPwmA;
    /* Set dead time to about 650ns */
    u16DeadTime                                                     = USEC_TO_COUNT(65, PWM_IP_BUS_CLK_FREQ) / 100U;
    sPwmSm0Config.sDeadTimeConfig.sDeadTimeValue0.bitsIntegerCycles = u16DeadTime;
    sPwmSm0Config.sDeadTimeConfig.sDeadTimeValue1.bitsIntegerCycles = u16DeadTime;
    /* Map fault input to submodule 0 PWM_A output*/
    sPwmSm0Config.sPwmAFaultInputMapping = DEMO_FAULT_MAP;

    sPwmSm1Config.sCounterConfig.eCountClockSource = kPWM_ClockSrcSubmodule0Clock;
    sPwmSm1Config.sCounterConfig.eCountInitSource  = kPWM_InitOnMasterSync;
    /* Map fault input to submodule 1/2 PWM_A output*/
    sPwmSm1Config.sPwmAFaultInputMapping = DEMO_FAULT_MAP;

    /* Initial submodule 0/1/2 and fault protection channel 0 , fault channel will take effect for all submodules*/
    PWM_Init(BOARD_PWM_BASEADDR, &sPwmConfig);

    /* Start the PWM generation counter for Submodules 0, 1 and 2 */
    PWM_EnableCounters(BOARD_PWM_BASEADDR, kPWM_SubModule0Enable | kPWM_SubModule1Enable | kPWM_SubModule2Enable);

    while (1U)
    {
        /* Delay at least 100 PWM periods. */
        SDK_DelayAtLeastMs((1000U * 100 / APP_DEFAULT_PWM_FREQUENCE) + 1, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

        /* Increase the duty cycle by 4% each time */
        u16PwmPulse += u16PwmPeriod / 25U;

        /* Reset the duty cycle percentage */
        if (u16PwmPulse > u16PwmPeriod)
        {
            u16PwmPulse = u16PwmPeriod / 25U;
        }

        /* Update duty cycles for submodule 0 PWM_A signals */
        PWM_GetValueConfig(&sPwmSm0Config.sValRegisterConfig, kPWM_SignedCenterAligned, u16PwmPeriod, u16PwmPulse,
                           u16PwmPulse);
        PWM_SetValueRegister(BOARD_PWM_BASEADDR, kPWM_SubModule0, kPWM_VAL2,
                             sPwmSm0Config.sValRegisterConfig.u16ValRegister2);
        PWM_SetValueRegister(BOARD_PWM_BASEADDR, kPWM_SubModule0, kPWM_VAL3,
                             sPwmSm0Config.sValRegisterConfig.u16ValRegister3);
        /* Update submodule 1 PWM_A duty cycles with half of the output duty cycle of channel submodule 0 PWM_A  */
        PWM_GetValueConfig(&sPwmSm1Config.sValRegisterConfig, kPWM_SignedCenterAligned, u16PwmPeriod, u16PwmPulse / 2U,
                           u16PwmPulse / 2U);
        PWM_SetValueRegister(BOARD_PWM_BASEADDR, kPWM_SubModule1, kPWM_VAL2,
                             sPwmSm1Config.sValRegisterConfig.u16ValRegister2);
        PWM_SetValueRegister(BOARD_PWM_BASEADDR, kPWM_SubModule1, kPWM_VAL3,
                             sPwmSm1Config.sValRegisterConfig.u16ValRegister3);
        /* Update submodule 2 PWM_A duty cycles with half of the output duty cycle of channel submodule 1 PWM_A  */
        PWM_GetValueConfig(&sPwmSm1Config.sValRegisterConfig, kPWM_SignedCenterAligned, u16PwmPeriod, u16PwmPulse / 4U,
                           u16PwmPulse / 4U);
        PWM_SetValueRegister(BOARD_PWM_BASEADDR, kPWM_SubModule2, kPWM_VAL2,
                             sPwmSm1Config.sValRegisterConfig.u16ValRegister2);
        PWM_SetValueRegister(BOARD_PWM_BASEADDR, kPWM_SubModule2, kPWM_VAL3,
                             sPwmSm1Config.sValRegisterConfig.u16ValRegister3);
        PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_SubModule0Enable | kPWM_SubModule1Enable | kPWM_SubModule2Enable);
    }
}
