/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mc_periph_init.h"
#include "peripherals.h"

/*******************************************************************************
 * Typedef
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void InitADC(void);
static void InitPWM(void);
static void InitINPUTMUX(void);
static void InitClock(void);
static void InitCTIMER(void);
static void InitQD(void);

#if M1_FAULT_ENABLE   
    static void InitCMP(void);
#endif /* M1_FAULT_ENABLE */


/*******************************************************************************
 * Constants
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* configuration structure for 3-phase PWM mc driver */
mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;

/* structure for current and voltage measurement*/
mcdrv_adc_t g_sM1AdcSensor;

/* Structure for Encoder driver */
mcdrv_eqd_enc_t g_sM1Enc;

/* Clock setup structure */
clock_setup_t g_sClockSetup;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief   void MCDRV_Init_M1(void)
 *           - Motor control driver main initialization
 *           - Calls initialization functions of peripherals required for motor
 *             control functionality
 *
 * @param   void
 *
 * @return  none
 */
void MCDRV_Init_M1(void)
{
    /* Init application clock dependent variables */
    InitClock();
  
    /* Init ADC */
    InitADC();
    
    /* 6-channel PWM peripheral init */
    InitPWM();
    
    /* Init INPUTMUX */
    InitINPUTMUX();
    
    /* Slow loop timer init */
    InitCTIMER();
	
	/* Qudrature decoder peripheral init */
    InitQD();
    

#if M1_FAULT_ENABLE    
    /* Comparator CMP */
    InitCMP();   
#endif /* M1_FAULT_ENABLE */

    
}

/*!
 * @brief      Core, bus, flash clock setup
 *
 * @param      void
 *
 * @return     none
 */
void InitClock(void)
{
    uint32_t ui32CyclesNumber = 0U;

    /* Calculate clock dependant variables for PMSM control algorithm */
    g_sClockSetup.ui32FastPeripheralClock = CLOCK_GetFreq(kCLOCK_MainClk);   
    g_sClockSetup.ui32CpuFrequency = CLOCK_GetFreq(kCLOCK_CoreSysClk);

    /* Parameters for motor M1 */
    g_sClockSetup.ui16M1PwmFreq   = M1_PWM_FREQ; /* 16 kHz */
    g_sClockSetup.ui16M1PwmModulo = (g_sClockSetup.ui32FastPeripheralClock) / g_sClockSetup.ui16M1PwmFreq;
    ui32CyclesNumber = ((M1_PWM_DEADTIME * (g_sClockSetup.ui32FastPeripheralClock / 1000000U)) / 1000U);
    g_sClockSetup.ui16M1PwmDeadTime   = ui32CyclesNumber;
    g_sClockSetup.ui16M1SpeedLoopFreq = M1_SPEED_LOOP_FREQ; /* 1kHz */
}


/*!
 * @brief   void InitPWM(void)
 *           - Initialization of the FTM0 peripheral for motor M1
 *           - 3-phase center-aligned PWM
 *
 * @param   void
 *
 * @return  none
 */
