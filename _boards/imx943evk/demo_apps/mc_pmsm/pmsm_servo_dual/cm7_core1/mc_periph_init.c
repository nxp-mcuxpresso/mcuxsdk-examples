/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021, 2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mc_periph_init.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/


/* Structure for 3-phase PWM MC driver */
mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;
mcdrv_pwm3ph_pwma_t g_sM2Pwm3ph;

/* Structure for BISS-C driver */
BISSC_Type g_sM1Enc = { .ui8DevDataLen = BISS_DEVICE_DATA_LEN, \
                          .ui8DevSTLen = BISS_DEVICE_ST_LEN, \
                          .ui8DevMTLen = BISS_DEVICE_MT_LEN};

/* Structure for EnDat2.2 driver */
mcdrv_endat2p2_t g_sM2Enc;

/* Structures for current and voltage measurement */
mcdrv_sinc_t g_sM1Curr3phDcBus = { .fltDCBvoltageScale = M1_U_DCB_MAX, \
                          .fltCurrentScale = M1_I_MAX, \
                          .ui32ENOB = 13U, \
                          .ui32OSR = (SINC1_OSR + 1U), \
                          .ui32ORD = SINC1_ORD};

mcdrv_sinc_t g_sM2Curr3phDcBus = { .fltDCBvoltageScale = M2_U_DCB_MAX, \
                          .fltCurrentScale = M2_I_MAX, \
                          .ui32ENOB = 13U, \
                          .ui32OSR = (SINC2_OSR + 1U), \
                          .ui32ORD = SINC2_ORD};

/* Clock setup structure */
clock_setup_t g_sClockSetup;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief   void MCDRV_Init(void)
 *           - Motor control driver main initialization
 *           - Calls initialization functions of peripherals required for motor
 *             control functionality
 *
 * @param   void
 *
 * @return  none
 */
void MCDRV_Init(void)
{
    /* Init application clock dependent variables */
    InitClock();

    /* Init TMR1 (slow loop counter) */
    M1_MCDRV_TMR_SLOWLOOP_INIT();

    /* 6-channel PWM peripheral init for M1 */
    M1_MCDRV_PWM_PERIPH_INIT();
    
    /* 6-channel PWM peripheral init for M2 */
    M2_MCDRV_PWM_PERIPH_INIT();
    
    /* Init SINC filters */
    M1_MCDRV_SINC_INIT();
    M2_MCDRV_SINC_INIT();
    
    /* Init BiSS-C */
    M1_MCDRV_BISS_PERIPH_INIT();
    
    /* Init EnDat2p2 */
    M2_MCDRV_ENDAT2P2_PERIPH_INIT();
    
}





/*

ksi = 1;
f0 = 100;
Ts = 0.0000625;
DiscMethodFactor = 2; !!!

M1_POSPE_TO_KP_GAIN = (4.0 * pi * ksi * f0) % (1256.64F)
M1_POSPE_TO_KI_GAIN = ((2*pi*f0)^2 * Ts)  % (24.6740F)
M1_POSPE_TO_THETA_GAIN = (Ts / pi / DiscMethodFactor)  % (0.0000198944 / 2)

*/

///* 16KHz */
//#define M1_POSPE_TO_KP_GAIN (1256.64F)
//#define M1_POSPE_TO_KI_GAIN (24.6740F)
//#define M1_POSPE_TO_THETA_GAIN (0.0000099472F) // DiscMethodFactor is 2!

/* 32KHz */
//#define M1_POSPE_TO_KP_GAIN (1256.64F)
//#define M1_POSPE_TO_KI_GAIN (12.3370F)
//#define M1_POSPE_TO_THETA_GAIN (0.0000049736F) // DiscMethodFactor is 2!

/* 64KHz */
#define M1_POSPE_TO_KP_GAIN (1256.64F)
#define M1_POSPE_TO_KI_GAIN (6.1685F)
#define M1_POSPE_TO_THETA_GAIN (0.0000024868) // DiscMethodFactor is 2!

/*!
 * @brief      Init BiSS master IP
 *
 * @param      void
 *
 * @return     none
 */
