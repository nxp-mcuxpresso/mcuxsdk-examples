/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021, 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mc_periph_init.h"
#include "fsl_lpadc.h"
#include "fsl_common.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Structure for current and voltage measurement */
mcdrv_adc_t g_sM1Curr3phDcBus;

/* Structure for 3-phase PWM MC driver */
mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;

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
    M1_MCDRV_CURR_3PH_VOLT_DCB_INIT();

    /* Init TMR1 (slow loop counter) */
    M1_MCDRV_TMR_SLOWLOOP_INIT();

    /* 6-channel PWM peripheral init for M1 */
    M1_MCDRV_PWM_PERIPH_INIT();

    /* Quadrature decoder peripheral init */
    M1_MCDRV_ENC_INIT();

    /* Comparator CMP */
    M1_MCDRV_CMP_INIT();
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

    /*
     * RT2660 HP Run: CM85 = 1000 MHz (kCLOCK_Root_CMPT_cpu_clk),
     *                MAIN/CMPT bus = 333 MHz (kCLOCK_Root_CGU_MAIN_ROOTCLK).
     * PWM, QTimer, EQDC all run from the MAIN HSP bus at 333 MHz.
     */
    g_sClockSetup.ui32FastPeripheralClock = CLOCK_GetRootClockFreq(kCLOCK_Root_CGU_MAIN_ROOTCLK);
    g_sClockSetup.ui32CpuFrequency        = CLOCK_GetRootClockFreq(kCLOCK_Root_CMPT_cpu_clk);

    /* Parameters for motor M1 */
    g_sClockSetup.ui16M1PwmFreq   = M1_PWM_FREQ; /* 16 kHz */
    g_sClockSetup.ui16M1PwmModulo = (g_sClockSetup.ui32FastPeripheralClock) / g_sClockSetup.ui16M1PwmFreq;
    ui32CyclesNumber = ((M1_PWM_DEADTIME * (g_sClockSetup.ui32FastPeripheralClock / 1000000U)) / 1000U);
    g_sClockSetup.ui16M1PwmDeadTime   = ui32CyclesNumber;
    g_sClockSetup.ui16M1SpeedLoopFreq = M1_SPEED_LOOP_FREQ; /* 1 kHz */
}

/*!
 * @brief   void InitADC(void)
 *           - Initialization of the ADC peripheral
 *
 * @param   void
 *
 * @return  none
 */
