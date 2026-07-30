/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_xbar.h"
#include "fsl_pwm.h"
#include "app.h"
/*${header:end}*/

#define DIG_ENCODER_MUX_HIPERFACE_DSL   0x0
#define DIG_ENCODER_MUX_ENDAT2P2        0x1
#define DIG_ENCODER_MUX_ENDAT3          0x2
#define DIG_ENCODER_NUX_BISS            0x3

/*${function:start}*/
void PWM_Trigger_Init(PWM_Type *PWMBase)
{
	uint32_t pwmSourceClockInHz = PWM_SRC_CLK_FREQ / (1 << DEMO_PWM_CLOCK_DEVIDER);
	uint32_t temp = pwmSourceClockInHz / APP_DEFAULT_PWM_FREQUENCE;
	unsigned short int ui16M1PwmModulo = temp & 0xFFFF;
	unsigned short int ui16EnociderTransactionTime = TRANSACTION_TIME_US * pwmSourceClockInHz / 1000000U;
	/* Full cycle reload */
	PWMBase->SM[0].CTRL |= PWM_CTRL_FULL_MASK;
	
	PWMBase->SM[0].CTRL |= PWM_CTRL_PRSC(DEMO_PWM_CLOCK_DEVIDER);

	/* Value register initial values, duty cycle 50% */
	PWMBase->SM[0].INIT = (uint16_t)(-(ui16M1PwmModulo / 2));
	PWMBase->SM[0].VAL0 = PWM_VAL0_VAL0((uint16_t)(0));

	PWMBase->SM[0].VAL1 = ((ui16M1PwmModulo / 2) - 1);

	/* Trigger for Encoder synchronization */
	PWMBase->SM[0].VAL5 = -(ui16M1PwmModulo / 2) + 10;

	/* Trigger for interrupt synchronization */
	PWMBase->SM[0].VAL4 = ((ui16M1PwmModulo / 2 - 1) - ui16EnociderTransactionTime );

	/* PWM0 ~ PWM3 module 0 trigger on VAL4 enabled for ADC synchronization */
	PWMBase->SM[0].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 4);
	PWMBase->SM[0].TCTRL |= PWM_TCTRL_OUT_TRIG_EN(1 << 5);

	/* Master reload is generated every one opportunity */
	PWMBase->SM[0].CTRL = (PWMBase->SM[0].CTRL & ~PWM_CTRL_LDFQ_MASK) | PWM_CTRL_LDFQ(ENCODER_ACCESS_FREQ_VS_PWM_FRE0 - 1);

	/* Start PWM trigger*/
	PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_CLDOK_MASK) | PWM_MCTRL_CLDOK(0x1);
	PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_LDOK_MASK) | PWM_MCTRL_LDOK(0x1);
	PWMBase->MCTRL = (PWMBase->MCTRL & ~PWM_MCTRL_RUN_MASK) | PWM_MCTRL_RUN(0x1);

#if (HIPERFACE_MUX == MOTOR_CTRL1)
	XBAR_SetSignalsConnection(kXBAR1_InputFlexpwm1Mux1Trigger0, kXBAR1_OutputHiperface2SyncXbar);
#elif (HIPERFACE_MUX == MOTOR_CTRL2)
	XBAR_SetSignalsConnection(kXBAR1_InputFlexpwm1Mux1Trigger0, kXBAR1_OutputHiperface1SyncXbar);
#endif
}

