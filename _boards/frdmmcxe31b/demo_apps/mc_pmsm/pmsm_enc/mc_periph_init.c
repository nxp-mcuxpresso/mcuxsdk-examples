/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021, 2025 NXP
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
static void InitBCTU(void);
static void InitLCU(void);
static void InitEMIOS(void);
static void InitPWM(void);
static void InitTRGMUX(void);
static void InitClock(void);
static void InitPIT(void);
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

/* Configuration structure for 3-phase PWM mc driver */
mcdrv_pwm3ph_emios_t g_sM1Pwm3ph;

/* Structure for current and voltage measurement*/
mcdrv_adc_t g_sM1AdcSensor;

///* Structure for Encoder driver */
//mcdrv_eqd_enc_t g_sM1Enc;

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
  
    /* Init Body Cross-triggering Unit(BCTU) */
    InitBCTU();
    
    /* Init Logic Control Unit (LCU) */
    InitLCU();
  
    /* Init ADC */
    InitADC();
    
    /* Init eMIOS (PWM) */
    InitEMIOS();
    
//    /* 6-channel PWM peripheral init */
//    InitPWM();
    
    /* Init INPUTMUX */
    InitTRGMUX();
    
    /* Slow loop timer init */
    InitPIT();
	
//    /* Qudrature decoder peripheral init */
//    InitQD();
//    
//#if M1_FAULT_ENABLE    
//    /* Comparator CMP */
//    InitCMP();   
//#endif /* M1_FAULT_ENABLE */

    
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
   
    /* The following peripherals use SYSTEM_CLK as APB/IPS clock: FlexPWM0/1, ADC0~3 */
    g_sClockSetup.ui32FastPeripheralClock = CLOCK_GetFreq(kCLOCK_CoreSysClk);
    g_sClockSetup.ui32CpuFrequency = CLOCK_GetFreq(kCLOCK_CoreSysClk);

//    /* Parameters for motor M1 */
//    g_sClockSetup.ui16M1PwmFreq   = M1_PWM_FREQ; /* 16 kHz */
//    g_sClockSetup.ui16M1PwmModulo = (g_sClockSetup.ui32FastPeripheralClock) / g_sClockSetup.ui16M1PwmFreq;
//    ui32CyclesNumber = ((M1_PWM_DEADTIME * (g_sClockSetup.ui32FastPeripheralClock / 1000000U)) / 1000U);
//    g_sClockSetup.ui16M1PwmDeadTime   = ui32CyclesNumber;
//    g_sClockSetup.ui16M1SpeedLoopFreq = M1_SPEED_LOOP_FREQ; /* 1kHz */
    
}


/*!
 * @brief   void InitEMIOS(void)
 *
 * @param   void
 *
 * @return  none
 */
