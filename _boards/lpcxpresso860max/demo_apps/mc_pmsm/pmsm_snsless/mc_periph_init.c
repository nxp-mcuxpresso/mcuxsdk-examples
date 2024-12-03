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
 * Functions
 ******************************************************************************/
static void InitADC(void);
static void InitPWM(void);
#if M1_FAULT_ENABLE   
static void InitCMP(void);
#endif /* M1_FAULT_ENABLE */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* configuration structure for 3-phase PWM mc driver */
mcdrv_pwm3ph_ftm_t g_sM1Pwm3ph;

/* structure for current and voltage measurement*/
mcdrv_adc_t g_sM1AdcSensor;

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
    /* Init ADC */
    InitADC();
    
    /* 6-channel PWM peripheral init */
    InitPWM();

#if M1_FAULT_ENABLE    
    /* Comparator ACOMP */
    InitCMP();
#endif /* M1_FAULT_ENABLE */

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

    CLOCK_EnableClock(kCLOCK_Ftm0);

    /* Disable all channels outputs using the OUTPUT MASK feature.
     * However, the output pins are still driven as GPIO since the
     * channel mode is set to FTM channel disabled after RESET */
    /* channel output is masked */
    M1_PWM_TIMER->OUTMASK = (FTM_OUTMASK_CH0OM_MASK | FTM_OUTMASK_CH1OM_MASK | FTM_OUTMASK_CH2OM_MASK |
                             FTM_OUTMASK_CH3OM_MASK | FTM_OUTMASK_CH4OM_MASK | FTM_OUTMASK_CH5OM_MASK);

    /* disable write protection for certain registers
     * free running counter and synchronization are different from TPM
     * Enable FTM features
     * Fault Control Mode - Fault control is enabled for all channels,
     * and the selected mode is the automatic fault clearing
     * Initialize The Channels Output */
    M1_PWM_TIMER->MODE = FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK | FTM_MODE_FAULTM(3) | FTM_MODE_INIT_MASK;

    /* Counter running in BDM mode */
    M1_PWM_TIMER->CONF = FTM_CONF_BDMMODE(0);

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
                            FTM_COMBINE_COMBINE1_MASK | FTM_COMBINE_COMP1_MASK | FTM_COMBINE_SYNCEN1_MASK |
                            FTM_COMBINE_COMBINE2_MASK | FTM_COMBINE_COMP2_MASK | FTM_COMBINE_SYNCEN2_MASK;

    /* Enable deatime */
    M1_PWM_TIMER->COMBINE |= FTM_COMBINE_DTEN0_MASK | FTM_COMBINE_DTEN1_MASK | FTM_COMBINE_DTEN2_MASK;

    /* Dead time length */
    M1_PWM_TIMER->DEADTIME =
        FTM_DEADTIME_DTPS(M1_PWM_DEADTIME_LENGTH_DTPS) | FTM_DEADTIME_DTVAL(M1_PWM_DEADTIME_LENGTH_DTVAL);

#if M1_FAULT_ENABLE
    /* Enable deatime */
    M1_PWM_TIMER->COMBINE |= FTM_COMBINE_FAULTEN0_MASK | FTM_COMBINE_FAULTEN1_MASK | FTM_COMBINE_FAULTEN2_MASK;

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
    
    /* Inverting control enabled */
    M1_PWM_TIMER->INVCTRL |= FTM_INVCTRL_INV0EN_MASK | FTM_INVCTRL_INV1EN_MASK | FTM_INVCTRL_INV2EN_MASK;
 
    /* Initial setting of value registers to 50 % of duty cycle  */
    M1_PWM_TIMER->CONTROLS[0].CnV = (uint32_t)(-M1_PWM_MODULO / 4);
    M1_PWM_TIMER->CONTROLS[1].CnV = (uint32_t)(M1_PWM_MODULO / 4);
    M1_PWM_TIMER->CONTROLS[2].CnV = (uint32_t)(-M1_PWM_MODULO / 4);
    M1_PWM_TIMER->CONTROLS[3].CnV = (uint32_t)(M1_PWM_MODULO / 4);
    M1_PWM_TIMER->CONTROLS[4].CnV = (uint32_t)(-M1_PWM_MODULO / 4);
    M1_PWM_TIMER->CONTROLS[5].CnV = (uint32_t)(M1_PWM_MODULO / 4);

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
    M1_PWM_TIMER->CONTROLS[2].CnSC = FTM_CnSC_ELSB_MASK;
    M1_PWM_TIMER->CONTROLS[3].CnSC = FTM_CnSC_ELSB_MASK;
    M1_PWM_TIMER->CONTROLS[4].CnSC = FTM_CnSC_ELSB_MASK;
    M1_PWM_TIMER->CONTROLS[5].CnSC = FTM_CnSC_ELSB_MASK;

    /* Set LOAD ok register */
    /* enable loading updated values */
    M1_PWM_TIMER->PWMLOAD |= FTM_PWMLOAD_LDOK_MASK;

    /* Initialization Trigger Enable */
    M1_PWM_TIMER->EXTTRIG = FTM_EXTTRIG_INITTRIGEN_MASK;
    
    /* Enable PWM */
    FTM0->SC |= (FTM_SC_PWMEN0(TRUE) | FTM_SC_PWMEN1(TRUE) | FTM_SC_PWMEN2(TRUE) | FTM_SC_PWMEN3(TRUE) |
                 FTM_SC_PWMEN4(TRUE) | FTM_SC_PWMEN5(TRUE));

    /* Set system clock as source for FTM (CLKS[1:0] = 01) */
    M1_PWM_TIMER->SC |= FTM_SC_CLKS(1);

    /* ---------------------------------------- */
    /* Initialization FTM 3-phase PWM mc driver */
    g_sM1Pwm3ph.pui32PwmBase = (FTM_Type *)(M1_PWM_TIMER); /* FTM base address */
    g_sM1Pwm3ph.ui16ChanPhA  = M1_PWM_PAIR_PHC;            /* PWM phase A top&bottom channel pair number */
    g_sM1Pwm3ph.ui16ChanPhB  = M1_PWM_PAIR_PHB;            /* PWM phase B top&bottom channel pair number */
    g_sM1Pwm3ph.ui16ChanPhC  = M1_PWM_PAIR_PHA;            /* PWM phase C top&bottom channel pair number */

    /* FTM Fault number for over current fault detection */
    g_sM1Pwm3ph.ui16FaultFixNum = M1_FAULT_NUM;
    
}