void InitADC(void)
{
#if FSL_LPADC_DRIVER_VERSION != (MAKE_VERSION(2, 11, 0))
#warning Used a different fsl_lpadc driver version! An example may not work correctly!
#endif

    lpadc_config_t mLpadcConfigStruct;
    lpadc_conv_trigger_config_t mLpadcTriggerConfigStruct;
    lpadc_conv_command_config_t mLpadcCommandConfigStruct;

    /* Enable clocks to LPADC peripherals */
    CLOCK_EnableClock(kCLOCK_MAIN_hsp_adc0);
    CLOCK_EnableClock(kCLOCK_MAIN_hsp_adc1);

    LPADC_GetDefaultConfig(&mLpadcConfigStruct);

    mLpadcConfigStruct.enableAnalogPreliminary = true;
    mLpadcConfigStruct.referenceVoltageSource  = kLPADC_ReferenceVoltageAlt2; /* VDDA_ADC_1P8, connect J11 (1-2) */
    mLpadcConfigStruct.conversionAverageMode   = kLPADC_ConversionAverage1024; /* max HW average during calibration */
    mLpadcConfigStruct.FIFO1Watermark          = 2U;

    LPADC_Init(HSP__ADC_0, &mLpadcConfigStruct);
    LPADC_Init(HSP__ADC_1, &mLpadcConfigStruct);

    LPADC_SetOffsetCalibrationMode(HSP__ADC_0, kLPADC_OffsetCalibration16bitMode);
    LPADC_SetOffsetCalibrationMode(HSP__ADC_0, kLPADC_OffsetCalibration12bitMode);
    LPADC_DoOffsetCalibration(HSP__ADC_0);
    LPADC_DoAutoCalibration(HSP__ADC_0);

    LPADC_SetOffsetCalibrationMode(HSP__ADC_1, kLPADC_OffsetCalibration16bitMode);
    LPADC_SetOffsetCalibrationMode(HSP__ADC_1, kLPADC_OffsetCalibration12bitMode);
    LPADC_DoOffsetCalibration(HSP__ADC_1);
    LPADC_DoAutoCalibration(HSP__ADC_1);

    
    /* *********************************************************************************
     *  HSP__ADC_0                                                                     *
     *                  FIFO0                            FIFO1                         *
     *  Conversion 1    I_A   - ADC0_A3  PIO_3_6_H       -                             *
     *                                                                                 *
     *  HSP__ADC_1                                                                     *
     *                  FIFO0                            FIFO1                         *
     *  Conversion 1    -                                I_B   - ADC1_B2  PIO_3_21_H   *
     *  Conversion 2    I_C   - ADC1_A3  PIO_3_22_H      -                             *
     *  Conversion 3    -                                UDCBus - ADC1_B1  PIO_3_19_H  *
     *                                                                                 *
     **********************************************************************************/

    LPADC_GetDefaultConvCommandConfig(&mLpadcCommandConfigStruct);
    mLpadcCommandConfigStruct.hardwareAverageMode = kLPADC_HardwareAverageCount4;
    mLpadcCommandConfigStruct.sampleTimeMode      = kLPADC_SampleTimeADCK3;
    mLpadcCommandConfigStruct.sampleScaleMode     = kLPADC_SamplePartScale;
    mLpadcCommandConfigStruct.enableWaitTrigger   = FALSE;

    /* HSP__ADC_0 */
    /* Set conversion CMD1 configuration: ADC0_A3 (CUR_A) */
    mLpadcCommandConfigStruct.channelNumber            = M1_ADC0_PH_A;
    mLpadcCommandConfigStruct.chainedNextCommandNumber = 0U;    /* this is the last command */
    mLpadcCommandConfigStruct.sampleChannelMode        = kLPADC_SampleChannelSingleEndSideA;
    LPADC_SetConvCommandConfig(HSP__ADC_0, 1U, &mLpadcCommandConfigStruct);


    /* HSP__ADC_1 */
    /* Set conversion CMD1 configuration: ADC1_B2 (CUR_B) */
    mLpadcCommandConfigStruct.channelNumber            = M1_ADC1_PH_B;
    mLpadcCommandConfigStruct.chainedNextCommandNumber = 1U;
    mLpadcCommandConfigStruct.sampleChannelMode        = kLPADC_SampleChannelSingleEndSideB;
    LPADC_SetConvCommandConfig(HSP__ADC_1, 1U, &mLpadcCommandConfigStruct);

    /* Set conversion CMD2 configuration: ADC1_A3 (CUR_C) */
    mLpadcCommandConfigStruct.channelNumber            = M1_ADC1_PH_C;
    mLpadcCommandConfigStruct.chainedNextCommandNumber = 2U;
    mLpadcCommandConfigStruct.sampleChannelMode        = kLPADC_SampleChannelSingleEndSideA;
    LPADC_SetConvCommandConfig(HSP__ADC_1, 2U, &mLpadcCommandConfigStruct);
    
    /* Set conversion CMD3 configuration: ADC1_B1 (UDCBus) */
    mLpadcCommandConfigStruct.channelNumber            = M1_ADC1_UDCB;
    mLpadcCommandConfigStruct.chainedNextCommandNumber = 0U;    /* this is the last command */
    mLpadcCommandConfigStruct.sampleChannelMode        = kLPADC_SampleChannelSingleEndSideB;
    LPADC_SetConvCommandConfig(HSP__ADC_1, 3U, &mLpadcCommandConfigStruct);
    
    
    /* Set trigger configuration */
    LPADC_GetDefaultConvTriggerConfig(&mLpadcTriggerConfigStruct);
    mLpadcTriggerConfigStruct.targetCommandId       = 1U;  /* CMD1 executed */
    mLpadcTriggerConfigStruct.enableHardwareTrigger = 1U;  /* HW trigger ON */
    mLpadcTriggerConfigStruct.channelAFIFOSelect    = 0U;  /* channel A -> FIFO0 */
    mLpadcTriggerConfigStruct.channelBFIFOSelect    = 1U;  /* channel B -> FIFO1 */
    mLpadcTriggerConfigStruct.delayPower            = 0U;
    mLpadcTriggerConfigStruct.priority              = 0U;  /* highest priority */
    LPADC_SetConvTriggerConfig(HSP__ADC_0, 0U, &mLpadcTriggerConfigStruct); /* trigger 0 */
    LPADC_SetConvTriggerConfig(HSP__ADC_1, 4U, &mLpadcTriggerConfigStruct); /* trigger 4 */

    /* Offset filter window */
    g_sM1Curr3phDcBus.ui16OffsetFiltWindow = ADC_OFFSET_WINDOW;

    /* Enable the watermark interrupt */
    LPADC_EnableInterrupts(HSP__ADC_1, kLPADC_FIFO1WatermarkInterruptEnable);
    EnableIRQ(HSP_ADC1_IRQn);
    NVIC_SetPriority(HSP_ADC1_IRQn, 1U);
}