static void InitEMIOS(void)
{
//    uint32_t ui32EmiosClk;
  
    CLOCK_EnableClock(kCLOCK_Emios0);
//    ui32EmiosClk = CLOCK_GetFreq(kCLOCK_EmiosClk);
       
    /* Ch1-Ch3 are used for PWMA-PWMC signal generation */
    // EMIOS_Init(EMIOS0,EMIOS_CH1,EMIOS_MODE_OPWMB_TB(EMIOS_BUS_BCDE,EMIOS_B1M_F,EMIOS_EDPOL_OUT_A_SET_B_CLR,APP_OPWMB_A_DC_0,APP_OPWMB_B_DC_0));
    EMIOS_0->UC[1U].C |= EMIOS_C_MODE(0x60U | 0x00U) | EMIOS_C_EDPOL(1U) | EMIOS_C_EDSEL(0U) | EMIOS_C_BSL(1U)| EMIOS_C_UCPRE(0U);
    EMIOS_0->UC[1U].C2 |= EMIOS_C2_UCRELDEL_INT(0U) | EMIOS_C2_UCPRECLK(0U) | EMIOS_C2_UCEXTPRE(0U);
    EMIOS_0->UC[1U].A = EMIOS_A_A(2000U);
    EMIOS_0->UC[1U].B = EMIOS_B_B(6000U);
    EMIOS_0->UC[1U].ALTA = EMIOS_ALTA_ALTA(1U);
    EMIOS_0->UC[1U].CNT |= EMIOS_CNT_C(0U);
      
    //EMIOS_Init(EMIOS0,EMIOS_CH2,EMIOS_MODE_OPWMB_TB(EMIOS_BUS_BCDE,EMIOS_B1M_F,EMIOS_EDPOL_OUT_A_SET_B_CLR,APP_OPWMB_A_DC_0,APP_OPWMB_B_DC_0));
    EMIOS_0->UC[2U].C |= EMIOS_C_MODE(0x60U | 0x00U) | EMIOS_C_EDPOL(1U) | EMIOS_C_EDSEL(0U) | EMIOS_C_BSL(1U)| EMIOS_C_UCPRE(0U);
    EMIOS_0->UC[2U].C2 |= EMIOS_C2_UCRELDEL_INT(0U) | EMIOS_C2_UCPRECLK(0U) | EMIOS_C2_UCEXTPRE(0U);
    EMIOS_0->UC[2U].A = EMIOS_A_A(2000U);
    EMIOS_0->UC[2U].B = EMIOS_B_B(6000U);
    EMIOS_0->UC[2U].ALTA = EMIOS_ALTA_ALTA(1U);
    EMIOS_0->UC[2U].CNT |= EMIOS_CNT_C(0U);
    
    //EMIOS_Init(EMIOS0,EMIOS_CH3,EMIOS_MODE_OPWMB_TB(EMIOS_BUS_BCDE,EMIOS_B1M_F,EMIOS_EDPOL_OUT_A_SET_B_CLR,APP_OPWMB_A_DC_0,APP_OPWMB_B_DC_0));
    EMIOS_0->UC[3U].C |=  EMIOS_C_MODE(0x60U | 0x00U) | EMIOS_C_EDPOL(1U) | EMIOS_C_EDSEL(0U) | EMIOS_C_BSL(1U)| EMIOS_C_UCPRE(0U);
    EMIOS_0->UC[3U].C2 |= EMIOS_C2_UCRELDEL_INT(0U) | EMIOS_C2_UCPRECLK(0U) | EMIOS_C2_UCEXTPRE(0U);
    EMIOS_0->UC[3U].A = EMIOS_A_A(2000U);
    EMIOS_0->UC[3U].B = EMIOS_B_B(6000U);
    EMIOS_0->UC[3U].ALTA = EMIOS_ALTA_ALTA(1U);
    EMIOS_0->UC[3U].CNT |= EMIOS_CNT_C(0U);
    
    /* Ch0 is used for time base and reload signal generation */
    //EMIOS_Init(EMIOS0,EMIOS_CH0,EMIOS_MODE_MCB_UP_ICLK(EMIOS_PRESC_1,EMIOS_PRESC_CLOCK,0x0001U,APP_MC_A1));
    EMIOS_0->UC[0U].C |= EMIOS_C_MODE(0x50U) | EMIOS_C_EDPOL(0U) | EMIOS_C_EDSEL(0U) | EMIOS_C_BSL(3U)| EMIOS_C_UCPRE(0U);
    EMIOS_0->UC[0U].C2 |= EMIOS_C2_UCRELDEL_INT(0U) | EMIOS_C2_UCPRECLK(0U) | EMIOS_C2_UCEXTPRE(0U);
    EMIOS_0->UC[0U].A = EMIOS_A_A(8000);
    EMIOS_0->UC[0U].B = EMIOS_B_B(0U);
    EMIOS_0->UC[0U].ALTA = EMIOS_ALTA_ALTA(0U);
    EMIOS_0->UC[0U].CNT |= EMIOS_CNT_C(0x0001U);
        
    /*Ch4 is used for ADC triggering through BCTU */
    //EMIOS_Init(EMIOS0,EMIOS_CH4,EMIOS_MODE_OPWMB_TB(EMIOS_BUS_A,EMIOS_B1M_F,EMIOS_EDPOL_OUT_A_SET_B_CLR,APP_OPWMB_A_DC_0,APP_OPWMB_B_DC_0));
    EMIOS_0->UC[4U].C |= EMIOS_C_MODE(0x60U | 0x00U) | EMIOS_C_EDPOL(1U) | EMIOS_C_EDSEL(0U) | EMIOS_C_BSL(1U)| EMIOS_C_UCPRE(0U);
    EMIOS_0->UC[4U].C2 |= EMIOS_C2_UCRELDEL_INT(0U) | EMIOS_C2_UCPRECLK(0U) | EMIOS_C2_UCEXTPRE(0U);
    EMIOS_0->UC[4U].A = EMIOS_A_A(1U);
    EMIOS_0->UC[4U].B = EMIOS_B_B(96);
    EMIOS_0->UC[4U].ALTA = EMIOS_ALTA_ALTA(0U);
    EMIOS_0->UC[4U].CNT |= EMIOS_CNT_C(0x0U);    
    
    //EMIOS_EnableUpdate(EMIOS0, EMIOS_CH1|EMIOS_CH2|EMIOS_CH3|EMIOS_CH4);
    EMIOS_0->OUDIS |= EMIOS_OUDIS_OU1(0U);
    EMIOS_0->OUDIS |= EMIOS_OUDIS_OU2(0U);
    EMIOS_0->OUDIS |= EMIOS_OUDIS_OU3(0U);
    EMIOS_0->OUDIS |= EMIOS_OUDIS_OU4(0U);
  
//    EMIOS_Enable(EMIOS0, EMIOS_CH0|EMIOS_CH23);
    EMIOS_0->UC[0].C |= EMIOS_C_UCPREN(1U);

//    EMIOS_EnablePrescaler(EMIOS0, 1u);
    EMIOS_0->MCR |= EMIOS_MCR_GTBE_MASK | EMIOS_MCR_GPREN_MASK | EMIOS_MCR_GPRE(1U);
    
    
    /* Initialize MC driver */
//    g_sM1Pwm3ph.pui32PwmBaseAddress = (eMIOS_0_Type *)EMIOS_0;
    g_sM1Pwm3ph.pui32PwmBaseAddress = (EMIOS_Type *)EMIOS_0;

//    g_sM1Pwm3ph.ui16PhASubNum = 0U; /* PWMA phase A sub-module number */
//    g_sM1Pwm3ph.ui16PhBSubNum = 1U; /* PWMA phase B sub-module number */
//    g_sM1Pwm3ph.ui16PhCSubNum = 2U; /* PWMA phase C sub-module number */
//
//    g_sM1Pwm3ph.ui16FaultFixNum = M1_FAULT_NUM; /* PWMA fixed-value over-current fault number */
//    g_sM1Pwm3ph.ui16FaultAdjNum = M1_FAULT_NUM; /* PWMA adjustable over-current fault number */
    
    
    
    

}