/*!
 * @brief   void MCDRV_Calib_ADC(void)
 *           - ADC initialization
 *
 * @param   void
 *
 * @return  none
 */
void MCDRV_Calib_ADC(void)
{
  
    uint32_t frequency = 0U;
  
    /* Attach FRO clock to ADC0. */
    CLOCK_Select(kADC_Clk_From_Fro);
    CLOCK_SetClkDivider(kCLOCK_DivAdcClk, 1U);
    /* Power on ADC0. */
    POWER_DisablePD(kPDRUNCFG_PD_ADC0);
    
    frequency = CLOCK_GetFreq(kCLOCK_CoreSysClk) / CLOCK_GetClkDivider(kCLOCK_DivAdcClk);
    ADC_DoSelfCalibration(ADC0, frequency);

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
    adc_config_t adcConfigStruct;
    adc_conv_seq_config_t adcConvSeqConfigStruct;

    /* Configure the converter. */
    adcConfigStruct.clockMode = kADC_ClockSynchronousMode; /* Using sync clock source. */
    adcConfigStruct.clockDividerNumber = 1U;
    adcConfigStruct.enableLowPowerMode = false;
    adcConfigStruct.voltageRange = kADC_HighVoltageRange;    
    
    ADC_Init(ADC0, &adcConfigStruct);
    
    /* Enable channel DEMO_ADC_SAMPLE_CHANNEL_NUMBER's conversion in Sequence A. */
    adcConvSeqConfigStruct.channelMask =
        (1U << CUR_A_CHANNEL_NUMBER) /* Includes channel DEMO_ADC_SAMPLE_CHANNEL_NUMBER. */
        | (1U << CUR_B_CHANNEL_NUMBER)
        | (1U << CUR_C_CHANNEL_NUMBER)
        | (1U << VOLT_DCB_CHANNEL_NUMBER);
    adcConvSeqConfigStruct.triggerMask      = 3U;
    adcConvSeqConfigStruct.triggerPolarity  = kADC_TriggerPolarityPositiveEdge;
    adcConvSeqConfigStruct.enableSingleStep = false;
    adcConvSeqConfigStruct.enableSyncBypass = false;
    adcConvSeqConfigStruct.interruptMode    = kADC_InterruptForEachSequence;
    ADC_SetConvSeqAConfig(ADC0, &adcConvSeqConfigStruct);
    ADC_EnableConvSeqA(ADC0, true); /* Enable the conversion sequence A. */
        
    /* Enable the interrupt. */
    /* Enable the interrupt the for sequence A done. */
    ADC_EnableInterrupts(ADC0, kADC_ConvSeqAInterruptEnable);
    NVIC_EnableIRQ(ADC0_SEQA_IRQn);

}

/*!
@brief   void InitCMP(void)
          - Initialization of the comparator 1 module for dc-bus over current
            detection to generate FTM0 fault
          - For enable Over-current fault is needed connect J6_8 (PIO0_14) with J1_9 (P0_13) on LPC board by wire
            (J2_9 with J4_8 on FRDM-MC-LVPMSM) and enable M1_FAULT_ENABLE macro.

@param   void

@return  none
*/

#if M1_FAULT_ENABLE
static void InitCMP(void)
{
    acomp_config_t acompConfigStruct;
    acomp_ladder_config_t ladderConfigStruct;
    
    /* Power on CMP0. */
    POWER_DisablePD(kPDRUNCFG_PD_ACMP);
    
    /* Initialize ACOMP module. */
    ACOMP_GetDefaultConfig(&acompConfigStruct);
    ACOMP_Init(ACOMP, &acompConfigStruct);

    /* Configure internal voltage ladder. */
    ladderConfigStruct.ladderValue      = M1_FAULT_CMP_THRESHOLD;
    ladderConfigStruct.referenceVoltage = kACOMP_LadderRefVoltagePinVDDCMP; /* VDDA as the reference voltage. */
    ACOMP_SetLadderConfig(ACOMP, &ladderConfigStruct);
    
    /* FTM0_FAULT0_MUX_SEL - Mux selection FTM0 fault from external I/O or internal. */
    SYSCON->FTMFLTCFG |= SYSCON_FTMFLTCFG_FTM0_FAULT0_MUX_SEL(1U);
    
    /* FTM0_FLT_INMUX - input select register for FTM0 FAULT */
    INPUTMUX->FTM0_FLT_INMUX[0] = ((INPUTMUX->FTM0_FLT_INMUX[0] & (~(INPUTMUX_FTM0_FLT_INMUX_INP_MASK))) | INPUTMUX_FTM0_FLT_INMUX_INP(0x03u));
    
    /* Configure ACOMP negative and positive input channels. */
    ACOMP_SetInputChannel(ACOMP, 0U, 3U);
  
}
#endif /* M1_FAULT_ENABLE */