void InitBiSS1(void)
{
  /* BiSS 20MHz */
  clk_t bissClk = {
      .clkId = BISS_SYS_CLK_ROOT,
      .pclkId = kCLOCK_Syspll1dfs1div2, /* 400 MHz */
      .rate = BISS_SYS_CLK_FREQ,
      .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
  };    

  BLK_CTRL_WAKEUPMIX_Type *blk_ctrl = BLK_CTRL_WAKEUPMIX;
  
    CLOCK_SetParent(&bissClk);
    CLOCK_SetRate(&bissClk);
    CLOCK_EnableClock(bissClk.clkId);
  
  blk_ctrl->DIAG_ENCODER_MUX_SEL = blk_ctrl->DIAG_ENCODER_MUX_SEL |
    BLK_CTRL_WAKEUPMIX_DIAG_ENCODER_MUX_SEL_diag_enc2_sel(DIG_ENCODER_MUX_BISS);
  
  /* Select Motor controller 1 */
  BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, ETH2_SEL);
  SDK_DelayAtLeastUs(100U, SystemCoreClock);  
  
  /* BISSC M1 structure */    
  g_sM1Enc.mt = 0U;
  g_sM1Enc.st = 0U;
  g_sM1Enc.mt_offset = 0U;
  g_sM1Enc.st_offset = 0U;       
  g_sM1Enc.ui16Pp        = M1_MOTOR_PP;

  /* BiSS driver initialization */
  status_t status;
  uint8_t slvID;
  biss_slave_info_t *slv;
  status_t my_biss_status;
  biss_master_t *master;

  /* Disable BiSS_EOT interrupt */
  DisableIRQ(BISS_EOT_IRQn);
  blk_ctrl->BISS1_EOT_CTL = 0;
  
  master = BISS_MasterInit(BISS_BASE, BISS_SYS_CLK_FREQ, BISS_MA_CLK_FREQ, BISS_AGS_CLK_FREQ);  
  
  if (master == NULL)
  {
      my_biss_status = kStatus_Fail;
  }
  SDK_DelayAtLeastUs(100U, SystemCoreClock);
  
  BISS_InitBissSequence(master);
  SDK_DelayAtLeastUs(400U, SystemCoreClock);
  
  /* Broadcast Active all slaves */
  BISS_CMDProcess(master, BISS_CMD_IDS_BOARDCAST, BISS_CMD_BOARDCAST_CTRL_ACTIVATED);
  SDK_DelayAtLeastUs(400U, SystemCoreClock);
  
  status = BISS_SLVScan(master);
  if (status != kStatus_Success)
  {
      my_biss_status = kStatus_Fail;
  }
  
  for (slvID = 0; slvID < master->slvCnt; slvID++)
  {
      slv = BISS_SLVGet(master, slvID);
     
      if (slv->dataLen == 0)
          slv->dataLen = g_sM1Enc.ui8DevDataLen;
      if (slv->crcLen == 0)
          slv->crcLen = BISS_DEVICE_CRC_LEN;
  }
  
  /* Manually initialize the slaves */
  BISS_SLVSetSCD(master, 0, g_sM1Enc.ui8DevDataLen, BISS_DEVICE_CRC_LEN);
  
  BISS_ChangeTriggerMode(master, BISS_PIN_TRIGGER);
  
  /* Asign BiSS master module address */
  g_sM1Enc.pMaster = (biss_master_t *)master;

  /* Enable BiSS_EOT interrupt */
  EnableIRQ(BISS_EOT_IRQn);
  blk_ctrl->BISS1_EOT_CTL = 0x3;
}

/*!
 * @brief      Init EnDat2.2 master IP
 *
 * @param      void
 *
 * @return     none
 */