static void InitBCTU(void)
{
  
    bctu_config_t config;
    bctu_trig_config_t trigConfig;

    bctu_convlist_config_t convListConfig[3U] = {
      {
            // LIST0
            .lastChan           = false,
            .lastChanPlusOne    = false,
            .waitTrig           = false,
            .waitTrigPlusOne    = false,
            .adcChan            = 43U, // CURA_S19_ADC0_CHN43
            .adcChanPlusOne     = 41U, // CURC_S17_ADC1_CHN41
      },
      {     
            // LIST1
            .lastChan           = false,
            .lastChanPlusOne    = false,
            .waitTrig           = false,
            .waitTrigPlusOne    = false,
            .adcChan            = 40U, // CURB_S16_ADC0_CHN40
            .adcChanPlusOne     = 41U, // CURC_S17_ADC1_CHN41
      },
      {     
            // LIST2
            .lastChan           = false,
            .lastChanPlusOne    = true,
            .waitTrig           = false,
            .waitTrigPlusOne    = false,
            .adcChan            = 88U, // VOLT_DCB
            .adcChanPlusOne     = 0U, // Dummy
      },

    };

    BCTU_GetDefaultConfig(&config);
    config.writeProtect = kBCTU_ProtectDis_Permanent;
    BCTU_Init(BCTU, &config);
    
    trigConfig.trigIndex = kBCTU_TrigSourceEmios0Ch4; //DEMO_BCTU_TRIGGER_INDEX;
    trigConfig.chanAddr = 0U; //DEMO_BCTU_TRIG_CHAN_ADDR;
    trigConfig.dataDest = kBCTU_DataDest_Fifo1;
    trigConfig.enableLoop = false;
    //trigConfig.targetAdc = (kBCTU_TrigAdc_0 | kBCTU_TrigAdc_1 | kBCTU_TrigAdc_2);
    trigConfig.targetAdc = (kBCTU_TrigAdc_0 | kBCTU_TrigAdc_1);
    trigConfig.trigRes = kBCTU_TrigRes_ConvList;
    BCTU_SetTrigConfig(BCTU, &trigConfig);

    for(uint8_t index = 0U; index < 3U; ++index)
    {
        BCTU_SetConvListConfig(BCTU, &(convListConfig[index]), index);
    }

    BCTU_SetFifoWaterMark(BCTU, kBCTU_Fifo_1, (6U - 1U));
    BCTU_EnableFifoInt(BCTU, kBCTU_Fifo_1_Int, true);
    EnableIRQ(BCTU_IRQn);

}