static void InitPWM(void)
{
    // Enable eFlexPWM1 AHB clock
    CLOCK_EnableClock(kCLOCK_GateFLEXPWM0);
    
    // Enable Submodules 0 - 2 clocks
    SYSCON->PWM0SUBCTL |= SYSCON_PWM0SUBCTL_CLK0_EN_MASK |
                          SYSCON_PWM0SUBCTL_CLK1_EN_MASK |
                          SYSCON_PWM0SUBCTL_CLK2_EN_MASK;
  
    /* PWM base pointer (affects the entire initialization) */
    PWM_Type *PWMBase = (PWM_Type *)FLEXPWM0;

    /* Full cycle reload */
    PWMBase->SM[0].CTRL |= PWM_CTRL_FULL_MASK;
    PWMBase->SM[1].CTRL |= PWM_CTRL_FULL_MASK;
    PWMBase->SM[2].CTRL |= PWM_CTRL_FULL_MASK;

    /* Value register initial values, duty cycle 50% */
    PWMBase->SM[0].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));
    PWMBase->SM[1].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));
    PWMBase->SM[2].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));

    PWMBase->SM[0].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    PWMBase->SM[1].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    PWMBase->SM[2].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));

    PWMBase->SM[0].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));
    PWMBase->SM[1].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));
    PWMBase->SM[2].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));

    PWMBase->SM[0].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
    PWMBase->SM[1].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
    PWMBase->SM[2].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));

    PWMBase->SM[0].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));
    PWMBase->SM[1].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));
    PWMBase->SM[2].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));

    /* Trigger for ADC synchronization */
    PWMBase->SM[0].VAL4 = PWM_VAL4_VAL4((uint16_t)((-(g_sClockSetup.ui16M1PwmModulo / 2) + 10)));
    PWMBase->SM[1].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));
    PWMBase->SM[2].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));

    PWMBase->SM[0].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMBase->SM[1].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMBase->SM[2].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));

    /* PWM0 module 0 trigger on VAL4 enabled for ADC synchronization */
    PWMBase->SM[0].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 4) | PWM_TCTRL_TRGFRQ(1);

    /* Set dead-time register */
    PWMBase->SM[0].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[1].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[2].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[0].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[1].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[2].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);

    /* Channels A and B disabled when fault 0 occurs */
    PWMBase->SM[0].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWMBase->SM[1].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWMBase->SM[2].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x1));
    PWMBase->SM[0].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));
    PWMBase->SM[1].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));
    PWMBase->SM[2].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x1));

    /* Modules one and two gets clock from module zero */
    PWMBase->SM[1].CTRL2 = (PWMBase->SM[1].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);
    PWMBase->SM[2].CTRL2 = (PWMBase->SM[2].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);

    /* Master reload active for modules one and two */
    PWMBase->SM[1].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;
    PWMBase->SM[2].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;

    /* Master reload is generated every one opportunity */
    PWMBase->SM[0].CTRL = (PWMBase->SM[0].CTRL & ~PWM_CTRL_LDFQ_MASK) | PWM_CTRL_LDFQ(M1_FOC_FREQ_VS_PWM_FREQ - 1);

    /* Master sync active for modules one and two*/
    PWMBase->SM[1].CTRL2 = (PWMBase->SM[1].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);
    PWMBase->SM[2].CTRL2 = (PWMBase->SM[2].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);

    /* Fault 0 active in logic level one, automatic clearing */
    PWMBase->FCTRL = (PWMBase->FCTRL & ~PWM_FCTRL_FLVL_MASK) | PWM_FCTRL_FLVL(0x1);
    PWMBase->FCTRL = (PWMBase->FCTRL & ~PWM_FCTRL_FAUTO_MASK) | PWM_FCTRL_FAUTO(0x1);

    /* Clear fault flags */
    PWMBase->FSTS = (PWMBase->FSTS & ~PWM_FSTS_FFLAG_MASK) | PWM_FSTS_FFLAG(0xF);

    /* PWMs are re-enabled at PWM full cycle */
    PWMBase->FSTS = (PWMBase->FSTS & ~PWM_FSTS_FFULL_MASK) | PWM_FSTS_FFULL(0x1);

    /* PWM fault filter - 5 Fast peripheral clocks sample rate, 5 agreeing samples to activate */
    PWMBase->FFILT = (PWMBase->FFILT & ~PWM_FFILT_FILT_PER_MASK) | PWM_FFILT_FILT_PER(5);
    PWMBase->FFILT = (PWMBase->FFILT & ~PWM_FFILT_FILT_CNT_MASK) | PWM_FFILT_FILT_CNT(5);
       
    /* Start PWMs (set load OK flags and run) */
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_CLDOK_MASK) | PWM_MCTRL_CLDOK(0xF);
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_LDOK_MASK) | PWM_MCTRL_LDOK(0xF);
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_RUN_MASK) | PWM_MCTRL_RUN(0x0);   

    /* Initialize MC driver */
    g_sM1Pwm3ph.pui32PwmBaseAddress = (PWM_Type *)PWMBase;

    g_sM1Pwm3ph.ui16PhASubNum = 0U; /* PWMA phase A sub-module number */
    g_sM1Pwm3ph.ui16PhBSubNum = 1U; /* PWMA phase B sub-module number */
    g_sM1Pwm3ph.ui16PhCSubNum = 2U; /* PWMA phase C sub-module number */

    g_sM1Pwm3ph.ui16FaultFixNum = M1_FAULT_NUM; /* PWMA fixed-value over-current fault number */
    g_sM1Pwm3ph.ui16FaultAdjNum = M1_FAULT_NUM; /* PWMA adjustable over-current fault number */
    
}


/*!
 * @brief   void InitADC(void)
 *           - Initialization of the ADC16 peripheral
 *           - Initialization of the A/D converter for current and voltage sensing
 *
 * @param   void
 *
 * @return  none
 */
