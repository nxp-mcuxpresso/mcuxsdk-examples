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

/*******************************************************************************
 * Functions
 ******************************************************************************/
static void InitADC(void);
static void InitPDB(void);
static void InitSlowLoop(void);
static void InitPWM(void);
#if M1_FAULT_CMP_ENABLE
static void InitCMP(void);
#endif /* M1_FAULT_CMP_ENABLE */
#if M1_INRUSH_ENABLE
static void InitRelay(void);
#endif /* M1_INRUSH_ENABLE */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* configuration structure for 3-phase PWM mc driver */
mcdrv_pwm3ph_ftm_t g_sM1Pwm3ph;

/* structure for current and voltage measurement*/
mcdrv_adc_mcxe_t g_sM1Curr3phDcBus;

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
#if M1_INRUSH_ENABLE
    InitRelay();
#endif /* M1_INRUSH_ENABLE */
    
    /* Init slow loop counter*/
    InitSlowLoop();

    /* 6-channel PWM peripheral init */
    InitPWM();
    
    /* Init ADC */
    InitADC();

    /* Init PDB */
    InitPDB();
    
#if M1_FAULT_CMP_ENABLE
    /* Comparator CMP1 */
    InitCMP();
#endif /* M1_FAULT_CMP_ENABLE */
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

    /* Disable all channels outputs using the OUTPUT MASK feature.
     * However, the output pins are still driven as GPIO since the
     * channel mode is set to FTM channel disabled after RESET */
    /* channel output is masked */
    M1_PWM_TIMER->OUTMASK = (FTM_OUTMASK_CH0OM_MASK | FTM_OUTMASK_CH1OM_MASK | FTM_OUTMASK_CH4OM_MASK |
                             FTM_OUTMASK_CH5OM_MASK | FTM_OUTMASK_CH6OM_MASK | FTM_OUTMASK_CH7OM_MASK);

    /* disable write protection for certain registers
     * free running counter and synchronization are different from TPM
     * Enable FTM features
     * Fault Control Mode - Fault control is enabled for all channels,
     * and the selected mode is the automatic fault clearing
     * Initialize The Channels Output */
    M1_PWM_TIMER->MODE = FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK | FTM_MODE_FAULTM(3) | FTM_MODE_INIT_MASK;

    /* Counter running in BDM mode */
    M1_PWM_TIMER->CONF = FTM_CONF_BDMMODE(1);

    /* initial value of the FTM counter */
    M1_PWM_TIMER->CNTIN = (uint32_t)(-M1_PWM_MODULO / 2);

    /* modulo value */
    M1_PWM_TIMER->MOD = (uint32_t)((M1_PWM_MODULO / 2) - 1);

    /* maximum loading point enable */
    M1_PWM_TIMER->SYNC = FTM_SYNC_CNTMAX_MASK;

    /* For center aligned PWM using combine mode:
     * COMBINE = 1 - combine mode set
     * COMP = 1 - complementary PWM set
     * SYNCEN = 1 - PWM update synchronization enabled
     */
    /* complementary mode */
    M1_PWM_TIMER->COMBINE = FTM_COMBINE_COMBINE0_MASK | FTM_COMBINE_COMP0_MASK | FTM_COMBINE_SYNCEN0_MASK |
                            FTM_COMBINE_COMBINE2_MASK | FTM_COMBINE_COMP2_MASK | FTM_COMBINE_SYNCEN2_MASK |
                            FTM_COMBINE_COMBINE3_MASK | FTM_COMBINE_COMP3_MASK | FTM_COMBINE_SYNCEN3_MASK;

#if M1_PWM_DEADTIME_ENABLE
    /* Enable deatime */
    M1_PWM_TIMER->COMBINE |= FTM_COMBINE_DTEN0_MASK | FTM_COMBINE_DTEN2_MASK | FTM_COMBINE_DTEN3_MASK;

    /* Dead time length */
    M1_PWM_TIMER->DEADTIME =
        FTM_DEADTIME_DTPS(M1_PWM_DEADTIME_LENGTH_DTPS) | FTM_DEADTIME_DTVAL(M1_PWM_DEADTIME_LENGTH_DTVAL);
#endif /* M1_PWM_DEADTIME_ENABLE */

#if M1_FAULT_ENABLE
    /* Enable deatime */
    M1_PWM_TIMER->COMBINE |= FTM_COMBINE_FAULTEN0_MASK | FTM_COMBINE_FAULTEN2_MASK | FTM_COMBINE_FAULTEN3_MASK;

#if M1_FAULT_NUM == 0U
    /* Enable Fault 0 */
    M1_PWM_TIMER->FLTCTRL |= FTM_FLTCTRL_FAULT0EN_MASK;
    /* Polarity Fault 0 */
    M1_PWM_TIMER->FLTPOL |= FTM_FLTPOL_FLT0POL(M1_FAULT_POL);