static void InitLCU(void)
{
    /* Structure of initialize LCU. */
    lcu_output_config_t outputConfig;

    /* Init pit module. */
    LCU_Init(LCU_1);

    /* Get LCU output default configuration. */
    LCU_GetOutputDefaultConfig(&outputConfig);
    
    outputConfig.outputEnable = true;
    outputConfig.softwareOverrideEnable = false;
    outputConfig.outputPolarity = kLCU_OutputPolarityNotInverted;
    outputConfig.forceInputSentivity = 0U;
    
    outputConfig.riseFilter = 96U;
    outputConfig.fallFilter = 0U;
    outputConfig.lutValue = 0xAAAAU;
    
    /* Init LCU output. */
    LCU_OutputInit(LCU_1, kLCU_Lc1Output0, &outputConfig);
    
    outputConfig.riseFilter = 96U;
    outputConfig.fallFilter = 0U;
    outputConfig.lutValue = 0x5555U;
    
    /* Init LCU output. */
    LCU_OutputInit(LCU_1, kLCU_Lc1Output1, &outputConfig);
    
    outputConfig.riseFilter = 96U;
    outputConfig.fallFilter = 0U;
    outputConfig.lutValue = 0xCCCCU;
    
    /* Init LCU output. */
    LCU_OutputInit(LCU_1, kLCU_Lc1Output2, &outputConfig);
    
    outputConfig.riseFilter = 96U;
    outputConfig.fallFilter = 0U;
    outputConfig.lutValue = 0x3333U;
    
    /* Init LCU output. */
    LCU_OutputInit(LCU_1, kLCU_Lc1Output3, &outputConfig);

    
    outputConfig.riseFilter = 96U;
    outputConfig.fallFilter = 0U;
    outputConfig.lutValue = 0xAAAAU;
  
    /* Init LCU output. */
    LCU_OutputInit(LCU_1, kLCU_Lc2Output2, &outputConfig);
    
    outputConfig.riseFilter = 96U;
    outputConfig.fallFilter = 0U;
    outputConfig.lutValue = 0x5555U;
    
    
    /* Init LCU output. */
    LCU_OutputInit(LCU_1, kLCU_Lc2Output3, &outputConfig);
    
    /* Select LC input source. */
    LCU_MuxSelect(LCU_1, kLCU_Lc1Input0, kLCU_MuxSelInput0);
    LCU_MuxSelect(LCU_1, kLCU_Lc1Input1, kLCU_MuxSelInput1);
    LCU_MuxSelect(LCU_1, kLCU_Lc2Input0, kLCU_MuxSelInput2);
  
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

    adc_config_t adcConfig;
    adc_calibration_config_t calibrationConfig;

    ADC_GetDefaultConfig(&adcConfig);
    adcConfig.clockFrequency    = kADC_ModuleClockFreqDivide2;
    adcConfig.bctuMode          = kADC_BctuTrig;
    adcConfig.convAvg           = kADC_ConvAvgDisable;
    ADC_Init(ADC_0, &adcConfig);
    ADC_Init(ADC_1, &adcConfig);
    ADC_Init(ADC_2, &adcConfig);

    calibrationConfig.enableAverage        = false;
//    calibrationConfig.sampleTime           = kADC_SampleTime8;
//    calibrationConfig.averageSampleNumbers = kADC_AverageSampleNumbers4;

    if (!(ADC_DoCalibration(ADC_0, &calibrationConfig)))
    {
//        PRINTF("ADC 0 calibration failed\r\n");
    }
    if (!(ADC_DoCalibration(ADC_1, &calibrationConfig)))
    {
//        PRINTF("ADC 1 calibration failed\r\n");
    }
    if (!(ADC_DoCalibration(ADC_2, &calibrationConfig)))
    {
//        PRINTF("ADC 2 calibration failed\r\n");
    }
    
    
    
    

    /* Start the ADC conversion chain to execute the conversion. */
//    ADC_StartConvChain(ADC_0, kADC_NormalConvScanMode);
  
//    lpadc_conv_trigger_config_t lpadcTriggerConfig;
//    lpadc_conv_command_config_t lpadcCommandConfig;
//    lpadc_config_t lpadcConfig;
//    
//    /* Init the lpadcConfig struct */
//    LPADC_GetDefaultConfig(&lpadcConfig);
//    lpadcConfig.enableAnalogPreliminary = true;
//    lpadcConfig.referenceVoltageSource = kLPADC_ReferenceVoltageAlt3;
//    lpadcConfig.conversionAverageMode = kLPADC_ConversionAverage1;
//    
//    /* Release peripheral reset */
//    RESET_ReleasePeripheralReset(kADC0_RST_SHIFT_RSTn);
//    RESET_ReleasePeripheralReset(kADC1_RST_SHIFT_RSTn);
//
//    /* Attach peripheral clock */
//    CLOCK_SetClockDiv(kCLOCK_DivADC, 1u);
//    CLOCK_AttachClk(kFRO_LF_DIV_to_ADC);
//
//    LPADC_Init(ADC0, &lpadcConfig);
//    LPADC_DoOffsetCalibration(ADC0);
//    LPADC_DoAutoCalibration(ADC0);
//    
//    LPADC_Init(ADC1, &lpadcConfig);
//    LPADC_DoOffsetCalibration(ADC1);
//    LPADC_DoAutoCalibration(ADC1);
//        
//    LPADC_GetDefaultConvCommandConfig(&lpadcCommandConfig);
//    lpadcCommandConfig.sampleChannelMode = kLPADC_SampleChannelSingleEndSideA;
//    lpadcCommandConfig.conversionResolutionMode = kLPADC_ConversionResolutionStandard;
//    lpadcCommandConfig.sampleTimeMode = kLPADC_SampleTimeADCK3;
//      
//    /* SET VOLT_DCB_CHANNEL_NUMBER (ADC0) */
//    lpadcCommandConfig.channelNumber = VOLT_DCB_CHANNEL_NUMBER;
//    lpadcCommandConfig.chainedNextCommandNumber = 0U;
//    LPADC_SetConvCommandConfig( ADC0, 1U, &lpadcCommandConfig );
//    
//    /* Init triggers (use trigger 0). */
//    LPADC_GetDefaultConvTriggerConfig(&lpadcTriggerConfig);
//    lpadcTriggerConfig.targetCommandId = 1U;
//    lpadcTriggerConfig.enableHardwareTrigger = true;
//    LPADC_SetConvTriggerConfig(ADC0, 0U, &lpadcTriggerConfig);
//    
//    /* SET CURRENTS CHANNELS (ADC1) */
//    lpadcCommandConfig.channelNumber = CUR_A_CHANNEL_NUMBER;
//    lpadcCommandConfig.chainedNextCommandNumber = 2U;
//    LPADC_SetConvCommandConfig( ADC1, 1U, &lpadcCommandConfig );
//
//    lpadcCommandConfig.channelNumber = CUR_B_CHANNEL_NUMBER;
//    lpadcCommandConfig.chainedNextCommandNumber = 3U;
//    LPADC_SetConvCommandConfig( ADC1, 2U, &lpadcCommandConfig );        
//                
//    lpadcCommandConfig.channelNumber = CUR_C_CHANNEL_NUMBER;
//    lpadcCommandConfig.chainedNextCommandNumber = 0U;
//    LPADC_SetConvCommandConfig( ADC1, 3U, &lpadcCommandConfig ); 
//    
//    /* Init triggers (use trigger 0). */
//    LPADC_GetDefaultConvTriggerConfig(&lpadcTriggerConfig);
//    lpadcTriggerConfig.targetCommandId = 1U;
//    lpadcTriggerConfig.enableHardwareTrigger = true;
//    LPADC_SetConvTriggerConfig(ADC1, 0U, &lpadcTriggerConfig);
//       
//    /* Set watermark level selection */
//    ADC1->FCTRL |= ADC_FCTRL_FWMARK(2);
//    
//    /* Enable the watermark interrupt. */
//    LPADC_EnableInterrupts(ADC1, kLPADC_FIFO0WatermarkInterruptEnable);
//    NVIC_SetPriority(ADC1_IRQn, 0U);
//    NVIC_EnableIRQ(ADC1_IRQn);  
//      
//    /* ADC0 base address */
//    g_sM1AdcSensor.pToAdcBase = ADC0;

}