static void InitADC(void)
{
  
    lpadc_conv_trigger_config_t lpadcTriggerConfig;
    lpadc_conv_command_config_t lpadcCommandConfig;
    lpadc_config_t lpadcConfig;
    
    /* Init the lpadcConfig struct */
    LPADC_GetDefaultConfig(&lpadcConfig);
    lpadcConfig.enableAnalogPreliminary = true;
    lpadcConfig.referenceVoltageSource = kLPADC_ReferenceVoltageAlt3;
    lpadcConfig.conversionAverageMode = kLPADC_ConversionAverage1;
    lpadcConfig.FIFOWatermark = 2U;
    
    /* Attach peripheral clock (24MHz) */
    CLOCK_SetClockDiv(kCLOCK_DivADC0, 4u);
    CLOCK_AttachClk(kFRO_HF_to_ADC0);
    
    LPADC_Init(ADC0, &lpadcConfig);

    LPADC_DoOffsetCalibration(ADC0);
    LPADC_DoAutoCalibration(ADC0);
    
    LPADC_GetDefaultConvCommandConfig(&lpadcCommandConfig);
    lpadcCommandConfig.sampleChannelMode = kLPADC_SampleChannelSingleEndSideA;
    lpadcCommandConfig.conversionResolutionMode = kLPADC_ConversionResolutionStandard;
    lpadcCommandConfig.sampleTimeMode = kLPADC_SampleTimeADCK3;
        
    /* Init ADC channels. */
    lpadcCommandConfig.channelNumber = CUR_A_CHANNEL_NUMBER;
    lpadcCommandConfig.chainedNextCommandNumber = 2U;
    LPADC_SetConvCommandConfig( ADC0, 1U, &lpadcCommandConfig );

    lpadcCommandConfig.channelNumber = CUR_B_CHANNEL_NUMBER;
    lpadcCommandConfig.chainedNextCommandNumber = 3U;
    LPADC_SetConvCommandConfig( ADC0, 2U, &lpadcCommandConfig );        
                
    lpadcCommandConfig.channelNumber = CUR_C_CHANNEL_NUMBER;
    lpadcCommandConfig.chainedNextCommandNumber = 4U;
    LPADC_SetConvCommandConfig( ADC0, 3U, &lpadcCommandConfig );

    lpadcCommandConfig.channelNumber = VOLT_DCB_CHANNEL_NUMBER;
    lpadcCommandConfig.chainedNextCommandNumber = 0U;
    LPADC_SetConvCommandConfig( ADC0, 4U, &lpadcCommandConfig );
      
    /* Init triggers (use trigger 0). */
    LPADC_GetDefaultConvTriggerConfig(&lpadcTriggerConfig);
    lpadcTriggerConfig.targetCommandId = 1U;
    lpadcTriggerConfig.enableHardwareTrigger = true;
    LPADC_SetConvTriggerConfig(ADC0, 0U, &lpadcTriggerConfig);
        
    /* Enable TCOMP interrupt. */
    LPADC_EnableInterrupts(ADC0, kLPADC_FIFOWatermarkInterruptEnable);
    NVIC_SetPriority(ADC0_IRQn, 0U);
    NVIC_EnableIRQ(ADC0_IRQn);
    
    /* ADC0 base address */
    g_sM1AdcSensor.pToAdcBase = ADC0;

}


/*!
 * @brief   void INPUTMUX(void)
 *
 * @param   void
 *
 * @return  none
 */
static void InitINPUTMUX(void)
{
    /* write to INPUTMUX0: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GateINPUTMUX0);
    /* INPUTMUX0 peripheral is released from reset */
    RESET_PeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);
    /* PWM0_SM0_OUT_TRIG0 is selected as trigger input for ADC0 channel 0 */
    INPUTMUX_AttachSignal(INPUTMUX0, 0U, kINPUTMUX_Pwm0Sm0OutTrig0ToAdc0Trigger);
    /* CMP0_OUT is selected as trigger inp);ut for FlexPWM0 FAULT channel 0 */
    INPUTMUX_AttachSignal(INPUTMUX0, 0U, kINPUTMUX_Cmp0OutToFlexPwm0Fault);
  
}


/*!
 * @brief   void InitCTIMER(void)
 *           - Initialization of the CTIMER0 peripheral
 *           - performs slow control loop counter
 *
 * @param   void
 *
 * @return  none
 */
