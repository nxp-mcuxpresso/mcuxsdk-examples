/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_flexpwm.h"
#include "fsl_xbar.h"
#include "fsl_lpit.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Definition for default PWM frequence in hz. */
#ifndef APP_DEFAULT_PWM_FREQUENCY
#define APP_DEFAULT_PWM_FREQUENCY (1000UL)
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
volatile bool lpitIsrFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
void DEMO_LPIT_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    LPIT_ClearStatusFlags(DEMO_LPIT_BASE, kLPIT_Channel0TimerFlag);
    lpitIsrFlag = true;
    SDK_ISR_EXIT_BARRIER;
}

static void lpit_init(void)
{
    /* Structure of initialize LPIT */
    lpit_config_t lpitConfig;
    lpit_chnl_params_t lpitChannelConfig;

    LPIT_GetDefaultConfig(&lpitConfig);
    LPIT_Init(DEMO_LPIT_BASE, &lpitConfig);

    lpitChannelConfig.chainChannel          = false;
    lpitChannelConfig.enableReloadOnTrigger = false;
    lpitChannelConfig.enableStartOnTrigger  = true;
    lpitChannelConfig.enableStopOnTimeout   = true;
    lpitChannelConfig.timerMode             = kLPIT_PeriodicCounter;
    /* Set default values for the trigger source */
    lpitChannelConfig.triggerSelect = kLPIT_Trigger_TimerChn0;
    lpitChannelConfig.triggerSource = kLPIT_TriggerSource_External;

    /* Init lpit channel 0 */
    LPIT_SetupChannel(DEMO_LPIT_BASE, kLPIT_Chnl_0, &lpitChannelConfig);

    /* Set timer period for channel 0 */
    LPIT_SetTimerPeriod(DEMO_LPIT_BASE, kLPIT_Chnl_0, USEC_TO_COUNT(1000000U, LPIT_SOURCECLOCK));

    /* Enable timer interrupts for channel 0 */
    LPIT_EnableInterrupts(DEMO_LPIT_BASE, kLPIT_Channel0TimerInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(DEMO_LPIT_IRQn);

    /* Start channel 0 */
    PRINTF("\r\nStarting channel No.0 ...");
    LPIT_StartTimer(DEMO_LPIT_BASE, kLPIT_Chnl_0);
}

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

    /* PWMB values don't matter in complementary mode */
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
    flexpwm_output_trigger_config_t outputTriggerConfig;
    uint32_t pwmSourceClockInHz;
    uint32_t pwmFrequencyInHz = APP_DEFAULT_PWM_FREQUENCY;
    uint16_t modValue;
    uint16_t deadTimeVal;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    XBAR_Init(kXBAR_DSC1);
    BLK_CTRL_WAKEUPMIX->LPIT_TRIG_SEL |= BLK_CTRL_WAKEUPMIX_LPIT_TRIG_SEL_LPIT1_TRIG0_INPUT_SEL(1);
    XBAR_SetSignalsConnection(kXBAR1_InputFlexpwm1Mux0Trigger0, kXBAR1_OutputLpit1LpitExtTrigIn0);
    PRINTF("\r\nIPSYNC trigger signal connected! \r\n");

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
     * submoduleConfig.counterConfig.modValue = 0;
     * submoduleConfig.counterConfig.initSource = kFLEXPWM_InitSource_LocalSync;
     * submoduleConfig.reloadConfig.loadMode = kFLEXPWM_LoadMode_Opportunity;
     * submoduleConfig.reloadConfig.enableFullCycleReload = false;
     * submoduleConfig.reloadConfig.enableHalfCycleReload = false;
     * submoduleConfig.reloadConfig.halfCycleValue = 0;
     * submoduleConfig.reloadConfig.loadFrequency = 0;
     * submoduleConfig.reloadConfig.reloadSource = kFLEXPWM_ReloadSource_LocalReload;
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
     * faultConfig.faultInputActiveLevel = false;
     * faultConfig.faultClearingMode = kFLEXPWM_FaultClearMode_Automatic;
     * faultConfig.enableCombinationalPath = true;
     * faultConfig.faultFilterCount = 0;
     * faultConfig.faultFilterPeriod = 0;
     */
    FLEXPWM_GetDefaultFaultConfig(&faultConfig);

#ifdef DEMO_PWM_FAULT_LEVEL
    faultConfig.faultInputActiveLevel = DEMO_PWM_FAULT_LEVEL;
#endif

    /* Sets up the PWM fault protection */
    FLEXPWM_ConfigFaultProtection(BOARD_PWM_BASEADDR, 0U, &faultConfig, DEMO_FAULT_MASK);

#if defined(FSL_FEATURE_PWM_FAULT_CH_COUNT) && (FSL_FEATURE_PWM_FAULT_CH_COUNT > 1)
    FLEXPWM_ConfigFaultProtection(BOARD_PWM_BASEADDR, 1U, &faultConfig, DEMO_FAULT_MASK);
#endif

    /* Get default fault submodule configuration */
    FLEXPWM_GetDefaultFaultSubmoduleConfig(&faultSubmoduleConfig);

    /* Disable fault-to-PWM-output mapping for submodule 0/1/2 */
    faultSubmoduleConfig.pwma.disableMask_ch0 = 0U;
    faultSubmoduleConfig.pwmb.disableMask_ch0 = 0U;
    faultSubmoduleConfig.pwmx.disableMask_ch0 = 0U;

#if defined(FSL_FEATURE_PWM_FAULT_CH_COUNT) && (FSL_FEATURE_PWM_FAULT_CH_COUNT > 1)
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

    /* Configure output trigger for submodule 0 */
    /* Set VAL4 compare value for trigger timing */
    FLEXPWM_SetVAL4(BOARD_PWM_BASEADDR, 0U, 0xff20U);
    
    /* Configure output trigger - VAL4 match triggers PWM_OUT_TRIG0 */
    outputTriggerConfig.outTriggerEnable = kFLEXPWM_OutputTriggerMask_VAL4;
    outputTriggerConfig.muxTrig0Source = kFLEXPWM_TriggerMuxSource_GeneratedTrigger;
    outputTriggerConfig.muxTrig1Source = kFLEXPWM_TriggerMuxSource_GeneratedTrigger;
    outputTriggerConfig.triggerFrequency = kFLEXPWM_TriggerFrequency_EveryCycle;
    FLEXPWM_ConfigOutputTrigger(BOARD_PWM_BASEADDR, 0U, &outputTriggerConfig);

    /* Enable PWM output for all submodules */
    FLEXPWM_EnablePWMOutput(BOARD_PWM_BASEADDR, DEMO_SUBMODULE_MASK, DEMO_SUBMODULE_MASK, 0U);

    /* Set the load okay bit for all submodules to load registers from their buffer */
    FLEXPWM_SetLoadOkay(BOARD_PWM_BASEADDR, DEMO_SUBMODULE_MASK);

    /* Start the PWM generation from Submodules 0, 1 and 2 */
    FLEXPWM_EnableSubmoduleCounter(BOARD_PWM_BASEADDR, DEMO_SUBMODULE_MASK);

    PRINTF("LPIT init\r\n");
    lpit_init();

    while (true)
    {
        /* Check whether occur interupt and toggle LED */
        if (true == lpitIsrFlag)
        {
            PRINTF("\r\n Channel No.0 interrupt is occurred !");
            LED_TOGGLE();
            lpitIsrFlag = false;
        }
    }
}