/*!
 * @brief   void InitQTMR1(void)
 *           - Initialization of the TMR1 peripheral
 *           - Performs slow control loop counter
 *
 * @param   void
 *
 * @return  none
 */
void InitQTMR1(void)
{
    uint16_t ui16SpeedLoopFreq       = g_sClockSetup.ui16M1SpeedLoopFreq;
    uint32_t ui32FastPeripheralClock = g_sClockSetup.ui32FastPeripheralClock;
    uint16_t ui16CompareReg          = (ui32FastPeripheralClock / (16U * ui16SpeedLoopFreq));

    CLOCK_EnableClock(kCLOCK_MAIN_hsp_qtimer1);

    /* QTMR1_CTRL: CM=0,PCS=0,SCS=0,ONCE=0,LENGTH=1,DIR=0,COINIT=0,OUTMODE=0 */
    /* Stop all functions of the timer */
    HSP__QTMR_1->CHANNEL[0].CTRL = 0x20;

    /* TMR0_SCTRL: all fields cleared */
    HSP__QTMR_1->CHANNEL[0].SCTRL = 0x00;
    HSP__QTMR_1->CHANNEL[0].LOAD  = 0x00;

    HSP__QTMR_1->CHANNEL[0].COMP1  = ui16CompareReg;
    HSP__QTMR_1->CHANNEL[0].CMPLD1 = ui16CompareReg;

    /* Enable compare 1 interrupt and compare 1 preload */
    HSP__QTMR_1->CHANNEL[0].CSCTRL = 0x41;
    HSP__QTMR_1->CHANNEL[0].CSCTRL |= TMR_CSCTRL_DBG_EN(1U);

    /* Primary Count Source to IP_bus_clk */
    HSP__QTMR_1->CHANNEL[0].CTRL |= TMR_CTRL_PCS(0x0C);

    /* Reset counter register */
    HSP__QTMR_1->CHANNEL[0].CNTR = 0x00;

    /* Run counter */
    HSP__QTMR_1->CHANNEL[0].CTRL |= TMR_CTRL_CM(0x01);

    /* Enable & setup interrupt from TMR1 */
    EnableIRQ(HSP_QTMR1_IRQn);
    NVIC_SetPriority(HSP_QTMR1_IRQn, 2U);
}

