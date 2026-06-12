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
}

void BOARD_InitHardware(void)
{
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
	BLK_CTRL_WAKEUPMIX_Type *blk_base = BLK_CTRL_WAKEUPMIX;

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

	blk_base->DIAG_ENCODER_MUX_SEL =
		BLK_CTRL_WAKEUPMIX_DIAG_ENCODER_MUX_SEL_diag_enc1_sel(DIG_ENCODER_MUX_HIPERFACE_DSL) |
		BLK_CTRL_WAKEUPMIX_DIAG_ENCODER_MUX_SEL_diag_enc2_sel(DIG_ENCODER_MUX_HIPERFACE_DSL);

	/* Select Motor controller 1 */
	BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, ETH2_SEL);
	BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, ETH2_SEL);

	/* Select Motor controller 2 */
	BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, ETH3_SEL);
	BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, ETH3_SEL);
	SDK_DelayAtLeastUs(100U, SystemCoreClock);

	XBAR_Init(kXBAR_DSC1);
	xbar_control_config_t xbaraConfig;
	xbaraConfig.activeEdge                   = kXBAR_EdgeRising;
	xbaraConfig.requestType                  = kXBAR_RequestInterruptEnable;
	XBAR_SetSignalsConnection(kXBAR1_InputFlexpwm1Mux1Trigger0, kXBAR1_OutputHiperface1SyncXbar);
	XBAR_SetSignalsConnection(kXBAR1_InputFlexpwm1Mux0Trigger0, kXBAR1_OutputEdma4IpdReq76);
	/*output the tripgger signal to verify the synchronization on Sync Mode and Stetcher/Divider */
	XBAR_SetSignalsConnection(kXBAR1_InputFlexpwm1Mux1Trigger0, kXBAR1_OutputIomuxXbarOut07);
	XBAR_SetOutputSignalConfig(kXBAR1_OutputEdma4IpdReq76, &xbaraConfig);

	blk_base->HIPERFACE_EXT_SYNC_OUT_CTL = 0x3;

	blk_base->HIPERFACE1_SYNC_CTL1 = BLK_CTRL_WAKEUPMIX_HIPERFACE1_SYNC_CTL1_clk_source_sel(0x00);
	blk_base->HIPERFACE1_SYNC_CTL1 |= BLK_CTRL_WAKEUPMIX_HIPERFACE1_SYNC_CTL1_sync_clk_enable_MASK;

	blk_base->HIPERFACE1_SYNC_CTL1 &= ~BLK_CTRL_WAKEUPMIX_HIPERFACE1_SYNC_CTL1_sync_source_sel_MASK; 
	blk_base->HIPERFACE1_SYNC_CTL1 |= BLK_CTRL_WAKEUPMIX_HIPERFACE1_SYNC_CTL1_sync_source_sel(0);
	blk_base->HIPERFACE1_SYNC_CTL2 = 134; // Minimum sync signal high/active duration: 1us.
	blk_base->HIPERFACE1_SYNC_CTL1 &= ~BLK_CTRL_WAKEUPMIX_HIPERFACE1_SYNC_CTL1_stretch_bypass_MASK;

	blk_base->HIPERFACE1_SYNC_CTL1 &= ~BLK_CTRL_WAKEUPMIX_HIPERFACE1_SYNC_CTL1_sync_div_enable_MASK;

	blk_base->HIPERFACE1_SYNC_CTL1 |= BLK_CTRL_WAKEUPMIX_HIPERFACE1_SYNC_CTL1_sync_enable_MASK;
}

void hiperface_fast_pos_irq_enable()
{
	BLK_CTRL_WAKEUPMIX_Type *blk_base = BLK_CTRL_WAKEUPMIX;
	blk_base->HIPERFACE1_INT_CTL |= (1 << 5);
}

void hiperface_fast_pos_irq_disable()
{
	BLK_CTRL_WAKEUPMIX_Type *blk_base = BLK_CTRL_WAKEUPMIX;
	blk_base->HIPERFACE1_INT_CTL &= ~(1 << 5);
}

void hiperface_clear_fast_pos_irq_status()
{
	BLK_CTRL_WAKEUPMIX_Type *blk_base = BLK_CTRL_WAKEUPMIX;
	blk_base->HIPERFACE1_INT_CTL |= (1 << 1);
}

/*${function:end}*/