static void InitCTIMER(void)
{
    /* Clock setting */
    CLOCK_SetClockDiv(kCLOCK_DivCTIMER0, 1u);
    CLOCK_EnableClock(kCLOCK_GateCTIMER0);

    /* Configure match control register. */
    CTIMER0->MCR |= CTIMER_MCR_MR0R(1U)  |   /* Enable reset of TC after it matches with MR0. */
                    CTIMER_MCR_MR0I(1U);     /* Enable interrupt generation after TC matches with MR0. */
    
    /* Configure match register. */
    CTIMER0->MR[0] = (CLOCK_GetFreq(kCLOCK_FroHf))  /* Get CTimer0 frequency for correct set Match register value. */
                     / M1_SLOW_LOOP_FREQ;           /* Set slow control loop frequency in Hz. */
    
    /* Configure interrupt register. */
    CTIMER0->IR = CTIMER_IR_MR0INT_MASK;     /* Set interrupt flag for match channel 0. */
    NVIC_SetPriority(CTIMER0_IRQn, 2U);
    NVIC_EnableIRQ(CTIMER0_IRQn);            /* Enable LEVEL1 interrupt and update the call back function. */

    /* Configure timer control register. */
    CTIMER0->TCR |= CTIMER_TCR_CEN_MASK;     /* Start the timer counter. */
    
}


/*!
 * @brief   void InitQD1(void)
 *           - Initialization of the Quadrature Encoder 1 peripheral
 *           - performs speed and position sensor processing
 *
 * @param   void
 *
 * @return  none
 */
static void InitQD(void)
{      
    
    /* Enable clock to ENC modules */
    CLOCK_EnableClock(kCLOCK_GateQDC0);
    
    RESET_PeripheralReset(kQDC0_RST_SHIFT_RSTn);

    INPUTMUX_Init(INPUTMUX0);
    INPUTMUX_AttachSignal(INPUTMUX0, 0, kINPUTMUX_TrigIn10ToQdc0Phasea);
    INPUTMUX_AttachSignal(INPUTMUX0, 0, kINPUTMUX_TrigIn5ToQdc0Phaseb);
     
    /* Pass initialization data into encoder driver structure */
    /* encoder position and speed measurement */
    g_sM1Enc.pui32QdBase   = (EQDC_Type *)QDC0;
    g_sM1Enc.sTo.f16PGain = M1_POSPE_TO_KP_GAIN;
    g_sM1Enc.sTo.i16PGainSh = M1_POSPE_TO_KP_SHIFT;
    g_sM1Enc.sTo.f16IGain = M1_POSPE_TO_KI_GAIN;
    g_sM1Enc.sTo.i16IGainSh = M1_POSPE_TO_KI_SHIFT;
    g_sM1Enc.sTo.f16ThGain = M1_POSPE_TO_THETA_GAIN;
    g_sM1Enc.sTo.i16ThGainSh = M1_POSPE_TO_THETA_SHIFT;
    g_sM1Enc.a32PosMeGain = M1_POSPE_MECH_POS_GAIN;
    g_sM1Enc.ui16Pp = M1_MOTOR_PP;
    g_sM1Enc.bDirection = M1_POSPE_ENC_DIRECTION;
    g_sM1Enc.sTo.f32Speed = M1_POSPE_ENC_N_MIN;
    g_sM1Enc.ui16PulseNumber = M1_POSPE_ENC_PULSES;
		
    /* Quadrature pulses per one revolution */
    M1_MCDRV_QD_SET_PULSES(&g_sM1Enc); 
    /* Set encoder direction */
    M1_MCDRV_QD_SET_DIRECTION(&g_sM1Enc); 
    /* Enable modulo counting and revolution counter increment on roll-over */
    QDC0->CTRL2 = EQDC_CTRL2_REVMOD_MASK;

}


/*!
 * @brief   void InitCMP(void)
 *
 * @param   void
 *
 * @return  none
 */
#if M1_FAULT_ENABLE
static void InitCMP(void)
{
  
    lpcmp_config_t mLpcmpConfigStruct;
    lpcmp_dac_config_t mLpcmpDacConfigStruct;
    
    /* Get default configuration */
    LPCMP_GetDefaultConfig(&mLpcmpConfigStruct);
    
    /* Init the LPCMP module. */
    LPCMP_Init(CMP0, &mLpcmpConfigStruct);

    /* Configure the internal DAC to output half of reference voltage. */
    mLpcmpDacConfigStruct.enableLowPowerMode     = false;
    mLpcmpDacConfigStruct.referenceVoltageSource = kLPCMP_VrefSourceVin1;
    mLpcmpDacConfigStruct.DACValue = (CMP_THRESHOLD); /* Overcurrent threshold */
    LPCMP_SetDACConfig(CMP0, &mLpcmpDacConfigStruct);

    /* Configure LPCMP input channels. */
    LPCMP_SetInputChannels(CMP0, CMP_INPUT_CHANNEL, CMP_DAC_CHANNEL);
    
}
#endif /* M1_FAULT_ENABLE */

