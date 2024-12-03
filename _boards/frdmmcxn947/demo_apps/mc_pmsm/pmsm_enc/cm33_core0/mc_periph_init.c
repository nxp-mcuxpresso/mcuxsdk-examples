/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mc_periph_init.h"
#include "peripherals.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_inputmux.h"
#include "fsl_port.h"
#include "fsl_lpcmp.h"
#include "fsl_spc.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Typedef
 ******************************************************************************/

typedef struct AdcInitData_t
{
    ADC_Type * const PtoAdc;
    const uint32_t u32ClockDivider;
    const clock_div_name_t ClockDivName;
    const clock_attach_id_t ClockAttachId;
} AdcInitData_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void InitFlexPWM1(void);
static void InitAdc1(void);
static void InitSlowLoop(void);
static void InitClock(void);
static void InitInputmux(void);
static void InitVref(void);
static void InitQD1(void);
static void InitCMP(void);

/*******************************************************************************
 * Constants
 ******************************************************************************/

/* ADC instances init data */
static const AdcInitData_t AdcInitData[] =
{
    { ADC0, 11U, kCLOCK_DivAdc0Clk, kPLL0_to_ADC0 }, // (150 / (11+1)) = 12.5 MHz
    { ADC1, 11U, kCLOCK_DivAdc1Clk, kPLL0_to_ADC1 }  // (150 / (11+1)) = 12.5 MHz
};

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* configuration structure for 3-phase PWM mc driver */
mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;

/* structure for current and voltage measurement*/
mcdrv_adc_t g_sM1AdcSensor;

/* Structure for Encoder driver */
mcdrv_qd_enc_t g_sM1Enc;

/* Clock setup structure */
clock_setup_t g_sClockSetup;


/*******************************************************************************
 * Public functions
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
    
    /* Init slow loop counter */
    InitSlowLoop();
    
    /* Init signal multiplexing */
    InitInputmux();             

    /* Init Vref */
    InitVref();
    
    /* 6-channel PWM peripheral init for M1 */
    InitFlexPWM1();
    
    /* Init ADC */
    InitAdc1();
    
    /* Qudrature decoder peripheral init */
    InitQD1();
    
    /* Comparator CMP */
    InitCMP();
}


/*******************************************************************************
 * Local functions
 ******************************************************************************/

/*!
 * @brief   void InitPWM1(void)
 *           - Initialization of the eFlexPWM1 peripheral for motor M1
 *           - 3-phase center-aligned PWM
 *
 * @param   void
 *
 * @return  none
 */
static void InitFlexPWM1()
{
    
    PWM_Type *PWMBase = (PWM_Type *)PWM1;

    // Enable eFlexPWM1 AHB clock
    CLOCK_EnableClock(kCLOCK_Pwm1);
    
    // Enable Submodules 0 - 2 clocks
    SYSCON->PWM1SUBCTL |= SYSCON_PWM1SUBCTL_CLK0_EN_MASK | SYSCON_PWM1SUBCTL_CLK1_EN_MASK | SYSCON_PWM1SUBCTL_CLK2_EN_MASK;
    
    /* Full and Half cycle reload */
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
    PWMBase->SM[0].VAL4 = PWM_VAL4_VAL4((uint16_t)((-(g_sClockSetup.ui16M1PwmModulo / 2) + (g_sClockSetup.ui16M1PwmDeadTime/2))));
    PWMBase->SM[1].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));
    PWMBase->SM[2].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));

    PWMBase->SM[0].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMBase->SM[1].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMBase->SM[2].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    
    /* PWM sub-module 0 trigger on VAL4 enabled for ADC synchronization */
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

    /* PWM fault filter - 5 Fast peripheral clocks sample rate, 5 agreeing
       samples to activate */
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
 *           - Initialization of the ADC peripheral
 *
 * @param   void
 *
 * @return  none
 */