/*!
 * @brief   void INPUTMUX(void)
 *
 * @param   void
 *
 * @return  none
 */
static void InitTRGMUX(void)
{
  
    CLOCK_EnableClock(kCLOCK_Trgmux);   
     
    //                              TRGMUX_OUT[0-3], OUT_1
    TRGMUX_SetTriggerSource(TRGMUX, kTRGMUX_ExtOut0, kTRGMUX_TriggerInput1, kTRGMUX_SourceEmios0IppDoEmiosCh4);
    
    // LCU1
    TRGMUX_SetTriggerSource(TRGMUX, kTRGMUX_Lcu1_0, kTRGMUX_TriggerInput0, kTRGMUX_SourceEmios0IppDoEmiosCh1);
    TRGMUX_SetTriggerSource(TRGMUX, kTRGMUX_Lcu1_0, kTRGMUX_TriggerInput1, kTRGMUX_SourceEmios0IppDoEmiosCh2);
    TRGMUX_SetTriggerSource(TRGMUX, kTRGMUX_Lcu1_0, kTRGMUX_TriggerInput2, kTRGMUX_SourceEmios0IppDoEmiosCh3);

}


/*!
 * @brief   void InitPIT(void)
 *           - Initialization of the CTIMER0 peripheral
 *           - performs slow control loop counter
 *
 * @param   void
 *
 * @return  none
 */