/*!
 * @brief   void M1_InitPWM(void)
 *           - Initialization of the eFlexPWMA peripheral for motor M1
 *           - 3-phase center-aligned PWM
 *
 * @param   void
 *
 * @return  none
 */
void M1_InitPWM(void)
{
    /* Enable PWM1 clock */
    CLOCK_EnableClock(kCLOCK_MAIN_hsp_flexpwm1);

    /* PWM base pointer (affects the entire initialization) */
    PWM_Type *PWMBase = (PWM_Type *)HSP__FLEXPWM_1;

    /* Full and Half cycle reload */
    PWMBase->SM[0].CTRL |= PWM_CTRL_FULL_MASK | PWM_CTRL_HALF_MASK;
    PWMBase->SM[1].CTRL |= PWM_CTRL_FULL_MASK | PWM_CTRL_HALF_MASK;
    PWMBase->SM[2].CTRL |= PWM_CTRL_FULL_MASK | PWM_CTRL_HALF_MASK;

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
    PWMBase->SM[0].VAL4 = PWM_VAL4_VAL4((uint16_t)((-(g_sClockSetup.ui16M1PwmModulo / 2) + (g_sClockSetup.ui16M1PwmDeadTime / 2))));
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

    /* Modules one and two get clock from module zero */
    PWMBase->SM[1].CTRL2 = (PWMBase->SM[1].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);
    PWMBase->SM[2].CTRL2 = (PWMBase->SM[2].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);

    /* Master reload active for modules one and two */
    PWMBase->SM[1].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;
    PWMBase->SM[2].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;

    /* Master reload is generated every one opportunity */
    PWMBase->SM[0].CTRL = (PWMBase->SM[0].CTRL & ~PWM_CTRL_LDFQ_MASK) | PWM_CTRL_LDFQ(M1_FOC_FREQ_VS_PWM_FREQ - 1);

    /* Master sync active for modules one and two */
    PWMBase->SM[1].CTRL2 = (PWMBase->SM[1].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);
    PWMBase->SM[2].CTRL2 = (PWMBase->SM[2].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);

    /* Fault 0 active in logic level one, automatic clearing */
    PWMBase->FAULT[0].FCTRL = (PWMBase->FAULT[0].FCTRL & ~PWM_FCTRL_FLVL_MASK) | PWM_FCTRL_FLVL(0x1);
    PWMBase->FAULT[0].FCTRL = (PWMBase->FAULT[0].FCTRL & ~PWM_FCTRL_FAUTO_MASK) | PWM_FCTRL_FAUTO(0x1);

    /* Clear fault flags */
    PWMBase->FAULT[0].FSTS = (PWMBase->FAULT[0].FSTS & ~PWM_FSTS_FFLAG_MASK) | PWM_FSTS_FFLAG(0xF);

    /* PWMs are re-enabled at PWM full cycle */
    PWMBase->FAULT[0].FSTS = (PWMBase->FAULT[0].FSTS & ~PWM_FSTS_FFULL_MASK) | PWM_FSTS_FFULL(0x1);

    /* PWM fault filter - 5 Fast peripheral clocks sample rate, 5 agreeing samples to activate */
    PWMBase->FAULT[0].FFILT = (PWMBase->FAULT[0].FFILT & ~PWM_FFILT_FILT_PER_MASK) | PWM_FFILT_FILT_PER(5);
    PWMBase->FAULT[0].FFILT = (PWMBase->FAULT[0].FFILT & ~PWM_FFILT_FILT_CNT_MASK) | PWM_FFILT_FILT_CNT(5);

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
 * @brief   void M1_InitQD(void)
 *           - Initialization of the Quadrature Encoder peripheral
 *           - Performs speed and position sensor processing
 *
 * @param   void
 *
 * @return  none
 */
void M1_InitQD(void)
{
    /* Enable clock to EQDC1 module (QDC1 on RT2660) */
    CLOCK_EnableClock(kCLOCK_MAIN_hsp_qdc1);

    HSP__EQDC_1->CTRL2 &= ~EQDC_CTRL2_LDMOD_MASK;
    HSP__EQDC_1->CTRL  &= ~EQDC_CTRL_LDOK_MASK;

    HSP__EQDC_1->CTRL |= EQDC_CTRL_LDOK_MASK;
    while (HSP__EQDC_1->CTRL & EQDC_CTRL_LDOK_MASK)
    {
    }

    /* Pass initialization data into encoder driver structure */
    g_sM1Enc.pui32QdBase     = (EQDC_Type *)HSP__EQDC_1;
    g_sM1Enc.ui16Pp          = M1_MOTOR_PP;
    g_sM1Enc.bDirection      = M1_POSPE_ENC_DIRECTION;
    g_sM1Enc.ui16PulseNumber = M1_POSPE_ENC_PULSES;

    /* Enable modulo counting and revolution counter increment on roll-over */
    HSP__EQDC_1->CTRL2 = EQDC_CTRL2_REVMOD_MASK;

    /* Prescaler for the timer within QDC */
    HSP__EQDC_1->FILT  = EQDC_FILT_FILT_CNT(2) | EQDC_FILT_FILT_PER(1) | EQDC_FILT_PRSC(6);
    HSP__EQDC_1->CTRL2 = EQDC_CTRL2_REVMOD_MASK | EQDC_CTRL2_PMEN_MASK;

    /* EQDC_1 timer clock comes from MAIN HSP bus (same as PWM/QTimer) */
    g_sM1Enc.ui32QDTimerFrequency = (CLOCK_GetRootClockFreq(kCLOCK_Root_CGU_MAIN_ROOTCLK)) >>
                                    ((HSP__EQDC_1->FILT & EQDC_FILT_PRSC_MASK) >> EQDC_FILT_PRSC_SHIFT);

    /* Position gain */
    g_sM1Enc.i32Q10Cnt2PosGain = ((0xffffffffU / (4 * g_sM1Enc.ui16PulseNumber)) * 0x400U);
    /* Speed conversion constant */
    g_sM1Enc.f32SpeedCalConst = (frac32_t)((2 * FLOAT_PI * g_sM1Enc.ui32QDTimerFrequency /
                                             (4 * g_sM1Enc.ui16PulseNumber * M1_N_MAX)) * 0x8000000U);
    /* Coefficient converting fractional speed into mechanical angular speed */
    g_sM1Enc.fltSpeedFracToAngularCoeff = (float_t)(M1_N_MAX);

    g_sM1Enc.f32PosMechInit   = FRAC32(0.0);
    g_sM1Enc.f32PosMechOffset = FRAC32(0.0);

    M1_MCDRV_ENC_SET_DIRECTION(&g_sM1Enc);

    /* Initialization modulo counter */
    M1_MCDRV_ENC_SET_PULSES(&g_sM1Enc);
}

/*!
 * @brief   void InitCMP(void)
 *           - Initialization of the comparator module for dc-bus over-current
 *             detection to generate PWM fault
 *
 * @param   void
 *
 * @return  none
 */
void InitCMP(void)
{
    /* Enable CMP clock */
    CLOCK_EnableClock(kCLOCK_WAKE_acmp2);

    /* Enable high speed */
    WAKE__ACMP_2->C0 |= CMP_C0_PMODE_MASK;

    /* Configure channel: positive port input (3U) from DAC, negative port input (7U) from minus mux */
    WAKE__ACMP_2->C1 |= CMP_C1_PSEL(3U) | CMP_C1_MSEL(7U);

    /* Voltage reference 3V PAD, DAC value 150, High speed mode */
    WAKE__ACMP_2->C1 |= (CMP_C1_VRSEL(1U) | CMP_C1_VOSEL(150U) | CMP_C1_DACEN_MASK | CMP_C1_DMODE_MASK);

    /* Enable CMP */
    WAKE__ACMP_2->C0 |= CMP_C0_EN_MASK;
}