static void InitAdc1(void)
{
    const AdcInitData_t * const pCurrentInitData = &(AdcInitData[1U]);
  
    lpadc_conv_trigger_config_t lpadcTriggerConfig;
    lpadc_conv_command_config_t lpadcCommandConfig;
    lpadc_config_t lpadcConfig;
    
    // Init the lpadcConfig struct
    LPADC_GetDefaultConfig(&lpadcConfig);
    lpadcConfig.enableAnalogPreliminary = true;
    lpadcConfig.referenceVoltageSource = kLPADC_ReferenceVoltageAlt3;
    lpadcConfig.conversionAverageMode = kLPADC_ConversionAverage1;

    /* Set clocks */
    CLOCK_SetClkDiv(pCurrentInitData->ClockDivName, pCurrentInitData->u32ClockDivider);
    CLOCK_AttachClk(pCurrentInitData->ClockAttachId);

    LPADC_Init(ADC1, &lpadcConfig);

    LPADC_DoOffsetCalibration(ADC1);
    LPADC_DoAutoCalibration(ADC1);
    
    LPADC_GetDefaultConvCommandConfig(&lpadcCommandConfig);
    lpadcCommandConfig.sampleChannelMode = kLPADC_SampleChannelSingleEndSideB;
    lpadcCommandConfig.conversionResolutionMode = kLPADC_ConversionResolutionStandard;
    lpadcCommandConfig.sampleTimeMode = kLPADC_SampleTimeADCK3;
     
    /* Init ADC channels. */
    lpadcCommandConfig.channelNumber = CUR_A_CHANNEL_NUMBER;
    lpadcCommandConfig.chainedNextCommandNumber = 2U;
    LPADC_SetConvCommandConfig( ADC1, 1U, &lpadcCommandConfig );

    lpadcCommandConfig.channelNumber = CUR_B_CHANNEL_NUMBER;
    lpadcCommandConfig.chainedNextCommandNumber = 3U;
    LPADC_SetConvCommandConfig( ADC1, 2U, &lpadcCommandConfig );        
                
    lpadcCommandConfig.channelNumber = CUR_C_CHANNEL_NUMBER;
    lpadcCommandConfig.chainedNextCommandNumber = 4U;
    LPADC_SetConvCommandConfig( ADC1, 3U, &lpadcCommandConfig );

    lpadcCommandConfig.channelNumber = VOLT_DCB_CHANNEL_NUMBER;
    lpadcCommandConfig.chainedNextCommandNumber = 0U;
    LPADC_SetConvCommandConfig( ADC1, 4U, &lpadcCommandConfig );
    
    
    /* Init triggers (use trigger 0). */
    LPADC_GetDefaultConvTriggerConfig(&lpadcTriggerConfig);
    lpadcTriggerConfig.targetCommandId = 1U;
    lpadcTriggerConfig.enableHardwareTrigger = true;
    LPADC_SetConvTriggerConfig(ADC1, 0U, &lpadcTriggerConfig);
    
    /* Enable TCOMP interrupt. */
    LPADC_EnableInterrupts(ADC1, ADC_IE_TCOMP_IE(0xFU));
    NVIC_SetPriority(ADC1_IRQn, 0U);
    NVIC_EnableIRQ(ADC1_IRQn);
    
    /* ADC1 base address */
    g_sM1AdcSensor.pToAdcBase = ADC1;
}



/*!
 * @brief   void InitSlowLoop(void)
 *           - Initialization of the CTIMER0 peripheral
 *           - performs slow control loop counter
 *
 * @param   void
 *
 * @return  none
 */
static void InitSlowLoop(void)
{
    /* Use 96 MHz clock for some of the Ctimer0. */
    CLOCK_SetClkDiv(kCLOCK_DivCtimer0Clk, 1u);
    CLOCK_AttachClk(kFRO_HF_to_CTIMER0);
    
    /* Enable Timer0 clock. */
    SYSCON->AHBCLKCTRLSET[1] |= SYSCON_AHBCLKCTRL1_TIMER0_MASK;

    /* Enable Timer0 clock reset. */
    SYSCON->PRESETCTRLSET[1] = SYSCON_PRESETCTRL1_TIMER0_RST_MASK;             /* Set bit. */
    while (0u == (SYSCON->PRESETCTRL1 & SYSCON_PRESETCTRL1_TIMER0_RST_MASK))   /* Wait until it reads 0b1 */
    {
    }
    
    /* Clear Timer0 clock reset. */                                  
    SYSCON->PRESETCTRLCLR[1] = SYSCON_PRESETCTRL1_TIMER0_RST_MASK;             /* Clear bit */
    while (SYSCON_PRESETCTRL1_TIMER0_RST_MASK ==                               /* Wait until it reads 0b0 */     
          (SYSCON->PRESETCTRL1 & SYSCON_PRESETCTRL1_TIMER0_RST_MASK))
    {
    }

    /* Configure match control register. */
    CTIMER0->MCR |= CTIMER_MCR_MR0R(1U)  |   /* Enable reset of TC after it matches with MR0. */
                    CTIMER_MCR_MR0I(1U);     /* Enable interrupt generation after TC matches with MR0. */
    
    /* Configure match register. */
    CTIMER0->MR[0] = (CLOCK_GetFreq(kCLOCK_FroHf))  /* Get CTimer0 frequency for correct set Match register value. */
                     / M1_SLOW_LOOP_FREQ;           /* Set slow control loop frequency in Hz. */
    
    /* Configure interrupt register. */
    CTIMER0->IR = CTIMER_IR_MR0INT_MASK;     /* Set interrupt flag for match channel 0. */
    NVIC_SetPriority(CTIMER0_IRQn, 1U);
    NVIC_EnableIRQ(CTIMER0_IRQn);            /* Enable LEVEL1 interrupt and update the call back function. */

    /* Configure timer control register. */
    CTIMER0->TCR |= CTIMER_TCR_CEN_MASK;     /* Start the timer counter. */
}