void InitEndat2p2(void)
{
    int ret, data;
  
    /* EnDat2.2 100MHz */
    clk_t endat2p2Clk = {
        .clkId = kCLOCK_Endat21,
        .pclkId = kCLOCK_Syspll1dfs1div2, /* 400 MHz */
        .rate = ENDAT2P2_CLK_48M,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };

    BLK_CTRL_WAKEUPMIX_Type *blk_ctrl = BLK_CTRL_WAKEUPMIX;

    CLOCK_SetParent(&endat2p2Clk);
    CLOCK_SetRate(&endat2p2Clk);
    CLOCK_EnableClock(endat2p2Clk.clkId);
 
    blk_ctrl->DIAG_ENCODER_MUX_SEL = blk_ctrl->DIAG_ENCODER_MUX_SEL |
      BLK_CTRL_WAKEUPMIX_DIAG_ENCODER_MUX_SEL_diag_enc1_sel(DIG_ENCODER_MUX_ENDAT2P2);

    blk_ctrl->ENDAT_STRETCHER_CTRL =
    BLK_CTRL_WAKEUPMIX_ENDAT_STRETCHER_CTRL_endat2p2_nstr_value(3) |
    BLK_CTRL_WAKEUPMIX_ENDAT_STRETCHER_CTRL_endat2p2_nstr_ctrl(1);
    
    g_sM2Enc.dev = ENDAT2P2_InitMaster(ENDAT2P2_1, ENDAT2P2_CLK_48M);
      
    ////////////////////////////////////////////////////////////
    /* Init encoder - ENDAT2P2_InitEncoder(dev); */
    
    ENDAT2P2_EncoderRest(g_sM2Enc.dev);
    SDK_DelayAtLeastUs((50 * 1000), SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    
    ENDAT2P2_ClearEncoderError(g_sM2Enc.dev);
    SDK_DelayAtLeastUs((50 * 1000), SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    ENDAT2P2_ClearEncoderWarning(g_sM2Enc.dev);
    SDK_DelayAtLeastUs((50 * 1000), SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    ENDAT2P2_GetEncoderInfo(g_sM2Enc.dev);

    ENDAT2P2_SetDataWordLength(g_sM2Enc.dev, g_sM2Enc.dev->pos_res);
    
    //////////////////////////////////////////////////////////
    
    //ENDAT2P2_EnableDelayCompensation(dev);

    ENDAT2P2_SetFTCLOCK(g_sM2Enc.dev, ENDAT2P2_CLK_8M); //ENDAT2P2_FTCLK

    /* Change tm to 3.75us */
    data = ENDAT2P2_GetParamWithPos(g_sM2Enc.dev, MRS_CODE_OPERATING_STATUS, ENDAT2P2_MEM_WORD_3);
    data = (data & (~3)) | 0x1;

    ENDAT2P2_SetParamWithPos(g_sM2Enc.dev, MRS_CODE_OPERATING_STATUS, ENDAT2P2_MEM_WORD_3, data);
    ENDAT2P2_EncoderRestWithPos(g_sM2Enc.dev);

    data = ENDAT2P2_GetParamWithPos(g_sM2Enc.dev, MRS_CODE_OPERATING_STATUS, ENDAT2P2_MEM_WORD_3);

    ENDAT2P2_SetRecoveryTimer(g_sM2Enc.dev, 0);
         
    ret = ENDAT2P2_GetEncoderError(g_sM2Enc.dev);
    ret = ENDAT2P2_GetEncoderWarning(g_sM2Enc.dev);

    if(g_sM2Enc.dev->cmd_set_2_2)
    {
        ENDAT2P2_ClearEncoderErrorWithPos(g_sM2Enc.dev);

        ret = ENDAT2P2_GetEncoderErrorWithPos(g_sM2Enc.dev);
        ret = ENDAT2P2_GetParamWithPos(g_sM2Enc.dev, MRS_CODE_PARAM_ENCODER_MANUFACTURER_PAGE1, ENDAT2P2_MEM_WORD_1);
    }

    ////////////////////////////////////////////////////////////////////////////
    /* EnDat HW Strobe Loop */

    endat2p2_mode_cmd_t cmd = ENDAT2P2_CMD_SEND_POSITION_VALUE;

    /* reset additional info's if present */
    ENDAT2P2_EncoderRest(g_sM2Enc.dev);
    SDK_DelayAtLeastUs(500U, SystemCoreClock);
    
    ENDAT2P2_CMDBuild(g_sM2Enc.dev, cmd, 0, 0);

    ENDAT2P2_CleanStatus(g_sM2Enc.dev);
    
    ENDAT2P2_SetHWStrobe(g_sM2Enc.dev, true);

    ENDAT2P2_CleanStatus(g_sM2Enc.dev);
    
    
    g_sM2Enc.ui16Pp        = M2_MOTOR_PP;
    
    
//    ENDAT2P2_SetHWStrobe(dev, false);
       
    /* Enable interrupt */
    ENDAT2P2_SetInterruptMask(g_sM2Enc.dev, ENDAT2P2_INTERRUPTMASKREGISTER_RECEIVE_REGISTER1_MASK_MASK);
    EnableIRQ(ENDAT2P2_IRQn);
    NVIC_SetPriority(ENDAT2P2_IRQn, 0U);

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
    g_sClockSetup.ui32FastPeripheralClock = (uint32_t)CLOCK_GetRate(kCLOCK_Buswakeup);
    g_sClockSetup.ui32CpuFrequency = (uint32_t)CLOCK_GetRate(kCLOCK_M71);    


    /* Parameters for motor M1 */
    g_sClockSetup.ui16M1PwmFreq   = M1_PWM_FREQ; /* 32 kHz */
    g_sClockSetup.ui16M1PwmModulo = (g_sClockSetup.ui32FastPeripheralClock) / g_sClockSetup.ui16M1PwmFreq;
    ui32CyclesNumber = ((M1_PWM_DEADTIME * (g_sClockSetup.ui32FastPeripheralClock / 1000000U)) / 1000U);
    g_sClockSetup.ui16M1PwmDeadTime   = ui32CyclesNumber;
    g_sClockSetup.ui16M1SpeedLoopFreq = M1_SPEED_LOOP_FREQ; /* 4kHz */
    
    /* Parameters for motor M2 */
    g_sClockSetup.ui16M2PwmFreq   = M2_PWM_FREQ; /* 32 kHz */
    g_sClockSetup.ui16M2PwmModulo = (g_sClockSetup.ui32FastPeripheralClock) / g_sClockSetup.ui16M2PwmFreq;
    ui32CyclesNumber = ((M2_PWM_DEADTIME * (g_sClockSetup.ui32FastPeripheralClock / 1000000U)) / 1000U);
    g_sClockSetup.ui16M2PwmDeadTime   = ui32CyclesNumber;
    g_sClockSetup.ui16M2SpeedLoopFreq = M2_SPEED_LOOP_FREQ; /* 4kHz */
}

/*!
 * @brief   void InitTMR1(void)
 *           - Initialization of the TMR1 peripheral
 *           - Performs slow control loop counter
 *
 * @param   void
 *
 * @return  none
 */
void InitTMR1(void)
{
    uint16_t ui16SpeedLoopFreq       = g_sClockSetup.ui16M1SpeedLoopFreq;
    uint32_t ui32FastPeripheralClock = g_sClockSetup.ui32FastPeripheralClock;
    uint16_t ui16CompareReg          = (ui32FastPeripheralClock / (16U * ui16SpeedLoopFreq));

//    CLOCK_EnableClock(kCLOCK_Qtimer1);

    /* TMR0_CTRL: CM=0,PCS=0,SCS=0,ONCE=0,LENGTH=1,DIR=0,COINIT=0,OUTMODE=0 */
    /* Stop all functions of the timer */
    TMR1->CHANNEL[0].CTRL = 0x20;

    /* TMR0_SCTRL: TCF=0,TCFIE=0,TOF=0,TOFIE=0,IEF=0,IEFIE=0,IPS=0,INPUT=0,
    Capture_Mode=0,MSTR=0,EEOF=0,VAL=0,FORCE=0,OPS=0,OEN=0 */
    TMR1->CHANNEL[0].SCTRL = 0x00;
    TMR1->CHANNEL[0].LOAD  = 0x00; /* Reset load register */

    TMR1->CHANNEL[0].COMP1  = ui16CompareReg; /* Set up compare 1 register */
    TMR1->CHANNEL[0].CMPLD1 = ui16CompareReg; /* Also set the compare preload register */

    /* TMR0_CSCTRL: DBG_EN=0,FAULT=0,ALT_LOAD=0,ROC=0,TCI=0,UP=0,OFLAG=0,TCF2EN=0,TCF1EN=1,
    TCF2=0,TCF1=0,CL2=0,CL1=1 */
    /* Enable compare 1 interrupt and compare 1 preload*/
    TMR1->CHANNEL[0].CSCTRL = 0x41;
    TMR1->CHANNEL[0].CSCTRL |= TMR_CSCTRL_DBG_EN(1U);

    /* Primary Count Source to IP_bus_clk */
    TMR1->CHANNEL[0].CTRL |= TMR_CTRL_PCS(0x0C); /* Frequency = IP_bus clock/PCS */

    /* Reset counter register */
    TMR1->CHANNEL[0].CNTR = 0x00;

    /* Run counter */
    TMR1->CHANNEL[0].CTRL |= TMR_CTRL_CM(0x01);

    /* Enable & setup interrupt from QTMR1 */
    EnableIRQ(TMR1_IRQn);
    NVIC_SetPriority(TMR1_IRQn, 2U);

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
    /* PWM base pointer (affects the entire initialization) */
    PWM_Type *PWMBase = (PWM_Type *)PWM2;

    /* Full and Half cycle reload */
    PWMBase->SM[0].CTRL |= PWM_CTRL_FULL_MASK | PWM_CTRL_HALF_MASK;
    PWMBase->SM[1].CTRL |= PWM_CTRL_FULL_MASK | PWM_CTRL_HALF_MASK;
    PWMBase->SM[2].CTRL |= PWM_CTRL_FULL_MASK | PWM_CTRL_HALF_MASK;
    
    /* Value register initial values, duty cycle 50% */
    PWMBase->SM[0].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));
    PWMBase->SM[1].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));
    PWMBase->SM[2].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));

    PWMBase->SM[0].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));
    PWMBase->SM[1].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));
    PWMBase->SM[2].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));

    PWMBase->SM[0].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
    PWMBase->SM[1].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
    PWMBase->SM[2].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));

    PWMBase->SM[0].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));
    PWMBase->SM[1].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));
    PWMBase->SM[2].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));

    /* Trigger for SINC filter measurement (PWM2_SM0_VAL0) ... sampling window centered with PWM2 full cycle reload */      
    PWMBase->SM[0].VAL0 = PWM_VAL0_VAL0((uint16_t)(((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1)) - (SINC1_PRETRIGGER_VAL*SINC1_PWM_CLK_RATION)));
    PWMBase->SM[1].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    PWMBase->SM[2].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    
    /* Trigger for SINC filter measurement (PWM2_SM0_VAL4) ... sampling window centered with PWM2 half cycle reload */ 
    PWMBase->SM[0].VAL4 = PWM_VAL4_VAL4((uint16_t)(0U - (SINC1_PRETRIGGER_VAL*SINC1_PWM_CLK_RATION)));
    PWMBase->SM[1].VAL4 = PWM_VAL4_VAL4((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));
    PWMBase->SM[2].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));

    /* Trigger for sync PWM1 on VAL5 */
    PWMBase->SM[0].VAL5 = PWM_VAL5_VAL5((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
    PWMBase->SM[1].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMBase->SM[2].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));

    /* PWM sub-module 0 trigger0 on VAL0 and VAL4 enabled for trigger SINC. SINC is in continuous mode. */
    PWMBase->SM[0].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 0) | PWM_TCTRL_OUT_TRIG_EN(1 << 4);
    
    /* PWM2 submodule 0 trigger1 on VAL5 enabled for PWM1 synchronization */
    PWMBase->SM[0].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 5);
    
    /* PWM sub-module 1 trigger0 on VAL0 and VAL4 enabled for trigger BiSS. */
    PWMBase->SM[1].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 0) | PWM_TCTRL_OUT_TRIG_EN(1 << 4);
    
    /* Set dead-time register */
    PWMBase->SM[0].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[1].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[2].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[0].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[1].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);
    PWMBase->SM[2].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M1PwmDeadTime);

    /* Channels A and B disabled when faults 0 and 1 occur */
    PWMBase->SM[0].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x3));
    PWMBase->SM[1].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x3));
    PWMBase->SM[2].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x3));
    PWMBase->SM[0].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x3));
    PWMBase->SM[1].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x3));
    PWMBase->SM[2].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x3));

    /* Modules one and two gets clock from module zero */
    PWMBase->SM[1].CTRL2 = (PWMBase->SM[1].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);
    PWMBase->SM[2].CTRL2 = (PWMBase->SM[2].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);

    /* Master reload active for modules one and two */
    PWMBase->SM[1].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;
    PWMBase->SM[2].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;

    /* Master sync active for modules one and two*/
    PWMBase->SM[1].CTRL2 = (PWMBase->SM[1].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);
    PWMBase->SM[2].CTRL2 = (PWMBase->SM[2].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);

    /* Fault 0 active in logic level 0, fault 1  active in level 1, automatic clearing */
    PWMBase->FCTRL = (PWMBase->FCTRL & ~PWM_FCTRL_FLVL_MASK) | PWM_FCTRL_FLVL(0x0) | PWM_FCTRL_FLVL(0x2);
    PWMBase->FCTRL = (PWMBase->FCTRL & ~PWM_FCTRL_FAUTO_MASK) | PWM_FCTRL_FAUTO(0x1) | PWM_FCTRL_FAUTO(0x2);

    /* Clear fault flags */
    PWMBase->FSTS = (PWMBase->FSTS & ~PWM_FSTS_FFLAG_MASK) | PWM_FSTS_FFLAG(0xF);

    /* PWMs are re-enabled at PWM full cycle */
    PWMBase->FSTS = (PWMBase->FSTS & ~PWM_FSTS_FFULL_MASK) | PWM_FSTS_FFULL(0x1) | PWM_FSTS_FFULL(0x2);

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
    g_sM1Pwm3ph.ui16Fault2FixNum = M1_FAULT_OV_NUM; /* PWMA fixed-value over-voltage fault number */
    
    g_sM1Pwm3ph.ui16Modulo = PWMBase->SM[0].VAL1;
}