static void InitPIT(void)
{
//    /* Clock setting */
//    RESET_PeripheralReset(kCTIMER0_RST_SHIFT_RSTn);
//    CLOCK_AttachClk(kFRO_HF_to_CTIMER0); 
  
    /* Structure of initialize PIT */
    pit_config_t pitConfig;

    /*
     * pitConfig.enableRunInDebug = false;
     */
    PIT_GetDefaultConfig(&pitConfig);

    /* Init pit module */
    PIT_Init(PIT_0, &pitConfig);

    /* Set timer period for channel 0 */
    PIT_SetTimerPeriod(PIT_0, kPIT_Chnl_0, USEC_TO_COUNT(1000U, CLOCK_GetFreq(kCLOCK_Pit0Clk)));

    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(PIT_0, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(PIT0_IRQn);
    NVIC_SetPriority(PIT0_IRQn, 1);

    /* Start channel 0 */
    PIT_StartTimer(PIT_0, kPIT_Chnl_0);

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
//    CLOCK_EnableClock(kCLOCK_GateQDC0);    
//    RESET_ReleasePeripheralReset(kQDC0_RST_SHIFT_RSTn);
//    
//    /* Pass initialization data into encoder driver structure */
//    /* encoder position and speed measurement */
//    g_sM1Enc.pui32QdBase   = (EQDC_Type *)EQDC0;
//    g_sM1Enc.sTo.fltPGain  = M1_POSPE_TO_KP_GAIN;
//    g_sM1Enc.sTo.fltIGain  = M1_POSPE_TO_KI_GAIN;
//    g_sM1Enc.sTo.fltThGain = M1_POSPE_TO_THETA_GAIN;
//    g_sM1Enc.a32PosMeGain  = M1_POSPE_MECH_POS_GAIN;
//    g_sM1Enc.ui16Pp        = M1_MOTOR_PP;
//    g_sM1Enc.bDirection    = M1_POSPE_ENC_DIRECTION;
//    g_sM1Enc.fltSpdEncMin  = M1_POSPE_ENC_N_MIN;
//    g_sM1Enc.ui16PulseNumber = M1_POSPE_ENC_PULSES;
//    
//    /* Quadrature pulses per one revolution */
//    M1_MCDRV_QD_SET_PULSES(&g_sM1Enc); 
//    /* Set encoder direction */
//    M1_MCDRV_QD_SET_DIRECTION(&g_sM1Enc); 
//    /* Enable modulo counting and revolution counter increment on roll-over */
//    EQDC0->CTRL2 = EQDC_CTRL2_REVMOD_MASK;

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
    
//    /* Attach peripheral clock */
//    CLOCK_AttachClk(kFRO_LF_DIV_to_CMP2);
//    CLOCK_SetClockDiv(kCLOCK_DivCMP2_FUNC, 1U);
//
//    /* Enable CMP2 and CMP2_DAC */
//    SPC0->ACTIVE_CFG1 |= ((1U << 18U) | (1U << 22U));
//  
//    lpcmp_config_t mLpcmpConfigStruct;
//    lpcmp_dac_config_t mLpcmpDacConfigStruct;
//    
//    /* Get default configuration */
//    LPCMP_GetDefaultConfig(&mLpcmpConfigStruct);
//    
//    /* Init the LPCMP module. */
//    LPCMP_Init(CMP2, &mLpcmpConfigStruct);
//
//    /* Configure the internal DAC to output half of reference voltage. */
//    mLpcmpDacConfigStruct.enableLowPowerMode     = false;
//    mLpcmpDacConfigStruct.referenceVoltageSource = kLPCMP_VrefSourceVin1;
//    mLpcmpDacConfigStruct.DACValue = (CMP_THRESHOLD); /* Overcurrent threshold */
//    LPCMP_SetDACConfig(CMP2, &mLpcmpDacConfigStruct);
//
//    /* Configure LPCMP input channels. */
//    LPCMP_SetInputChannels(CMP2, CMP_INPUT_CHANNEL, CMP_DAC_CHANNEL);
    
}
#endif /* M1_FAULT_ENABLE */