/*!
* @brief   void InitClock(void)
*          - Core, bus, flash clock setup
*
* @param   void
*
* @return  none
*/
static void InitClock(void)
{
    uint32_t ui32CyclesNumber = 0U;

    /* Calculate clock dependant variables for PMSM control algorithm */
    g_sClockSetup.ui32FastPeripheralClock = CLOCK_GetFreq(kCLOCK_CoreSysClk);
    g_sClockSetup.ui32CpuFrequency = CLOCK_GetFreq(kCLOCK_CoreSysClk);

    /* Parameters for motor M1 */
    g_sClockSetup.ui16M1PwmFreq   = M1_PWM_FREQ; /* 10 kHz */
    g_sClockSetup.ui16M1PwmModulo = (g_sClockSetup.ui32FastPeripheralClock) / g_sClockSetup.ui16M1PwmFreq;
    ui32CyclesNumber = ((M1_PWM_DEADTIME * (g_sClockSetup.ui32FastPeripheralClock / 1000000U)) / 1000U);
    g_sClockSetup.ui16M1PwmDeadTime   = ui32CyclesNumber;
    g_sClockSetup.ui16M1SpeedLoopFreq = M1_SPEED_LOOP_FREQ; /* 1kHz */
}


/*!
 * @brief   void InitInputmux(void)
 *           - Initialization of the Input Multiplexing
 *
 * @param   void
 *
 * @return  none
 */
static void InitInputmux(void)
{
    CLOCK_EnableClock(kCLOCK_InputMux);
    
    /* PWM0_SM0_OUT_TRIG0 is selected as trigger input for ADC1 channel 0 */
    INPUTMUX_AttachSignal(INPUTMUX0, 0U, kINPUTMUX_Pwm1A0Trig0ToAdc1Trigger);
    
}


/*!
 * @brief   void InitVref(void)
 *
 * @param   void
 *
 * @return  none
 */