/*!
 * @brief   void M2_InitPWM(void)
 *           - Initialization of the eFlexPWMA peripheral for motor M2
 *           - 3-phase center-aligned PWM
 *
 * @param   void
 *
 * @return  none
 */
void M2_InitPWM(void)
{
    /* PWM base pointer (affects the entire initialization) */
    PWM_Type *PWMBase = (PWM_Type *)PWM1;

    /* Full and Half cycle reload */
    PWMBase->SM[0].CTRL |= PWM_CTRL_FULL_MASK | PWM_CTRL_HALF_MASK;
    PWMBase->SM[1].CTRL |= PWM_CTRL_FULL_MASK | PWM_CTRL_HALF_MASK;
    PWMBase->SM[2].CTRL |= PWM_CTRL_FULL_MASK | PWM_CTRL_HALF_MASK;
    
    /* Value register initial values, duty cycle 50% */
    PWMBase->SM[0].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));
    PWMBase->SM[1].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));
    PWMBase->SM[2].INIT = PWM_INIT_INIT((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));

    PWMBase->SM[0].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));
    PWMBase->SM[1].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));
    PWMBase->SM[2].VAL1 = PWM_VAL1_VAL1((uint16_t)((g_sClockSetup.ui16M1PwmModulo / 2) - 1));

    PWMBase->SM[0].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
    PWMBase->SM[1].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));
    PWMBase->SM[2].VAL2 = PWM_VAL2_VAL2((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 4)));

    PWMBase->SM[0].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));
    PWMBase->SM[1].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));
    PWMBase->SM[2].VAL3 = PWM_VAL3_VAL3((uint16_t)(g_sClockSetup.ui16M1PwmModulo / 4));

    /* Trigger for SINC filter measurement (PWM1_SM0_VAL0) ... sampling window centered with PWM1 full cycle reload */      
    PWMBase->SM[0].VAL0 = PWM_VAL0_VAL0((uint16_t)(((uint16_t)((g_sClockSetup.ui16M2PwmModulo / 2) - 1)) - (SINC2_PRETRIGGER_VAL*SINC2_PWM_CLK_RATION)));
    PWMBase->SM[1].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    PWMBase->SM[2].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));
    
    /* Trigger for SINC filter measurement (PWM1_SM0_VAL4) ... sampling window centered with PWM1 half cycle reload */ 
    PWMBase->SM[0].VAL4 = PWM_VAL4_VAL4((uint16_t)(0U - (SINC2_PRETRIGGER_VAL*SINC2_PWM_CLK_RATION)));
    PWMBase->SM[1].VAL4 = PWM_VAL4_VAL4((uint16_t)(-(g_sClockSetup.ui16M1PwmModulo / 2)));
    PWMBase->SM[2].VAL4 = PWM_VAL4_VAL4((uint16_t)(0));

    PWMBase->SM[0].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMBase->SM[1].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));
    PWMBase->SM[2].VAL5 = PWM_VAL5_VAL5((uint16_t)(0));

    /* PWM sub-module 0 trigger0 on VAL0 and VAL4 enabled for trigger SINC. SINC is in continuous mode. */
    PWMBase->SM[0].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 0) | PWM_TCTRL_OUT_TRIG_EN(1 << 4);
    
    /* PWM sub-module 1 trigger0 on VAL0 and VAL4 enabled for trigger EnDat2.2. */
    PWMBase->SM[1].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 0) | PWM_TCTRL_OUT_TRIG_EN(1 << 4);
    
    /* Set dead-time register */
    PWMBase->SM[0].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M2PwmDeadTime);
    PWMBase->SM[1].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M2PwmDeadTime);
    PWMBase->SM[2].DTCNT0 = PWM_DTCNT0_DTCNT0(g_sClockSetup.ui16M2PwmDeadTime);
    PWMBase->SM[0].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M2PwmDeadTime);
    PWMBase->SM[1].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M2PwmDeadTime);
    PWMBase->SM[2].DTCNT1 = PWM_DTCNT1_DTCNT1(g_sClockSetup.ui16M2PwmDeadTime);

    /* Channels A and B disabled when faults 0 and 1 occur */
    PWMBase->SM[0].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x3));
    PWMBase->SM[1].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x3));
    PWMBase->SM[2].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0A_MASK) | PWM_DISMAP_DIS0A(0x3));
    PWMBase->SM[0].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x3));
    PWMBase->SM[1].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x3));
    PWMBase->SM[2].DISMAP[0] = ((PWMBase->SM[0].DISMAP[0] & ~PWM_DISMAP_DIS0B_MASK) | PWM_DISMAP_DIS0B(0x3));

    /* Modules one and two gets clock from module zero */
    PWMBase->SM[1].CTRL2 = (PWMBase->SM[1].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);
    PWMBase->SM[2].CTRL2 = (PWMBase->SM[2].CTRL2 & ~PWM_CTRL2_CLK_SEL_MASK) | PWM_CTRL2_CLK_SEL(0x2);

    /* External synchronization for submodule 0 from PWM2 */
    PWMBase->SM[0].CTRL2 = (PWMBase->SM[0].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x3);
    
    /* Master reload active for modules one and two */
    PWMBase->SM[1].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;
    PWMBase->SM[2].CTRL2 |= PWM_CTRL2_RELOAD_SEL_MASK;

    /* Master sync active for modules one and two*/
    PWMBase->SM[1].CTRL2 = (PWMBase->SM[1].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);
    PWMBase->SM[2].CTRL2 = (PWMBase->SM[2].CTRL2 & ~PWM_CTRL2_INIT_SEL_MASK) | PWM_CTRL2_INIT_SEL(0x2);

    /* Fault 0 active in logic level 0, fault 1  active in level 1, automatic clearing */
    PWMBase->FCTRL = (PWMBase->FCTRL & ~PWM_FCTRL_FLVL_MASK) | PWM_FCTRL_FLVL(0x0) | PWM_FCTRL_FLVL(0x2);
    PWMBase->FCTRL = (PWMBase->FCTRL & ~PWM_FCTRL_FAUTO_MASK) | PWM_FCTRL_FAUTO(0x1) | PWM_FCTRL_FAUTO(0x2);

    /* Clear fault flags */
    PWMBase->FSTS = (PWMBase->FSTS & ~PWM_FSTS_FFLAG_MASK) | PWM_FSTS_FFLAG(0xF);

    /* PWMs are re-enabled at PWM full cycle */
    PWMBase->FSTS = (PWMBase->FSTS & ~PWM_FSTS_FFULL_MASK) | PWM_FSTS_FFULL(0x1) | PWM_FSTS_FFULL(0x2);

    /* PWM fault filter - 5 Fast peripheral clocks sample rate, 5 agreeing
       samples to activate */
    PWMBase->FFILT = (PWMBase->FFILT & ~PWM_FFILT_FILT_PER_MASK) | PWM_FFILT_FILT_PER(5);
    PWMBase->FFILT = (PWMBase->FFILT & ~PWM_FFILT_FILT_CNT_MASK) | PWM_FFILT_FILT_CNT(5);
       
    /* Start PWMs (set load OK flags and run) */
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_CLDOK_MASK) | PWM_MCTRL_CLDOK(0xF);
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_LDOK_MASK) | PWM_MCTRL_LDOK(0xF);
    PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_RUN_MASK) | PWM_MCTRL_RUN(0x0);
    
    /* Initialize MC driver */
    g_sM2Pwm3ph.pui32PwmBaseAddress = (PWM_Type *)PWMBase;

    g_sM2Pwm3ph.ui16PhASubNum = 0U; /* PWMA phase A sub-module number */
    g_sM2Pwm3ph.ui16PhBSubNum = 1U; /* PWMA phase B sub-module number */
    g_sM2Pwm3ph.ui16PhCSubNum = 2U; /* PWMA phase C sub-module number */

    g_sM2Pwm3ph.ui16FaultFixNum = M2_FAULT_NUM; /* PWMA fixed-value over-current fault number */
    g_sM2Pwm3ph.ui16FaultAdjNum = M2_FAULT_NUM; /* PWMA adjustable over-current fault number */
    g_sM2Pwm3ph.ui16Fault2FixNum = M2_FAULT_OV_NUM; /* PWMA fixed-value over-voltage fault number */
    
    g_sM2Pwm3ph.ui16Modulo = PWMBase->SM[0].VAL1;
}