#endif /* M1_FAULT_NUM */

#if M1_FAULT_NUM == 1U
    /* Enable Fault 1 */
    M1_PWM_TIMER->FLTCTRL |= FTM_FLTCTRL_FAULT1EN_MASK;
    /* Polarity Fault 1 */
    M1_PWM_TIMER->FLTPOL |= FTM_FLTPOL_FLT1POL(M1_FAULT_POL);

#endif /* M1_FAULT_NUM */
#endif /* M1_FAULT_ENABLE */

#if M1_PWM_POL_TOP == 0U
    /* PWM polarity setting - high side */
    M1_PWM_TIMER->POL |= FTM_POL_POL0_MASK | FTM_POL_POL4_MASK | FTM_POL_POL6_MASK;
#endif

#if M1_PWM_POL_BOTTOM == 0U
    /* PWM polarity setting - low side */
    M1_PWM_TIMER->POL |= FTM_POL_POL1_MASK | FTM_POL_POL5_MASK | FTM_POL_POL7_MASK;
#endif

    /* Initial setting of value registers to 50 % of duty cycle  */
    M1_PWM_TIMER->CONTROLS[0].CnV = (uint32_t)(-M1_PWM_MODULO / 4);
    M1_PWM_TIMER->CONTROLS[1].CnV = (uint32_t)(M1_PWM_MODULO / 4);
    M1_PWM_TIMER->CONTROLS[4].CnV = (uint32_t)(-M1_PWM_MODULO / 4);
    M1_PWM_TIMER->CONTROLS[5].CnV = (uint32_t)(M1_PWM_MODULO / 4);
    M1_PWM_TIMER->CONTROLS[6].CnV = (uint32_t)(-M1_PWM_MODULO / 4);
    M1_PWM_TIMER->CONTROLS[7].CnV = (uint32_t)(M1_PWM_MODULO / 4);

    /* Note:
     * 1. From this moment the output pins are under FTM control. Since the PWM
     *    output is disabled by the FTM0OUTMASK register, there is no change on
     *    PWM outputs. Before the channel mode is set, the correct output pin
     *    polarity has to be defined.
     * 2. Even if the odd channels are generated automatically by complementary
     *    logic, these channels have to be set to be in the same channel mode.
     */
    M1_PWM_TIMER->CONTROLS[0].CnSC = FTM_CnSC_ELSB_MASK;
    M1_PWM_TIMER->CONTROLS[1].CnSC = FTM_CnSC_ELSB_MASK;
    M1_PWM_TIMER->CONTROLS[4].CnSC = FTM_CnSC_ELSB_MASK;
    M1_PWM_TIMER->CONTROLS[5].CnSC = FTM_CnSC_ELSB_MASK;
    M1_PWM_TIMER->CONTROLS[6].CnSC = FTM_CnSC_ELSB_MASK;
    M1_PWM_TIMER->CONTROLS[7].CnSC = FTM_CnSC_ELSB_MASK;

    /* Set LOAD ok register */
    /* enable loading updated values */
    M1_PWM_TIMER->PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;

    /* Initialization Trigger Enable */
    M1_PWM_TIMER->EXTTRIG = FTM_EXTTRIG_INITTRIGEN_MASK;

    /* Set system clock as source for FTM (CLKS[1:0] = 01) */
    M1_PWM_TIMER->SC = FTM_SC_CLKS(1);
    
    /* PWM enable */
    M1_PWM_TIMER->SC |= FTM_SC_PWMEN0(1) | FTM_SC_PWMEN1(1) | FTM_SC_PWMEN4(1) | FTM_SC_PWMEN5(1) | FTM_SC_PWMEN6(1) | FTM_SC_PWMEN7(1);

    /* ---------------------------------------- */
    /* Initialization FTM 3-phase PWM mc driver */
    g_sM1Pwm3ph.pui32PwmBase = (FTM_Type *)(M1_PWM_TIMER); /* FTM base address */
    g_sM1Pwm3ph.ui16ChanPhA  = M1_PWM_PAIR_PHA;            /* PWM phase A top&bottom channel pair number */
    g_sM1Pwm3ph.ui16ChanPhB  = M1_PWM_PAIR_PHB;            /* PWM phase B top&bottom channel pair number */
    g_sM1Pwm3ph.ui16ChanPhC  = M1_PWM_PAIR_PHC;            /* PWM phase C top&bottom channel pair number */

    /* FTM Fault number for over current fault detection */
    g_sM1Pwm3ph.ui16FaultFixNum = M1_FAULT_NUM;
}

/*!
 * @brief   void InitSlowLoop(void)
 *           - Initialization of the FTM peripheral
 *           - performs slow control loop counter
 *
 * @param   void
 *
 * @return  none
 */