void BOARD_InitHardware(void)
{
	int32_t SCMI_status = 0;
    uint32_t blk_ctrl_size = 0;
    uint32_t blk_ctrl_value = 0;
	/* Hiperface 75MHz */
	clk_t encoderplldfs0ctl = {
		.clkId = kCLOCK_Encoderplldfs0ctl,
		.rate = 75000000UL,
		.clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
	};

	clk_t encoderplldfs0 = {
		.clkId = kCLOCK_Encoderplldfs0,
		.rate = 75000000UL,
		.clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
	};

	clk_t hiperfaceClk = {
		.clkId = HIPERFACE_CLOCK_ROOT,
		.pclkId = kCLOCK_Encoderplldfs0, /* 75 MHz */
		.rate = 75000000UL,
		.clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
	};

	SystemPlatformInit();
	BOARD_InitDebugConsolePins();
    BOARD_InitEncoder2Pins();
    BOARD_InitEncoder1Pins();
    BOARD_InitI2C6Pins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

	CLOCK_SetRate(&encoderplldfs0ctl);
	CLOCK_EnableClock(encoderplldfs0ctl.clkId);

	CLOCK_SetRate(&encoderplldfs0);
	CLOCK_EnableClock(encoderplldfs0.clkId);

	CLOCK_SetParent(&hiperfaceClk);
	CLOCK_SetRate(&hiperfaceClk);
	CLOCK_EnableClock(hiperfaceClk.clkId);

	/* DIAG_ENCODER_MUX_SEL will be updated */
    SCMI_status = SCMI_MiscControlGet(SCMI_A2P, DEV_SM_CTRL_ENC_DIAG_MUX_SEL, &blk_ctrl_size, &blk_ctrl_value);
    if (SCMI_status != SCMI_ERR_SUCCESS) {
        /* Encoder mux select configuration failed */
        return;
    }

#if (HIPERFACE_MUX == MOTOR_CTRL1)
    /* Select Hiperface2 for encoder2 */
    blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_DIAG_ENCODER_MUX_SEL_diag_enc2_sel(DIG_ENCODER_MUX_HIPERFACE_DSL);
#elif (HIPERFACE_MUX == MOTOR_CTRL2)
    /* Select Hiperface1 for encoder1 */
    blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_DIAG_ENCODER_MUX_SEL_diag_enc1_sel(DIG_ENCODER_MUX_HIPERFACE_DSL);
#endif

    /* Update DIAG_ENCODER_MUX_SEL */
    SCMI_status = SCMI_MiscControlSet(SCMI_A2P, DEV_SM_CTRL_ENC_DIAG_MUX_SEL, blk_ctrl_size,
                                      &blk_ctrl_value);
    if (SCMI_status != SCMI_ERR_SUCCESS) {
        /* Failed to set DIAG_ENCODER_MUX_SEL register  */
        return;
    }

#if (HIPERFACE_MUX == MOTOR_CTRL1)
	/* Select Motor controller 1 */
	BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, ETH2_SEL);
	BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, ETH2_SEL);
#elif (HIPERFACE_MUX == MOTOR_CTRL2)
	/* Select Motor controller 2 */
	BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, ETH3_SEL);
	BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, ETH3_SEL);
#endif

	SDK_DelayAtLeastUs(100U, SystemCoreClock);

	XBAR_Init(DEMO_XBARA_BASEADDR);

	blk_ctrl_value = 134; // Minimum sync signal high/active duration: 1us.
#if (HIPERFACE_MUX == MOTOR_CTRL1)
    /* Update HIPERFACE2_SYNC_CTL2 */
    SCMI_status = SCMI_MiscControlSet(SCMI_A2P, DEV_SM_CTRL_HPF2_SYNC_SRC_CFG2, blk_ctrl_size,
                                      &blk_ctrl_value);
#elif (HIPERFACE_MUX == MOTOR_CTRL2)
    /* Update HIPERFACE1_SYNC_CTL2 */
    SCMI_status = SCMI_MiscControlSet(SCMI_A2P, DEV_SM_CTRL_HPF1_SYNC_SRC_CFG2, blk_ctrl_size,
                                      &blk_ctrl_value);
#endif
    if (SCMI_status != SCMI_ERR_SUCCESS) {
        /* Failed to set HIPERFACE1/2_SYNC_CTL2 register  */
        return;
    }

	blk_ctrl_value = BLK_CTRL_WAKEUPMIX_HIPERFACE1_SYNC_CTL1_clk_source_sel(0x00);
	blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_HIPERFACE1_SYNC_CTL1_sync_clk_enable_MASK;
	blk_ctrl_value &= ~BLK_CTRL_WAKEUPMIX_HIPERFACE1_SYNC_CTL1_sync_source_sel_MASK;
	blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_HIPERFACE1_SYNC_CTL1_sync_source_sel(0);
	blk_ctrl_value &= ~BLK_CTRL_WAKEUPMIX_HIPERFACE1_SYNC_CTL1_stretch_bypass_MASK;
	blk_ctrl_value &= ~BLK_CTRL_WAKEUPMIX_HIPERFACE1_SYNC_CTL1_sync_div_enable_MASK;
	blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_HIPERFACE1_SYNC_CTL1_sync_enable_MASK;
