/*
 * Copyright 2025-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mc_periph_init.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/
#define MC_SM_CTRL_PDM_CLK_SEL          0U   /*!< AON PDM clock sel */
#define MC_SM_CTRL_MQS1_SETTINGS        1U   /*!< AON MQS settings */
#define MC_SM_CTRL_MQS2_SETTINGS        2U   /*!< WAKE MQS settings */
#define MC_SM_CTRL_SAI1_MCLK            3U   /*!< AON SAI1 MCLK */
#define MC_SM_CTRL_SAI2_MCLK            4U   /*!< WAKE SAI2 MCLK */
#define MC_SM_CTRL_SAI3_MCLK            5U   /*!< WAKE SAI3 MCLK */
#define MC_SM_CTRL_SAI4_MCLK            6U   /*!< WAKE SAI4 MCLK */
#define MC_SM_CTRL_ADC_TEST             7U   /*!< BBSM SNVS ADC enable */
#define MC_SM_CTRL_GPT_MUX              8U   /*!< GPT mux */
#define MC_SM_CTRL_XBAR_DIR_CTRL        9U   /*!< XBAR IO direction */
#define MC_SM_CTRL_XBAR_TRIG_SYNC       10U  /*!< XBAR trigger sync ctrl1 */
#define MC_SM_CTRL_ADC_TRIGGER          11U  /*!< ADC trigger */
#define MC_SM_CTRL_HPF1_SYNC_SRC_CFG1   12U  /*!< Hiperface#1 sync src cfg#1 */
#define MC_SM_CTRL_HPF1_SYNC_SRC_CFG2   13U  /*!< Hiperface#1 sync src cfg#2 */
#define MC_SM_CTRL_HPF2_SYNC_SRC_CFG1   14U  /*!< Hiperface#2 sync src cfg#1 */
#define MC_SM_CTRL_HPF2_SYNC_SRC_CFG2   15U  /*!< Hiperface#2 sync src cfg#2 */
#define MC_SM_CTRL_HPF1_INTR_CTRL       16U  /*!< Hiperface#1 interrupt ctrl */
#define MC_SM_CTRL_HPF2_INTR_CTRL       17U  /*!< Hiperface#2 interrupt ctrl */
#define MC_SM_CTRL_ENDAT3_STATUS        18U  /*!< EnDat 3.0 status register */
#define MC_SM_CTRL_ENC_DIAG_MUX_SEL     19U  /*!< Diagnostic bus mux sel reg */
#define MC_SM_CTRL_HPF_SYNC_OUT_CTL     20U  /*!< Hiperface ext sync out ctrl */
#define MC_SM_CTRL_ENDAT_STRETCH_CTRL   21U  /*!< ENDAT_STRETCHER_CTRL */
#define MC_SM_CTRL_BISS1_PULSE_STR_CTL  22U  /*!< BISS#1 pulse stretch ctrl */
#define MC_SM_CTRL_XBAR_TRIG_SYNC_2     23U  /*!< XBAR trigger sync ctrl#2  */
#define MC_SM_CTRL_XBAR_TRIG_SYNC_3     24U  /*!< XBAR trigger sync ctrl#3  */
#define MC_SM_CTRL_XBAR_TRIG_SYNC_4     25U  /*!< XBAR trigger sync ctrl#4  */
#define MC_SM_CTRL_XBAR_DIR_CTRL_2      26U   /*!< XBAR IO direction ctrl#2 */
/*******************************************************************************
 * Variables
 ******************************************************************************/


/* Structure for 3-phase PWM MC driver */
mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;
mcdrv_pwm3ph_pwma_t g_sM2Pwm3ph;

/* Structure for EnDat2.2, EnDat3 driver */
#if (M1_ENCODER == ENCODER_ENDAT3)
mcdrv_endat3_t g_sM1Enc;
#elif (M1_ENCODER == ENCODER_ENDAT2P2_2)
mcdrv_endat2p2_t g_sM1Enc;
#elif (M1_ENCODER == ENCODER_BISS)
BISSC_Type g_sM1Enc = { .ui8DevDataLen = BISS_DEVICE_DATA_LEN, \
                          .ui8DevSTLen = BISS_DEVICE_ST_LEN, \
                          .ui8DevMTLen = BISS_DEVICE_MT_LEN};
#endif

#if (M2_ENCODER == ENCODER_ENDAT3)
mcdrv_endat3_t g_sM2Enc;
#elif (M2_ENCODER == ENCODER_ENDAT2P2_1)
mcdrv_endat2p2_t g_sM2Enc;
#elif (M2_ENCODER == ENCODER_BISS)
BISSC_Type g_sM2Enc = { .ui8DevDataLen = BISS_DEVICE_DATA_LEN, \
                          .ui8DevSTLen = BISS_DEVICE_ST_LEN, \
                          .ui8DevMTLen = BISS_DEVICE_MT_LEN};
#endif

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
    /* 6-channel PWM peripheral init for M1 */
    M1_MCDRV_PWM_PERIPH_INIT();
    /* 6-channel PWM peripheral init for M2 */
    M2_MCDRV_PWM_PERIPH_INIT();
}

/* 64KHz */
#define M1_POSPE_TO_KP_GAIN (1256.64F)
#define M1_POSPE_TO_KI_GAIN (6.1685F)
#define M1_POSPE_TO_THETA_GAIN (0.0000024868) // DiscMethodFactor is 2!