static void InitSlowLoop(void)
{

  /* Configuration of FTM module */
    /* Disable write protection for certain registers */
    /* Enable FTM features */
    M1_SLOW_LOOP_TIMER->MODE = FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK;

    /* Settings up FTM SC register for clock setup */
    /* Set system clock as source for FTM0 (CLKS[1:0] = 01) */
    /* Set prescaler to 16 */
    /* Enable interrupt */
    M1_SLOW_LOOP_TIMER->SC = FTM_SC_PS(4) | FTM_SC_CLKS(1) | FTM_SC_TOIE_MASK;

    /* Counter running in BDM mode */
    M1_SLOW_LOOP_TIMER->CONF = FTM_CONF_BDMMODE(3);

    /* Set count value to 0 */
    M1_SLOW_LOOP_TIMER->CNTIN = 0x0;

    /* Slow Loop freq */
    M1_SLOW_LOOP_TIMER->MOD = (M1_SLOW_LOOP_MODULO) >> (M1_SLOW_LOOP_TIMER->SC & FTM_SC_PS_MASK);

    /* LOADOK */
    M1_SLOW_LOOP_TIMER->PWMLOAD = FTM_PWMLOAD_LDOK_MASK;

    /* Enable & setup interrupts */
    EnableIRQ(FTM2_Ovf_Reload_IRQn);
    NVIC_SetPriority(FTM2_Ovf_Reload_IRQn, 3);
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
  
    adc12_config_t adc12ConfigStruct;
    adc12_channel_config_t adc12ChannelConfigStruct;

    ADC12_GetDefaultConfig(&adc12ConfigStruct);
    
    adc12ConfigStruct.resolution = kADC12_Resolution12Bit;
    adc12ConfigStruct.sampleClockCount = 2U;
    
    ADC12_Init(ADC0, &adc12ConfigStruct);
    ADC12_EnableHardwareTrigger(ADC0, true);

    ADC12_Init(ADC1, &adc12ConfigStruct);
    ADC12_EnableHardwareTrigger(ADC1, true);
   
    ADC12_DoAutoCalibration(ADC0);
    ADC12_DoAutoCalibration(ADC1);

    adc12ChannelConfigStruct.channelNumber = 2U; // ADC0 channel  (Curr B)
    adc12ChannelConfigStruct.enableInterruptOnConversionCompleted = false; /* Enable the interrupt. */
    ADC12_SetChannelConfig(ADC0, 0U, &adc12ChannelConfigStruct); // channel group 0  

    adc12ChannelConfigStruct.channelNumber = 0U; // ADC0 channel  (Volt DCB)
    adc12ChannelConfigStruct.enableInterruptOnConversionCompleted = true; /* Enable the interrupt. */
    ADC12_SetChannelConfig(ADC0, 1U, &adc12ChannelConfigStruct); // channel group 1

    
    adc12ChannelConfigStruct.channelNumber = 6U; // ADC1 channel  (Curr C)
    adc12ChannelConfigStruct.enableInterruptOnConversionCompleted = false; /* Enable the interrupt. */
    ADC12_SetChannelConfig(ADC1, 0U, &adc12ChannelConfigStruct); // channel group 0
    
    adc12ChannelConfigStruct.channelNumber = 12U; // ADC1 channel (Curr A)
    adc12ChannelConfigStruct.enableInterruptOnConversionCompleted = false; /* Enable the interrupt. */
    ADC12_SetChannelConfig(ADC1, 1U, &adc12ChannelConfigStruct); // channel group 1
    
    /* Enable ADC ISR */
    EnableIRQ(ADC0_IRQn);
    NVIC_SetPriority(ADC0_IRQn, 1);
    
}

/*!
 * @brief      void InitPDB(void)
 *              - Initialization of the PDB for ADC triggering and
 *		         synchronization between FTM0 and ADC16
 *
 * @param      void
 *
 * @return     none
 */