#if (HIPERFACE_MUX == MOTOR_CTRL1)
    /* Update HIPERFACE1_SYNC_CTL1 */
    SCMI_status = SCMI_MiscControlSet(SCMI_A2P, DEV_SM_CTRL_HPF2_SYNC_SRC_CFG1, blk_ctrl_size,
                                      &blk_ctrl_value);
#elif (HIPERFACE_MUX == MOTOR_CTRL2)
    /* Update HIPERFACE1_SYNC_CTL1 */
    SCMI_status = SCMI_MiscControlSet(SCMI_A2P, DEV_SM_CTRL_HPF1_SYNC_SRC_CFG1, blk_ctrl_size,
                                      &blk_ctrl_value);
#endif
    if (SCMI_status != SCMI_ERR_SUCCESS) {
        /* Failed to set HIPERFACE1/2_SYNC_CTL1 register  */
        return;
    }
}

 void hiperface_fast_pos_irq_enable()
{
#ifdef DEMO_HIPERFACE_POS_RCVD_VIA_XBAR
	xbar_control_config_t xbaraConfig;
	xbaraConfig.activeEdge                   = kXBAR_EdgeRising;
	xbaraConfig.requestType                  = kXBAR_RequestInterruptEnable;
	XBAR_SetOutputSignalConfig(DEMO_XBARA_IRQ_OUTPIT_SIGNAL, &xbaraConfig);
#endif

#if (HIPERFACE_MUX == MOTOR_CTRL1)
#ifdef DEMO_HIPERFACE_POS_RCVD_VIA_XBAR
	XBAR_SetSignalsConnection( kXBAR1_InputHiperface2FastPosRcvdEvt, DEMO_XBARA_IRQ_OUTPIT_SIGNAL);
#else
	BLK_CTRL_WAKEUPMIX_Type *blk_base = BLK_CTRL_WAKEUPMIX;
	blk_base->HIPERFACE2_INT_CTL |= (1 << 5);
#endif
#elif (HIPERFACE_MUX == MOTOR_CTRL2)
#ifdef DEMO_HIPERFACE_POS_RCVD_VIA_XBAR
	XBAR_SetSignalsConnection( kXBAR1_InputHiperface1FastPosRcvdEvt, DEMO_XBARA_IRQ_OUTPIT_SIGNAL);
#else
	BLK_CTRL_WAKEUPMIX_Type *blk_base = BLK_CTRL_WAKEUPMIX;
	blk_base->HIPERFACE1_INT_CTL |= (1 << 5);
#endif
#endif
}

void hiperface_fast_pos_irq_disable()
{
#ifdef DEMO_HIPERFACE_POS_RCVD_VIA_XBAR
    xbar_control_config_t xbaraConfig;
    xbaraConfig.activeEdge                   = kXBAR_EdgeRising;
    xbaraConfig.requestType                  = kXBAR_RequestInterruptEnable;
    XBAR_SetOutputSignalConfig(DEMO_XBARA_IRQ_OUTPIT_SIGNAL, &xbaraConfig);
#else
#if (HIPERFACE_MUX == MOTOR_CTRL1)
    BLK_CTRL_WAKEUPMIX_Type *blk_base = BLK_CTRL_WAKEUPMIX;
    blk_base->HIPERFACE2_INT_CTL &= ~(1 << 5);
#elif (HIPERFACE_MUX == MOTOR_CTRL2)
    BLK_CTRL_WAKEUPMIX_Type *blk_base = BLK_CTRL_WAKEUPMIX;
    blk_base->HIPERFACE1_INT_CTL &= ~(1 << 5);
#endif
#endif
}

void hiperface_clear_fast_pos_irq_status()
{
#ifdef DEMO_HIPERFACE_POS_RCVD_VIA_XBAR
   XBAR_ClearOutputStatusFlag(DEMO_XBARA_IRQ_OUTPIT_SIGNAL);
#else
#if (HIPERFACE_MUX == MOTOR_CTRL1)
    BLK_CTRL_WAKEUPMIX_Type *blk_base = BLK_CTRL_WAKEUPMIX;
    blk_base->HIPERFACE2_INT_CTL |= (1 << 1);
#elif (HIPERFACE_MUX == MOTOR_CTRL2)
    BLK_CTRL_WAKEUPMIX_Type *blk_base = BLK_CTRL_WAKEUPMIX;
    blk_base->HIPERFACE1_INT_CTL |= (1 << 1);
#endif
#endif
}

/*${function:end}*/