#if (M1_ENCODER == ENCODER_ENDAT3) || (M2_ENCODER == ENCODER_ENDAT3)
/*!
 * @brief   void InitEndat3(void)
 *           - Initialization of the EnDat3 peripheral
 *
 * @param   void
 *
 * @return  none
 */
void InitEndat3(void)
{
  status_t retVal;
  int32_t SCMI_status = 0;
  uint32_t blk_ctrl_size = 0;
  uint32_t blk_ctrl_value = 0;
  __attribute__((unused)) int32_t result;

  /* EnDat3.0 200MHz */
  clk_t endat3Clk_rxtx = {
    .clkId = kCLOCK_Endat31fast,
    .pclkId = kCLOCK_Syspll1dfs1div2, /* 400 MHz */
    .rate = 200000000UL,
    .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
  };

  clk_t endat3Clk_sys = {
    .clkId = kCLOCK_Endat31slow,
    .pclkId = kCLOCK_Syspll1dfs1div2, /* 400 MHz */
    .rate = 100000000UL,
    .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
  };

  CLOCK_SetParent(&endat3Clk_rxtx);
  CLOCK_SetRate(&endat3Clk_rxtx);
  CLOCK_EnableClock(endat3Clk_rxtx.clkId);

  CLOCK_SetParent(&endat3Clk_sys);
  CLOCK_SetRate(&endat3Clk_sys);
  CLOCK_EnableClock(endat3Clk_sys.clkId);

  /* DIAG_ENCODER_MUX_SEL will be updated */
  SCMI_status = SCMI_MiscControlGet(SCMI_A2P, MC_SM_CTRL_ENC_DIAG_MUX_SEL, &blk_ctrl_size, &blk_ctrl_value);

  if (SCMI_status == SCMI_ERR_SUCCESS)
  {
    /* endat3 mux select configuration successful */
    result = SCMI_ERR_SUCCESS;
  }
  else if(SCMI_status == SCMI_ERR_NOT_FOUND)
  {
    result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
  }
  else if(SCMI_status == SCMI_ERR_DENIED)
  {
    result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
  }

#if (M1_ENCODER == ENCODER_ENDAT3)
  /* Select endat3 for encoder2 */
  blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_DIAG_ENCODER_MUX_SEL_diag_enc2_sel(DIG_ENCODER_MUX_ENDAT3);
  XBAR_SetSignalsConnection(kXBAR1_InputFlexpwm2Mux0Trigger1, kXBAR1_OutputEndat3HwStrobe);
#elif (M2_ENCODER == ENCODER_ENDAT3)
  blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_DIAG_ENCODER_MUX_SEL_diag_enc1_sel(DIG_ENCODER_MUX_ENDAT3);
  XBAR_SetSignalsConnection(kXBAR1_InputFlexpwm1Mux0Trigger1, kXBAR1_OutputEndat3HwStrobe);
#endif

  /* Update DIAG_ENCODER_MUX_SEL */
  SCMI_status = SCMI_MiscControlSet(SCMI_A2P, MC_SM_CTRL_ENC_DIAG_MUX_SEL, blk_ctrl_size, &blk_ctrl_value);

  if (SCMI_status == SCMI_ERR_SUCCESS)
  {
    /* endat3 for encoder2 configuration successful */
    result = SCMI_ERR_SUCCESS;
  }
  else if(SCMI_status == SCMI_ERR_NOT_FOUND)
  {
    result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
  }
  else if(SCMI_status == SCMI_ERR_DENIED)
  {
    result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
  }

  /* ENDAT_STRETCHER_CTRL will be updated */
  SCMI_status = SCMI_MiscControlGet(SCMI_A2P, MC_SM_CTRL_ENDAT_STRETCH_CTRL, &blk_ctrl_size, &blk_ctrl_value);

  if (SCMI_status == SCMI_ERR_SUCCESS)
  {
    result = SCMI_ERR_SUCCESS;
  }
  else if(SCMI_status == SCMI_ERR_NOT_FOUND)
  {
    result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
  }
  else if(SCMI_status == SCMI_ERR_DENIED)
  {
    result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
  }

  /* Set values to be updated in the ENDAT_STRETCHER_CTRL register. */
  blk_ctrl_value &= ~(BLK_CTRL_WAKEUPMIX_ENDAT_STRETCHER_CTRL_endat3p0_hw_strobe_value_MASK << BLK_CTRL_WAKEUPMIX_ENDAT_STRETCHER_CTRL_endat3p0_hw_strobe_value_SHIFT);
  blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_ENDAT_STRETCHER_CTRL_endat3p0_hw_strobe_value(ENDAT3_STRETCHER_CTRL_HW_STROBE_COUNTER);
  blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_ENDAT_STRETCHER_CTRL_endat3p0_hw_strobe_ctrl(1);
  blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_ENDAT_STRETCHER_CTRL_endat3p0_async_en(1);
  blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_ENDAT_STRETCHER_CTRL_endat3p0_pol_sel(1);

  /* Update ENDAT_STRETCHER_CTRL register */
  SCMI_status = SCMI_MiscControlSet(SCMI_A2P, MC_SM_CTRL_ENDAT_STRETCH_CTRL, blk_ctrl_size, &blk_ctrl_value);

  if (SCMI_status == SCMI_ERR_SUCCESS)
  {
    /* ENDAT_STRETCHER_CTRL updated */
    result = SCMI_ERR_SUCCESS;
  }
  else if(SCMI_status == SCMI_ERR_NOT_FOUND)
  {
    result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
  }
  else if(SCMI_status == SCMI_ERR_DENIED)
  {
    result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
  }

  /* Set master clock to 12.5Mbps - the default data transfer rate of ECN1325 EnDat3 encoder */
  ENDAT3_RxTxClkConfig(ENDAT3, ENDAT3_SOURCE_CLOCK, ENDAT3_RXTX_RATE_12_5MBPS, 0);
  /*  Check the communication is working */
  if (ENDAT3_FG_Hello(ENDAT3) == kStatus_Success)
  {
    /* Encoder data transfer rate is 12.5Mbps */
    /* Switch the encoder data transfer rate to 25Mbps */
    retVal = ENDAT3_FG_Rate(ENDAT3, ENDAT3_RXTX_RATE_25MBPS);
    if (retVal != kStatus_Success)
    {
      /* Failed to switch to 25Mbps */
      return;
    }

    SDK_DelayAtLeastUs(20U, SystemCoreClock);
    if (ENDAT3_FG_Hello(ENDAT3) != kStatus_Success)
    {
      /* Failed to switch to 25Mbps */
      return;
    }
  }
  else
  {
    /* Actual encoder data transfer rate is 25Mbps, change master clock to the same clock (25Mbps) */
    ENDAT3_RxTxClkConfig(ENDAT3, ENDAT3_SOURCE_CLOCK, ENDAT3_RXTX_RATE_25MBPS,  0);

    SDK_DelayAtLeastUs(20U, SystemCoreClock);
    if (ENDAT3_FG_Hello(ENDAT3) != kStatus_Success)
    {
      /* Communication was not established. */
      return;
    }
  }

  /* Enable the FG_IRQ0 when HPF received */
  ENDAT3_FG_IRQ_Enable_With_FIxM_Frame_Count(ENDAT3, 0, 1);

  /* Enable EnDat3 HW trigger */
  ENDAT3_HW_Strobe_Enable(ENDAT3);

  /* Enable interrupt for EnDat3 event */
  NVIC_EnableIRQ(ENDAT3_FG_IRQn);
  NVIC_SetPriority(ENDAT3_FG_IRQn, 0U);
}
#endif  /* EnDat3 encoder is used. */