void Sinc1_Init(void)
{
    uint32_t u32ChannelId;    
    SINC_Type *SINCBase = (SINC_Type *)SINC1;

    /* Configure MCLKOUT0 to hal_clock_buswakeup / (7+1) = 133333333/8 */
    SINCBase->MCR = SINC_MCR_PRESCALE(0U) |
    		    SINC_MCR_MCLKDIV(7U);

    /* Disable MCLKOUT 1 & 2 */
    SINCBase->MCR |= SINC_MCR_MCLK1DIS(1U) | SINC_MCR_MCLK2DIS(1U); 

    /* Continuous conversion mode, set filter order, set oversampling ratio */
    SINCBase->CHANNEL[0].CDR =  SINC_CDR_PFCM(1U) | SINC_CDR_PFORD(SINC1_ORD) |	SINC_CDR_PFOSR(SINC1_OSR);
    SINCBase->CHANNEL[1].CDR =  SINC_CDR_PFCM(1U) | SINC_CDR_PFORD(SINC1_ORD) |	SINC_CDR_PFOSR(SINC1_OSR);
    SINCBase->CHANNEL[2].CDR =  SINC_CDR_PFCM(1U) | SINC_CDR_PFORD(SINC1_ORD) |	SINC_CDR_PFOSR(SINC1_OSR);
    SINCBase->CHANNEL[3].CDR =  SINC_CDR_PFCM(1U) | SINC_CDR_PFORD(SINC1_ORD) |	SINC_CDR_PFOSR(SINC1_OSR);

    /* Edge triggering, external bitstream from the MBIT[n], sample on clock negative edge, 
        set FIFO watermark, left justified, signed, left shift of 4 bits */     
    SINCBase->CHANNEL[0].CCFR = SINC_CCFR_ITLVL(0U) | SINC_CCFR_IBSEL(0U) | SINC_CCFR_IBFMT(0U) | SINC_CCFR_ICESEL(2U) |
                                SINC_CCFR_FIFOWMK(1U) | SINC_CCFR_RDFMT(0U) | SINC_CCFR_PFSFT(0x14U) |
                                SINC_CCFR_ICSEL(0U) | SINC_CCFR_ITSEL(1U);              /* MCLK_OUT0 clock output with internal loopback, HW trigger conversion */
    SINCBase->CHANNEL[1].CCFR = SINC_CCFR_ITLVL(0U) | SINC_CCFR_IBSEL(0U) | SINC_CCFR_IBFMT(0U) | SINC_CCFR_ICESEL(2U) |
                                SINC_CCFR_FIFOWMK(1U) | SINC_CCFR_RDFMT(0U) | SINC_CCFR_PFSFT(0x14U) |
                                SINC_CCFR_ICSEL(7U) | SINC_CCFR_ITSEL(3U);              /* Tie the clock internally to adjacent channel, grouped trigger shared with adjacent channel */
    SINCBase->CHANNEL[2].CCFR = SINC_CCFR_ITLVL(0U) | SINC_CCFR_IBSEL(0U) | SINC_CCFR_IBFMT(0U) | SINC_CCFR_ICESEL(2U) |
                                SINC_CCFR_FIFOWMK(1U) | SINC_CCFR_RDFMT(0U) | SINC_CCFR_PFSFT(0x14U) |
                                SINC_CCFR_ICSEL(7U) | SINC_CCFR_ITSEL(3U);
    SINCBase->CHANNEL[3].CCFR = SINC_CCFR_ITLVL(0U) | SINC_CCFR_IBSEL(0U) | SINC_CCFR_IBFMT(0U) | SINC_CCFR_ICESEL(2U) |
                                SINC_CCFR_FIFOWMK(1U) | SINC_CCFR_RDFMT(1U) | SINC_CCFR_PFSFT(0x14U) |          /* channel 3 set unsigned (DC bus voltage) */
                                SINC_CCFR_ICSEL(7U) | SINC_CCFR_ITSEL(3U);
    
    /* Primary filter enable, channel enable, FIFO enable */ 
    SINCBase->CHANNEL[0].CCR = SINC_CCR_PFEN(1U) | SINC_CCR_CHEN(1U) | SINC_CCR_FIFOEN(1U);		
    SINCBase->CHANNEL[1].CCR = SINC_CCR_PFEN(1U) | SINC_CCR_CHEN(1U) | SINC_CCR_FIFOEN(1U);
    SINCBase->CHANNEL[2].CCR = SINC_CCR_PFEN(1U) | SINC_CCR_CHEN(1U) | SINC_CCR_FIFOEN(1U);
    SINCBase->CHANNEL[3].CCR = SINC_CCR_PFEN(1U) | SINC_CCR_CHEN(1U) | SINC_CCR_FIFOEN(1U);

    /* Enable Data Output Ready interrupt CHFIE0 */
    SINCBase->NIE |= (1U << SINC_NIE_CHFIE0_SHIFT);
    
    /* Enable SINC1 interrupt */
    NVIC_SetPriority(SINC1_CH0_IRQn, SINC1_IRQ_PRIORITY);
    NVIC_EnableIRQ(SINC1_CH0_IRQn);
    
    /* Set base pointer used in the SINC MC driver */
    g_sM1Curr3phDcBus.pui32SincBaseAddress = (SINC_Type *)SINCBase;

    /* Master enable */
    SINCBase->MCR |= SINC_MCR_MEN(1U);

    /* Wait for the channel ready flags */
    for( u32ChannelId = 0U; u32ChannelId  < SINC1_NUM_CHANNELS_USED; ++u32ChannelId )
    {
    	while( 0U == (SINCBase->SR & (1U << (u32ChannelId + SINC_SR_CHRDY0_SHIFT)) ) )
    	{}
    }
}