static void InitVref(void)
{
    // Power up the VREF
    SPC0->ACTIVE_CFG1 |= 0x01U;
    
    // Enable VREF clock
    CLOCK_EnableClock(kCLOCK_Vref);

    /* Trim 1.0 V to 1.625 V */
    VREF0->UTRIM = 0x06U;  // Coarse tune to 1.6 V
    VREF0->UTRIM |= (0x3FU << 8U) ; //Fine tune to 1.625 V

    VREF0->CSR = VREF_CSR_BUF21EN_MASK      |
                 VREF_CSR_LPBG_BUF_EN_MASK  |
                 VREF_CSR_CHOPEN_MASK       |
                 VREF_CSR_ICOMPEN_MASK      |
                 VREF_CSR_HI_PWR_LV_MASK    |
                 VREF_CSR_REGEN_MASK        |
                 VREF_CSR_LPBGEN_MASK       |
                 VREF_CSR_HCBGEN_MASK;
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
static void InitQD1(void)
{
    const port_pin_config_t encTriggerPortConfig =
    {   /* Internal pull-up/down resistor is disabled */
        kPORT_PullDisable,
        /* Low internal pull resistor value is selected. */
        kPORT_LowPullResistor,
        /* Fast slew rate is configured */
        kPORT_FastSlewRate,
        /* Passive input filter is disabled */
        kPORT_PassiveFilterDisable,
        /* Open drain output is disabled */
        kPORT_OpenDrainDisable,
        /* Low drive strength is configured */
        kPORT_LowDriveStrength,
        /* Pin is configured as TRIG_INx */
        kPORT_MuxAlt1,
        /* Digital input enabled */
        kPORT_InputBufferEnable,
        /* Digital input is not inverted */
        kPORT_InputNormal,
        /* Pin Control Register fields [15:0] are not locked */
        kPORT_UnlockRegister
    };
    
    // Setup clocks ************************************************************
    // Enable clock to ENC modules
    CLOCK_EnableClock(kCLOCK_Qdc1);
    
    // Enable clock to the related PORT modules */
    CLOCK_EnableClock(kCLOCK_Port1);
    CLOCK_EnableClock(kCLOCK_Port2);
    CLOCK_EnableClock(kCLOCK_Port4);
    
    // Enable INPUTMUX0 clock
    CLOCK_EnableClock(kCLOCK_InputMux0);
    
    // Setup ports  ************************************************************   
    PORT_SetPinConfig(PORT1, 22U, &encTriggerPortConfig);   // ENC_A
    PORT_SetPinConfig(PORT2, 0U, &encTriggerPortConfig);    // ENC_B
    
    // Setup INPUTMUX  *********************************************************
    INPUTMUX_AttachSignal(INPUTMUX0, 0U, kINPUTMUX_TrigIn3ToQdc1Phasea);    //TRIG_IN3 -> ENC_A
    INPUTMUX_AttachSignal(INPUTMUX0, 0U, kINPUTMUX_TrigIn5ToQdc1Phaseb);    //TRIG_IN5 -> ENC_B
    
    /* Enable modulo counting and revolution counter increment on roll-over */
    QDC1->CTRL2 = (QDC_CTRL2_MOD_MASK | QDC_CTRL2_REVMOD_MASK);

    /* Pass initialization data into encoder driver structure */
    /* encoder position and speed measurement */
    g_sM1Enc.pui32QdBase   = (QDC_Type *)QDC1;
    g_sM1Enc.sTo.fltPGain  = M1_POSPE_TO_KP_GAIN;
    g_sM1Enc.sTo.fltIGain  = M1_POSPE_TO_KI_GAIN;
    g_sM1Enc.sTo.fltThGain = M1_POSPE_TO_THETA_GAIN;
    g_sM1Enc.a32PosMeGain  = M1_POSPE_MECH_POS_GAIN;
    g_sM1Enc.ui16Pp        = M1_MOTOR_PP;
    g_sM1Enc.bDirection    = M1_POSPE_ENC_DIRECTION;
    g_sM1Enc.fltSpdEncMin  = M1_POSPE_ENC_N_MIN;
    g_sM1Enc.ui16PulseNumber = M1_POSPE_ENC_PULSES;
      
    M1_MCDRV_QD_SET_DIRECTION(&g_sM1Enc);
      
    /* Initialization modulo counter*/
    M1_MCDRV_QD_SET_PULSES(&g_sM1Enc);

}

/*!
@brief   void InitCMP(void)
          - Initialization of the comparator module for dc-bus over current
            detection to generate PWM fault

@param   void

@return  none
*/
static void InitCMP(void)
{
    lpcmp_config_t mLpcmpConfigStruct;
    lpcmp_dac_config_t mLpcmpDacConfigStruct;
    
    /* attach FRO 12M to CMP1 */
    CLOCK_SetClkDiv(kCLOCK_DivCmp1FClk, 1u);
    CLOCK_AttachClk(kFRO12M_to_CMP1F);
    
    /* enable CMP1, CMP1_DAC  and VREF. */
    SPC_EnableActiveModeAnalogModules(SPC0, (kSPC_controlCmp1 | kSPC_controlCmp1Dac | kSPC_controlVref));
        
    /*
     *   k_LpcmpConfigStruct->enableStopMode      = false;
     *   k_LpcmpConfigStruct->enableOutputPin     = false;
     *   k_LpcmpConfigStruct->useUnfilteredOutput = false;
     *   k_LpcmpConfigStruct->enableInvertOutput  = false;
     *   k_LpcmpConfigStruct->hysteresisMode      = kLPCMP_HysteresisLevel0;
     *   k_LpcmpConfigStruct->powerMode           = kLPCMP_LowSpeedPowerMode;
     *   k_LpcmpConfigStruct->functionalSourceClock = kLPCMP_FunctionalClockSource0;
     */
    LPCMP_GetDefaultConfig(&mLpcmpConfigStruct);

    /* Init the LPCMP module. */
    LPCMP_Init(CMP1, &mLpcmpConfigStruct);

    /* Configure the internal DAC to output half of reference voltage. */
    mLpcmpDacConfigStruct.enableLowPowerMode     = false;
    mLpcmpDacConfigStruct.referenceVoltageSource = kLPCMP_VrefSourceVin1;
    mLpcmpDacConfigStruct.DACValue = (230U); /* Overcurrent threshold, max value 255 */
    LPCMP_SetDACConfig(CMP1, &mLpcmpDacConfigStruct);

    /* Configure LPCMP input channels. */
    LPCMP_SetInputChannels(CMP1, 1U, 7U);

}