#if (M2_ENCODER == ENCODER_ENDAT2P2_1)
/*!
 * @brief      Init EnDat2.2_1 master IP for M2
 *
 * @param      void
 *
 * @return     endat2p2_dev_t
 */
void InitEndat2p2_1(void)
{
    int data;
    int32_t SCMI_status = 0;
    uint32_t blk_ctrl_size = 0;
    uint32_t blk_ctrl_value = 0;
    __attribute__((unused)) int32_t result;

    endat2p2_dev_t *dev;
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

    /* DIAG_ENCODER_MUX_SEL will be updated */
    SCMI_status = SCMI_MiscControlGet(SCMI_A2P, MC_SM_CTRL_ENC_DIAG_MUX_SEL, &blk_ctrl_size, &blk_ctrl_value);

    if (SCMI_status == SCMI_ERR_SUCCESS)
    {
      /* endat2p2 mux select configuration successful */
      result = SCMI_ERR_SUCCESS;
    }
    else if(SCMI_status == SCMI_ERR_NOT_FOUND)
    {
      result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
    }
    else if(SCMI_status == SCMI_ERR_DENIED)
    {
      result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
    }

    /* Select endat2p2 for encoder2 */
    blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_DIAG_ENCODER_MUX_SEL_diag_enc1_sel(DIG_ENCODER_MUX_ENDAT2P2);

    /* Update DIAG_ENCODER_MUX_SEL */
    SCMI_status = SCMI_MiscControlSet(SCMI_A2P, MC_SM_CTRL_ENC_DIAG_MUX_SEL, blk_ctrl_size, &blk_ctrl_value);

    if (SCMI_status == SCMI_ERR_SUCCESS)
    {
      /* endat2p2 for encoder2 configuration successful */
      result = SCMI_ERR_SUCCESS;
    }
    else if(SCMI_status == SCMI_ERR_NOT_FOUND)
    {
      result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
    }
    else if(SCMI_status == SCMI_ERR_DENIED)
    {
      result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
    }

    /* XBAR_TRIG_SYNC_CTRL2 will be updated */
    SCMI_status = SCMI_MiscControlGet(SCMI_A2P, MC_SM_CTRL_XBAR_TRIG_SYNC_2, &blk_ctrl_size, &blk_ctrl_value);

    if (SCMI_status == SCMI_ERR_SUCCESS)
    {
      /* XBAR_TRIG_SYNC_CTRL2 select successful */
      result = SCMI_ERR_SUCCESS;
    }
    else if(SCMI_status == SCMI_ERR_NOT_FOUND)
    {
      result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
    }
    else if(SCMI_status == SCMI_ERR_DENIED)
    {
      result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
    }

    /* Trigger out synchronizer enable */
    blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_XBAR_TRIG_SYNC_CTRL2_SYNC_ENABLE(2U);

    /* Update XBAR_TRIG_SYNC_CTRL2 */
    SCMI_status = SCMI_MiscControlSet(SCMI_A2P, MC_SM_CTRL_XBAR_TRIG_SYNC_2, blk_ctrl_size, &blk_ctrl_value);

    if (SCMI_status == SCMI_ERR_SUCCESS)
    {
      /* Trigger XBAR synchronizer enabled */
      result = SCMI_ERR_SUCCESS;
    }
    else if(SCMI_status == SCMI_ERR_NOT_FOUND)
    {
      result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
    }
    else if(SCMI_status == SCMI_ERR_DENIED)
    {
      result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
    }

    /* XBAR_TRIG_SYNC_CTRL3 will be updated */
    SCMI_status = SCMI_MiscControlGet(SCMI_A2P, MC_SM_CTRL_XBAR_TRIG_SYNC_3, &blk_ctrl_size, &blk_ctrl_value);

    if (SCMI_status == SCMI_ERR_SUCCESS)
    {
      /* XBAR_TRIG_SYNC_CTRL3 select successful */
      result = SCMI_ERR_SUCCESS;
    }
    else if(SCMI_status == SCMI_ERR_NOT_FOUND)
    {
      result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
    }
    else if(SCMI_status == SCMI_ERR_DENIED)
    {
      result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
    }

    /* XBAR trigger synchronizer control - set pulse width */
    blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_XBAR_TRIG_SYNC_CTRL3_PULSE_WIDTH1(7U);

    /* Update XBAR_TRIG_SYNC_CTRL3 */
    SCMI_status = SCMI_MiscControlSet(SCMI_A2P, MC_SM_CTRL_XBAR_TRIG_SYNC_3, blk_ctrl_size, &blk_ctrl_value);

    if (SCMI_status == SCMI_ERR_SUCCESS)
    {
      /* XBAR trigger synchronizer control - pulse width set successful */
      result = SCMI_ERR_SUCCESS;
    }
    else if(SCMI_status == SCMI_ERR_NOT_FOUND)
    {
      result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
    }
    else if(SCMI_status == SCMI_ERR_DENIED)
    {
      result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
    }

    /* ENDAT_STRETCHER_CTRL will be updated */
    SCMI_status = SCMI_MiscControlGet(SCMI_A2P, MC_SM_CTRL_ENDAT_STRETCH_CTRL, &blk_ctrl_size, &blk_ctrl_value);

    if (SCMI_status == SCMI_ERR_SUCCESS)
    {
      /* ENDAT_STRETCHER_CTRL select successful */
      result = SCMI_ERR_SUCCESS;
    }
    else if(SCMI_status == SCMI_ERR_NOT_FOUND)
    {
      result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
    }
    else if(SCMI_status == SCMI_ERR_DENIED)
    {
      result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
    }

    /* defines the number of cycle for the trigger signal coming from xbar */
    blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_ENDAT_STRETCHER_CTRL_endat2p1_nstr_value(3) |
      BLK_CTRL_WAKEUPMIX_ENDAT_STRETCHER_CTRL_endat2p1_nstr_ctrl(1);

    /* Update ENDAT_STRETCHER_CTRL */
    SCMI_status = SCMI_MiscControlSet(SCMI_A2P, MC_SM_CTRL_ENDAT_STRETCH_CTRL, blk_ctrl_size, &blk_ctrl_value);

    if (SCMI_status == SCMI_ERR_SUCCESS)
    {
      /* number of cycle for the trigger signal set */
      result = SCMI_ERR_SUCCESS;
    }
    else if(SCMI_status == SCMI_ERR_NOT_FOUND)
    {
      result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
    }
    else if(SCMI_status == SCMI_ERR_DENIED)
    {
      result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
    }

    XBAR_SetSignalsConnection(kXBAR1_InputFlexpwm1Mux0Trigger1, kXBAR1_OutputTriggerSyncAsyncIn1);

    /* Trigger EnDat2.2 */
    XBAR_SetSignalsConnection(kXBAR1_InputTriggerSyncSyncOut1, kXBAR1_OutputEndat21StrN);
    dev = ENDAT2P2_InitMaster(ENDAT2P2_1, ENDAT2P2_CLK_48M);

    ////////////////////////////////////////////////////////////
    /* Init encoder - ENDAT2P2_InitEncoder(dev); */

    ENDAT2P2_EncoderRest(dev);
    SDK_DelayAtLeastUs((50 * 1000), SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    ENDAT2P2_ClearEncoderError(dev);
    SDK_DelayAtLeastUs((50 * 1000), SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    ENDAT2P2_ClearEncoderWarning(dev);
    SDK_DelayAtLeastUs((50 * 1000), SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    ENDAT2P2_GetEncoderInfo(dev);

    ENDAT2P2_SetDataWordLength(dev, dev->pos_res);

    //////////////////////////////////////////////////////////

    ENDAT2P2_SetFTCLOCK(dev, ENDAT2P2_CLK_8M); // ENDAT2P2_FTCLK

    /* Change tm to 3.75us */
    data = ENDAT2P2_GetParamWithPos(dev, MRS_CODE_OPERATING_STATUS, ENDAT2P2_MEM_WORD_3);
    data = (data & (~3)) | 0x1;

    ENDAT2P2_SetParamWithPos(dev, MRS_CODE_OPERATING_STATUS, ENDAT2P2_MEM_WORD_3, data);
    ENDAT2P2_EncoderRestWithPos(dev);

    data = ENDAT2P2_GetParamWithPos(dev, MRS_CODE_OPERATING_STATUS, ENDAT2P2_MEM_WORD_3);

    ENDAT2P2_SetRecoveryTimer(dev, 0);

    ENDAT2P2_GetEncoderError(dev);
    ENDAT2P2_GetEncoderWarning(dev);

    if (dev->cmd_set_2_2)
    {
        ENDAT2P2_ClearEncoderErrorWithPos(dev);

        ENDAT2P2_GetEncoderErrorWithPos(dev);
        ENDAT2P2_GetParamWithPos(dev, MRS_CODE_PARAM_ENCODER_MANUFACTURER_PAGE1, ENDAT2P2_MEM_WORD_1);
    }

    ////////////////////////////////////////////////////////////////////////////
    /* EnDat HW Strobe Loop */

    endat2p2_mode_cmd_t cmd = ENDAT2P2_CMD_SEND_POSITION_VALUE;

    /* reset additional info's if present */
    ENDAT2P2_EncoderRest(dev);
    SDK_DelayAtLeastUs(500U, SystemCoreClock);

    ENDAT2P2_CMDBuild(dev, cmd, 0, 0);

    ENDAT2P2_CleanStatus(dev);

    ENDAT2P2_SetHWStrobe(dev, true);

    ENDAT2P2_CleanStatus(dev);

    /* Enable interrupt */
    ENDAT2P2_SetInterruptMask(dev, ENDAT2P2_INTERRUPTMASKREGISTER_RECEIVE_REGISTER1_MASK_MASK);
    EnableIRQ(M2_ENDAT2P2_IRQn);
    NVIC_SetPriority(M2_ENDAT2P2_IRQn, 0U);
}
#endif

#if (M1_ENCODER == ENCODER_ENDAT2P2_2)
/*!
 * @brief      Init EnDat2.2_2 master IP for M1
 *
 * @param      void
 *
 * @return     none
 */
void InitEndat2p2_2(void)
{
    int data;
    endat2p2_dev_t *dev;
    int32_t SCMI_status = 0;
    uint32_t blk_ctrl_size = 0;
    uint32_t blk_ctrl_value = 0;
    __attribute__((unused)) int32_t result;

    /* EnDat2.2 100MHz */
    clk_t endat2p2Clk = {
        .clkId = kCLOCK_Endat22,
        .pclkId = kCLOCK_Syspll1dfs1div2, /* 400 MHz */
        .rate = ENDAT2P2_CLK_48M,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };

    CLOCK_SetParent(&endat2p2Clk);
    CLOCK_SetRate(&endat2p2Clk);
    CLOCK_EnableClock(endat2p2Clk.clkId);

    /* DIAG_ENCODER_MUX_SEL will be updated */
    SCMI_status = SCMI_MiscControlGet(SCMI_A2P, MC_SM_CTRL_ENC_DIAG_MUX_SEL, &blk_ctrl_size, &blk_ctrl_value);

    if (SCMI_status == SCMI_ERR_SUCCESS)
    {
      /* endat2p2 mux select configuration successful */
      result = SCMI_ERR_SUCCESS;
    }
    else if(SCMI_status == SCMI_ERR_NOT_FOUND)
    {
      result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
    }
    else if(SCMI_status == SCMI_ERR_DENIED)
    {
      result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
    }

    /* Select endat2p2 for encoder2 */
    blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_DIAG_ENCODER_MUX_SEL_diag_enc2_sel(DIG_ENCODER_MUX_ENDAT2P2);

    /* Update DIAG_ENCODER_MUX_SEL */
    SCMI_status = SCMI_MiscControlSet(SCMI_A2P, MC_SM_CTRL_ENC_DIAG_MUX_SEL, blk_ctrl_size, &blk_ctrl_value);

    if (SCMI_status == SCMI_ERR_SUCCESS)
    {
      /* endat2p2 for encoder2 configuration successful */
      result = SCMI_ERR_SUCCESS;
    }
    else if(SCMI_status == SCMI_ERR_NOT_FOUND)
    {
      result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
    }
    else if(SCMI_status == SCMI_ERR_DENIED)
    {
      result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
    }

    /* XBAR_TRIG_SYNC_CTRL2 will be updated */
    SCMI_status = SCMI_MiscControlGet(SCMI_A2P, MC_SM_CTRL_XBAR_TRIG_SYNC_2, &blk_ctrl_size, &blk_ctrl_value);

    if (SCMI_status == SCMI_ERR_SUCCESS)
    {
      /* XBAR_TRIG_SYNC_CTRL2 select successful */
      result = SCMI_ERR_SUCCESS;
    }
    else if(SCMI_status == SCMI_ERR_NOT_FOUND)
    {
      result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
    }
    else if(SCMI_status == SCMI_ERR_DENIED)
    {
      result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
    }

    /* Trigger out synchronizer enable */
    blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_XBAR_TRIG_SYNC_CTRL2_SYNC_ENABLE(1U);

    /* Update XBAR_TRIG_SYNC_CTRL2 */
    SCMI_status = SCMI_MiscControlSet(SCMI_A2P, MC_SM_CTRL_XBAR_TRIG_SYNC_2, blk_ctrl_size, &blk_ctrl_value);

    if (SCMI_status == SCMI_ERR_SUCCESS)
    {
      /* Trigger XBAR synchronizer enabled */
      result = SCMI_ERR_SUCCESS;
    }
    else if(SCMI_status == SCMI_ERR_NOT_FOUND)
    {
      result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
    }
    else if(SCMI_status == SCMI_ERR_DENIED)
    {
      result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
    }

    /* XBAR_TRIG_SYNC_CTRL3 will be updated */
    SCMI_status = SCMI_MiscControlGet(SCMI_A2P, MC_SM_CTRL_XBAR_TRIG_SYNC_3, &blk_ctrl_size, &blk_ctrl_value);

    if (SCMI_status == SCMI_ERR_SUCCESS)
    {
      /* XBAR_TRIG_SYNC_CTRL3 select successful */
      result = SCMI_ERR_SUCCESS;
    }
    else if(SCMI_status == SCMI_ERR_NOT_FOUND)
    {
      result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
    }
    else if(SCMI_status == SCMI_ERR_DENIED)
    {
      result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
    }
    /* XBAR trigger synchronizer control - set pulse width */
    blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_XBAR_TRIG_SYNC_CTRL3_PULSE_WIDTH0(7U);

    /* Update XBAR_TRIG_SYNC_CTRL3 */
    SCMI_status = SCMI_MiscControlSet(SCMI_A2P, MC_SM_CTRL_XBAR_TRIG_SYNC_3, blk_ctrl_size, &blk_ctrl_value);

    if (SCMI_status == SCMI_ERR_SUCCESS)
    {
      /* XBAR trigger synchronizer control - pulse width set successful */
      result = SCMI_ERR_SUCCESS;
    }
    else if(SCMI_status == SCMI_ERR_NOT_FOUND)
    {
      result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
    }
    else if(SCMI_status == SCMI_ERR_DENIED)
    {
      result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
    }

    /* ENDAT_STRETCHER_CTRL will be updated */
    SCMI_status = SCMI_MiscControlGet(SCMI_A2P, MC_SM_CTRL_ENDAT_STRETCH_CTRL, &blk_ctrl_size, &blk_ctrl_value);

    if (SCMI_status == SCMI_ERR_SUCCESS)
    {
      /* ENDAT_STRETCHER_CTRL select successful */
      result = SCMI_ERR_SUCCESS;
    }
    else if(SCMI_status == SCMI_ERR_NOT_FOUND)
    {
      result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
    }
    else if(SCMI_status == SCMI_ERR_DENIED)
    {
      result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
    }

    /* defines the number of cycle for the trigger signal coming from xbar */
    blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_ENDAT_STRETCHER_CTRL_endat2p2_nstr_value(3) |
      BLK_CTRL_WAKEUPMIX_ENDAT_STRETCHER_CTRL_endat2p2_nstr_ctrl(1);

    /* Update ENDAT_STRETCHER_CTRL */
    SCMI_status = SCMI_MiscControlSet(SCMI_A2P, MC_SM_CTRL_ENDAT_STRETCH_CTRL, blk_ctrl_size, &blk_ctrl_value);

    if (SCMI_status == SCMI_ERR_SUCCESS)
    {
      /* number of cycle for the trigger signal set */
      result = SCMI_ERR_SUCCESS;
    }
    else if(SCMI_status == SCMI_ERR_NOT_FOUND)
    {
      result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
    }
    else if(SCMI_status == SCMI_ERR_DENIED)
    {
      result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
    }

    XBAR_SetSignalsConnection(kXBAR1_InputFlexpwm2Mux0Trigger1, kXBAR1_OutputTriggerSyncAsyncIn0);
    XBAR_SetSignalsConnection(kXBAR1_InputTriggerSyncSyncOut0, kXBAR1_OutputEndat22StrN);
    dev = ENDAT2P2_InitMaster(ENDAT2P2_2, ENDAT2P2_CLK_48M);

    ////////////////////////////////////////////////////////////
    /* Init encoder - ENDAT2P2_InitEncoder(dev); */

    ENDAT2P2_EncoderRest(dev);
    SDK_DelayAtLeastUs((50 * 1000), SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    ENDAT2P2_ClearEncoderError(dev);
    SDK_DelayAtLeastUs((50 * 1000), SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    ENDAT2P2_ClearEncoderWarning(dev);
    SDK_DelayAtLeastUs((50 * 1000), SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    ENDAT2P2_GetEncoderInfo(dev);

    ENDAT2P2_SetDataWordLength(dev, dev->pos_res);

    //////////////////////////////////////////////////////////

    //ENDAT2P2_EnableDelayCompensation(dev);

    ENDAT2P2_SetFTCLOCK(dev, ENDAT2P2_CLK_8M); //ENDAT2P2_FTCLK

    /* Change tm to 3.75us */
    data = ENDAT2P2_GetParamWithPos(dev, MRS_CODE_OPERATING_STATUS, ENDAT2P2_MEM_WORD_3);
    data = (data & (~3)) | 0x1;

    ENDAT2P2_SetParamWithPos(dev, MRS_CODE_OPERATING_STATUS, ENDAT2P2_MEM_WORD_3, data);
    ENDAT2P2_EncoderRestWithPos(dev);

    data = ENDAT2P2_GetParamWithPos(dev, MRS_CODE_OPERATING_STATUS, ENDAT2P2_MEM_WORD_3);

    ENDAT2P2_SetRecoveryTimer(dev, 0);

    ENDAT2P2_GetEncoderError(dev);
    ENDAT2P2_GetEncoderWarning(dev);

    if(dev->cmd_set_2_2)
    {
        ENDAT2P2_ClearEncoderErrorWithPos(dev);

        ENDAT2P2_GetEncoderErrorWithPos(dev);
        ENDAT2P2_GetParamWithPos(dev, MRS_CODE_PARAM_ENCODER_MANUFACTURER_PAGE1, ENDAT2P2_MEM_WORD_1);
    }

    ////////////////////////////////////////////////////////////////////////////
    /* EnDat HW Strobe Loop */

    endat2p2_mode_cmd_t cmd = ENDAT2P2_CMD_SEND_POSITION_VALUE;

    /* reset additional info's if present */
    ENDAT2P2_EncoderRest(dev);
    SDK_DelayAtLeastUs(500U, SystemCoreClock);
    
    ENDAT2P2_CMDBuild(dev, cmd, 0, 0);

    ENDAT2P2_CleanStatus(dev);
    
    ENDAT2P2_SetHWStrobe(dev, true);

    ENDAT2P2_CleanStatus(dev);

    /* Enable interrupt */
    ENDAT2P2_SetInterruptMask(dev, ENDAT2P2_INTERRUPTMASKREGISTER_RECEIVE_REGISTER1_MASK_MASK);
    EnableIRQ(M1_ENDAT2P2_IRQn);
    NVIC_SetPriority(M1_ENDAT2P2_IRQn, 0U);

}
#endif

#if (M1_ENCODER == ENCODER_BISS) || (M2_ENCODER == ENCODER_BISS)

uint64_t BISS_ENC_GET_POSITION(biss_master_t *master)
{
  uint64_t position;
  position = BISS_SLVGetSCDRawData(master, 0);
  position = (position & (((uint64_t) 1 << (BISS_DEVICE_DATA_LEN)) - 1)) >> 2;
  return position;
}

/*!
 * @brief      Init BiSS master IP
 *
 * @param      void
 *
 * @return     none
 */
void InitBiSS1(void)
{
  xbar_control_config_t BissIRQxBARConfig;
  int32_t SCMI_status = 0;
  uint32_t blk_ctrl_size = 0;
  uint32_t blk_ctrl_value = 0;
  __attribute__((unused)) int32_t result;

  /* BiSS 20MHz */
  clk_t bissClk = {
      .clkId = BISS_SYS_CLK_ROOT,
      .pclkId = kCLOCK_Syspll1dfs1div2, /* 400 MHz */
      .rate = BISS_SYS_CLK_FREQ,
      .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
  };

  CLOCK_SetParent(&bissClk);
  CLOCK_SetRate(&bissClk);
  CLOCK_EnableClock(bissClk.clkId);

  /* DIAG_ENCODER_MUX_SEL will be updated */
  SCMI_status = SCMI_MiscControlGet(SCMI_A2P, MC_SM_CTRL_ENC_DIAG_MUX_SEL, &blk_ctrl_size, &blk_ctrl_value);

  if (SCMI_status == SCMI_ERR_SUCCESS)
  {
    /* endat2p2 mux select configuration successful */
    result = SCMI_ERR_SUCCESS;
  }
  else if(SCMI_status == SCMI_ERR_NOT_FOUND)
  {
    result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
  }
  else if(SCMI_status == SCMI_ERR_DENIED)
  {
    result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
  }

#if (M1_ENCODER == ENCODER_BISS)
  /* Select biss for encoder2 */
  blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_DIAG_ENCODER_MUX_SEL_diag_enc2_sel(DIG_ENCODER_MUX_BISS);
#elif (M2_ENCODER == ENCODER_BISS)
  /* Select biss for encoder1 */
  blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_DIAG_ENCODER_MUX_SEL_diag_enc1_sel(DIG_ENCODER_MUX_BISS);
#endif

  /* Update DIAG_ENCODER_MUX_SEL */
  SCMI_status = SCMI_MiscControlSet(SCMI_A2P, MC_SM_CTRL_ENC_DIAG_MUX_SEL, blk_ctrl_size, &blk_ctrl_value);

  if (SCMI_status == SCMI_ERR_SUCCESS)
  {
    /* DIAG_ENCODER_MUX_SEL register successful set */
    result = SCMI_ERR_SUCCESS;
  }
  else if(SCMI_status == SCMI_ERR_NOT_FOUND)
  {
    result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
  }
  else if(SCMI_status == SCMI_ERR_DENIED)
  {
    result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
  }

  /* XBAR_TRIG_SYNC_CTRL2 will be updated */
  SCMI_status = SCMI_MiscControlGet(SCMI_A2P, MC_SM_CTRL_XBAR_TRIG_SYNC_2, &blk_ctrl_size, &blk_ctrl_value);

  if (SCMI_status == SCMI_ERR_SUCCESS)
  {
    /* XBAR_TRIG_SYNC_CTRL2 select successful */
    result = SCMI_ERR_SUCCESS;
  }
  else if(SCMI_status == SCMI_ERR_NOT_FOUND)
  {
    result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
  }
  else if(SCMI_status == SCMI_ERR_DENIED)
  {
    result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
  }

  /* Trigger out synchronizer enable */
  blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_XBAR_TRIG_SYNC_CTRL2_SYNC_ENABLE(1U);

  /* Update XBAR_TRIG_SYNC_CTRL2 */
  SCMI_status = SCMI_MiscControlSet(SCMI_A2P, MC_SM_CTRL_XBAR_TRIG_SYNC_2, blk_ctrl_size, &blk_ctrl_value);

  if (SCMI_status == SCMI_ERR_SUCCESS)
  {
    /* XBAR_TRIG_SYNC_CTRL2 updated */
    result = SCMI_ERR_SUCCESS;
  }
  else if(SCMI_status == SCMI_ERR_NOT_FOUND)
  {
    result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
  }
  else if(SCMI_status == SCMI_ERR_DENIED)
  {
    result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
  }

  /* Update XBAR_TRIG_SYNC_CTRL3 */
  SCMI_status = SCMI_MiscControlGet(SCMI_A2P, MC_SM_CTRL_XBAR_TRIG_SYNC_3, &blk_ctrl_size, &blk_ctrl_value);

  if (SCMI_status == SCMI_ERR_SUCCESS)
  {
    /* endat3 mux select configuration successful */
    result = SCMI_ERR_SUCCESS;
  }
  else if(SCMI_status == SCMI_ERR_NOT_FOUND)
  {
    result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
  }
  else if(SCMI_status == SCMI_ERR_DENIED)
  {
    result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
  }

  /* Pulse width control register of channel0 */
  blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_XBAR_TRIG_SYNC_CTRL3_PULSE_WIDTH0(7U);

  /* XBAR_TRIG_SYNC_CTRL3 will be updated */
  SCMI_status = SCMI_MiscControlSet(SCMI_A2P, MC_SM_CTRL_XBAR_TRIG_SYNC_3, blk_ctrl_size, &blk_ctrl_value);

  if (SCMI_status == SCMI_ERR_SUCCESS)
  {
    /* XBAR_TRIG_SYNC_CTRL3 updated */
    result = SCMI_ERR_SUCCESS;
  }
  else if(SCMI_status == SCMI_ERR_NOT_FOUND)
  {
    result = SCMI_ERR_NOT_FOUND; /* does not point to a valid control */
  }
  else if(SCMI_status == SCMI_ERR_DENIED)
  {
    result = SCMI_ERR_DENIED; /* calling agent is not allowed to get this control */
  }

  /* EXTENDED PWM to trigger biss getsens */
  XBAR_SetSignalsConnection(kXBAR1_InputFlexpwm2Mux0Trigger1, kXBAR1_OutputTriggerSyncAsyncIn0);
  XBAR_SetSignalsConnection(kXBAR1_InputTriggerSyncSyncOut0, kXBAR1_OutputBissGetsens);

  /* Select Motor controller 1 */
  BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, ETH2_SEL);
  SDK_DelayAtLeastUs(100U, SystemCoreClock);

  /* BiSS driver initialization */
  status_t status;
  uint8_t slvID;
  biss_slave_info_t *slv;
  biss_master_t *master;

  /* Disable XBAR1 interrupt 0/1 */
  DisableIRQ(XBAR1_IRQn);

  master = BISS_MasterInit(BISS1, BISS_SYS_CLK_FREQ, BISS_MA_CLK_FREQ, BISS_AGS_CLK_FREQ);

  if (master == NULL)
  {
      return;
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
      return;
  }

  for (slvID = 0; slvID < master->slvCnt; slvID++)
  {
      slv = BISS_SLVGet(master, slvID);
      if (slv->dataLen == 0)
          slv->dataLen = BISS_DEVICE_DATA_LEN;
      if (slv->crcLen == 0)
          slv->crcLen = BISS_DEVICE_CRC_LEN;
  }

  /* Manually initialize the slaves */
  BISS_SLVSetSCD(master, 0, BISS_DEVICE_DATA_LEN, BISS_DEVICE_CRC_LEN);
  BISS_ChangeTriggerMode(master, BISS_PIN_TRIGGER);

  /* Configure biss EOT interrupt to XBAR1_CH0 IRQ */
  XBAR_SetSignalsConnection(kXBAR1_InputBissEot, kXBAR1_OutputEdma4IpdReq76);
  BissIRQxBARConfig.activeEdge = kXBAR_EdgeRising;
  BissIRQxBARConfig.requestType = kXBAR_RequestInterruptEnable;
  XBAR_SetOutputSignalConfig(kXBAR1_OutputEdma4IpdReq76, &BissIRQxBARConfig);

  /* Enable XBAR1 interrupt 0/1 */
  EnableIRQ(XBAR1_IRQn);
}       /* BiSS encoder is used. */
#endif

/*!
 * @brief      M1 encoder initialization
 *
 * @param      void
 *
 * @return     none
 */
void M1_Encoder_init(void)
{
#if (M1_ENCODER == ENCODER_ENDAT2P2_2)
    InitEndat2p2_2();
    g_sM1Enc.dev = ENDAT2P2_GetDev(ENDAT2P2_2);

#elif (M1_ENCODER == ENCODER_ENDAT3)
    InitEndat3();
    g_sM1Enc.pui32EnDat3BaseAddress = (ENDAT3_Type *)ENDAT3;
#elif (M1_ENCODER == ENCODER_BISS)
    InitBiSS1();
    /* BISSC M1 structure */
    g_sM1Enc.mt = 0U;
    g_sM1Enc.st = 0U;
    g_sM1Enc.mt_offset = 0U;
    g_sM1Enc.st_offset = 0U;
    g_sM1Enc.pMaster = BISS_GetMaster(BISS1);
#endif
    g_sM1Enc.ui16Pp = M1_MOTOR_PP;
}

/*!
 * @brief      M2 encoder initialization
 *
 * @param      void
 *
 * @return     none
 */
void M2_Encoder_init(void)
{
#if (M2_ENCODER == ENCODER_ENDAT2P2_1)
    InitEndat2p2_1();
    g_sM2Enc.dev = ENDAT2P2_GetDev(ENDAT2P2_1);
#elif (M2_ENCODER == ENCODER_ENDAT3)
    InitEndat3();
    g_sM2Enc.pui32EnDat3BaseAddress = (ENDAT3_Type *)ENDAT3;
#elif (M1_ENCODER == ENCODER_BISS)
    InitBiSS1();
    /* BISSC M1 structure */
    g_sM2Enc.mt = 0U;
    g_sM2Enc.st = 0U;
    g_sM2Enc.mt_offset = 0U;
    g_sM2Enc.st_offset = 0U;
    g_sM2Enc.pMaster = BISS_GetMaster(BISS1);
#endif
    g_sM2Enc.ui16Pp = M2_MOTOR_PP;
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