void Sinc2_Init(void)
{
    uint32_t u32ChannelId;    
    SINC_Type *SINCBase = (SINC_Type *)SINC2;

    /* Configure MCLKOUT0 to hal_clock_buswakeup / (7+1) = 133333333/8 */
    SINCBase->MCR = SINC_MCR_PRESCALE(0U) |
                    SINC_MCR_MCLKDIV(7U);

    /* Disable MCLKOUT 2 */
    SINCBase->MCR |= SINC_MCR_MCLK2DIS(1U); 

    /* Continuous conversion mode, set filter order, set oversampling ratio */
    SINCBase->CHANNEL[0].CDR =  SINC_CDR_PFCM(1U) | SINC_CDR_PFORD(SINC2_ORD) |	SINC_CDR_PFOSR(SINC2_OSR);
    SINCBase->CHANNEL[1].CDR =  SINC_CDR_PFCM(1U) | SINC_CDR_PFORD(SINC2_ORD) |	SINC_CDR_PFOSR(SINC2_OSR);
    SINCBase->CHANNEL[2].CDR =  SINC_CDR_PFCM(1U) | SINC_CDR_PFORD(SINC2_ORD) |	SINC_CDR_PFOSR(SINC2_OSR);
    SINCBase->CHANNEL[3].CDR =  SINC_CDR_PFCM(1U) | SINC_CDR_PFORD(SINC2_ORD) |	SINC_CDR_PFOSR(SINC2_OSR);

    /* Edge triggering, external bitstream from the MBIT[n], sample on clock negative edge, 
        set FIFO watermark, left justified, signed, left shift of 4 bits */     
    SINCBase->CHANNEL[0].CCFR = SINC_CCFR_ITLVL(0U) | SINC_CCFR_IBSEL(0U) | SINC_CCFR_IBFMT(0U) | SINC_CCFR_ICESEL(2U) |
                                SINC_CCFR_FIFOWMK(1U) | SINC_CCFR_RDFMT(0U) | SINC_CCFR_PFSFT(0x14U) |
                                SINC_CCFR_ICSEL(0U) | SINC_CCFR_ITSEL(1U);              /* MCLK_OUT0 clock output with internal loopback, HW trigger conversion */
    SINCBase->CHANNEL[1].CCFR = SINC_CCFR_ITLVL(0U) | SINC_CCFR_IBSEL(0U) | SINC_CCFR_IBFMT(0U) | SINC_CCFR_ICESEL(2U) |
                                SINC_CCFR_FIFOWMK(1U) | SINC_CCFR_RDFMT(0U) | SINC_CCFR_PFSFT(0x14U) |
                                SINC_CCFR_ICSEL(7U) | SINC_CCFR_ITSEL(3U);              /* Tie the clock internally to adjacent channel, grouped trigger shared with adjacent channel */
    SINCBase->CHANNEL[2].CCFR = SINC_CCFR_ITLVL(0U) | SINC_CCFR_IBSEL(0U) | SINC_CCFR_IBFMT(0U) | SINC_CCFR_ICESEL(2U) |
                                SINC_CCFR_FIFOWMK(1U) | SINC_CCFR_RDFMT(0U) | SINC_CCFR_PFSFT(0x14U) |
                                SINC_CCFR_ICSEL(7U) | SINC_CCFR_ITSEL(3U);
    SINCBase->CHANNEL[3].CCFR = SINC_CCFR_ITLVL(0U) | SINC_CCFR_IBSEL(0U) | SINC_CCFR_IBFMT(0U) | SINC_CCFR_ICESEL(2U) |
                                SINC_CCFR_FIFOWMK(1U) | SINC_CCFR_RDFMT(1U) | SINC_CCFR_PFSFT(0x14U) |          /* channel 3 set unsigned (DC bus voltage) */
                                SINC_CCFR_ICSEL(7U) | SINC_CCFR_ITSEL(3U);

    /* Primary filter enable, channel enable, FIFO enable */ 
    SINCBase->CHANNEL[0].CCR = SINC_CCR_PFEN(1U) | SINC_CCR_CHEN(1U) | SINC_CCR_FIFOEN(1U);		
    SINCBase->CHANNEL[1].CCR = SINC_CCR_PFEN(1U) | SINC_CCR_CHEN(1U) | SINC_CCR_FIFOEN(1U);
    SINCBase->CHANNEL[2].CCR = SINC_CCR_PFEN(1U) | SINC_CCR_CHEN(1U) | SINC_CCR_FIFOEN(1U);
    SINCBase->CHANNEL[3].CCR = SINC_CCR_PFEN(1U) | SINC_CCR_CHEN(1U) | SINC_CCR_FIFOEN(1U);

    /* Enable Data Output Ready interrupt CHFIE0 */
    SINCBase->NIE |= (1U << SINC_NIE_CHFIE0_SHIFT);

    /* Enable SINC2 interrupt */
    NVIC_SetPriority(SINC2_CH0_IRQn, SINC2_IRQ_PRIORITY);
    NVIC_EnableIRQ(SINC2_CH0_IRQn);

    /* Set base pointer used in the SINC MC driver */
    g_sM2Curr3phDcBus.pui32SincBaseAddress = (SINC_Type *)SINCBase;

    /* Master enable */
    SINCBase->MCR |= SINC_MCR_MEN(1U);

    /* Wait for the channel ready flags */
    for( u32ChannelId = 0U; u32ChannelId  < SINC2_NUM_CHANNELS_USED; ++u32ChannelId )
    {
      while( 0U == (SINCBase->SR & (1U << (u32ChannelId + SINC_SR_CHRDY0_SHIFT)) ) )
      {}
    }
}