static void InitPDB(void)
{

    pdb_config_t pdbConfigStruct;
    pdb_adc_pretrigger_config_t pdbAdcPreTriggerConfigStruct;
    
    TRGMUX_SetTriggerSource(TRGMUX, kTRGMUX_Pdb0, kTRGMUX_TriggerInput0, kTRGMUX_SourceFtm0InitTrg);
    TRGMUX_SetTriggerSource(TRGMUX, kTRGMUX_Pdb1, kTRGMUX_TriggerInput0, kTRGMUX_SourceFtm0InitTrg);
  
    /* Configure the PDB counter. */
    PDB_GetDefaultConfig(&pdbConfigStruct);
    pdbConfigStruct.triggerInputSource = kPDB_TriggerSoftware; 
        
    PDB_Init(PDB0, &pdbConfigStruct);
    PDB_Init(PDB1, &pdbConfigStruct);

    /* Configure the delay interrupt. */
    PDB_SetModulusValue(PDB0, 4000U);
    PDB_SetModulusValue(PDB1, 4000U);

    /* The available delay value is less than or equal to the modulus value. */
    PDB_SetCounterDelayValue(PDB0, 4000U);
    PDB_EnableInterrupts(PDB0, kPDB_DelayInterruptEnable);
    PDB_SetCounterDelayValue(PDB1, 4000U);
    PDB_EnableInterrupts(PDB1, kPDB_DelayInterruptEnable);
    
    PDB0->CH[0].DLY[0] = 10;
    PDB1->CH[0].DLY[0] = 10;

    /* Configure the ADC Pre-Trigger. */
    pdbAdcPreTriggerConfigStruct.enablePreTriggerMask          = (1U << kPDB_ADCPreTrigger0)|(1U << kPDB_ADCPreTrigger1);
    pdbAdcPreTriggerConfigStruct.enableOutputMask              = (1U << kPDB_ADCPreTrigger0)|(1U << kPDB_ADCPreTrigger1);
    pdbAdcPreTriggerConfigStruct.enableBackToBackOperationMask = 2U;
    PDB_SetADCPreTriggerConfig(PDB0, kPDB_ADCTriggerChannel0, &pdbAdcPreTriggerConfigStruct); //PDB ch0

    pdbAdcPreTriggerConfigStruct.enablePreTriggerMask          = (1U << kPDB_ADCPreTrigger0)|(1U << kPDB_ADCPreTrigger1);
    pdbAdcPreTriggerConfigStruct.enableOutputMask              = (1U << kPDB_ADCPreTrigger0)|(1U << kPDB_ADCPreTrigger1);
    pdbAdcPreTriggerConfigStruct.enableBackToBackOperationMask = 2U;
    PDB_SetADCPreTriggerConfig(PDB1, kPDB_ADCTriggerChannel0, &pdbAdcPreTriggerConfigStruct); //PDB ch0
    
    /* LDOK */
    PDB_DoLoadValues(PDB0);
    PDB_DoLoadValues(PDB1);
    
    EnableIRQ(PDB0_IRQn);
    NVIC_SetPriority(PDB0_IRQn, 2);
    EnableIRQ(PDB1_IRQn);
    NVIC_SetPriority(PDB1_IRQn, 2);
    
}
/*!
@brief   void InitCMP(void)
          - Initialization of the comparator 1 module for dc-bus over current
            detection to generate FTM0 fault

@param   void

@return  none
*/
#if M1_FAULT_CMP_ENABLE
static void InitCMP(void)
{

    /* Selects the source of the FTM0 fault - TRGMUX_FTM0 out */
    SIM->FTMOPT0 = ((SIM->FTMOPT0 & /* Mask bits to zero which are setting */
                     (~(SIM_FTMOPT0_FTM0FLTXSEL_MASK)))
                    | SIM_FTMOPT0_FTM0FLTXSEL(1));
    
    /* Invert polarity */
    /* Filter - 4 consecutive samples must agree */
    M1_FAULT_CMP_INSTANCE->C0 |= CMP_C0_FILTER_CNT(4);

    /* DAC output set to M1_FAULT_CMP_THRESHOLD */
    /* Reference voltage will be VDD */
    /* Enable DAC */    
    M1_FAULT_CMP_INSTANCE->C1 = CMP_C1_VOSEL(M1_FAULT_CMP_THRESHOLD) | CMP_C1_DACEN_MASK;   

    /* Plus is CMP1_IN1 ~ overcurrent pin */
    /* Minus is CMP1_IN0 ~ 6bit reference */
    M1_FAULT_CMP_INSTANCE->C1 |= CMP_C1_INPSEL(1) | CMP_C1_INNSEL(0) | CMP_C1_PSEL(1) | CMP_C1_MSEL(0);

    /* Enable analog comparator */
    M1_FAULT_CMP_INSTANCE->C0 |= CMP_C0_EN_MASK;
    
}
#endif /* M1_FAULT_CMP_ENABLE */

/*!
 *@brief      Set Inrush relay on HVP
 *
 *@param      none
 *
 *@return     none
 */
#if M1_INRUSH_ENABLE
static void InitRelay(void)
{
    volatile register uint32_t ui32DelayMs = 0;

    /* setup SysTick */
    SysTick->LOAD = 0xFFFFFF;
    SysTick->VAL  = SysTick->LOAD;
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

    /* wait M1_INRUSH_DELAY milliseconds to turn on the relay */
    while (ui32DelayMs++ < M1_INRUSH_DELAY)
    {
        while (((SysTick->LOAD - SysTick->VAL) * 1000) < SystemCoreClock)
        {
        };
        SysTick->VAL = SysTick->LOAD;
    }

    /* turn on relay */
    M1_INRUSH_SET();
}
#endif /* M1_INRUSH_ENABLE */
