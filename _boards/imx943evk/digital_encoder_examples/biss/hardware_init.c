/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "board.h"
#include "pin_mux.h"
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

    PWMBase->SM[1].VAL1 = ((ui16M1PwmModulo / 2) - 1);


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
    /* BiSS 20MHz */
    clk_t bissClk = {
        .clkId = BISS_SYS_CLK_ROOT,
        .pclkId = kCLOCK_Syspll1dfs1div2, /* 400 MHz */
        .rate = BISS_SYS_CLK_FREQ,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };

    BLK_CTRL_WAKEUPMIX_Type *blk_ctrl = BLK_CTRL_WAKEUPMIX;

    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
    BOARD_InitEncoder1Pins();

    BOARD_InitEncoder2Pins();

    BOARD_InitI2C6Pins();

    BOARD_InitPWM1Pins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    CLOCK_SetParent(&bissClk);
    CLOCK_SetRate(&bissClk);
    CLOCK_EnableClock(bissClk.clkId);

    // encoder 1 select BiSS
    blk_ctrl->DIAG_ENCODER_MUX_SEL =
        BLK_CTRL_WAKEUPMIX_DIAG_ENCODER_MUX_SEL_diag_enc1_sel(DIG_ENCODER_NUX_BISS);

/*
    blk_ctrl->BISS1_GETSENS_PULSE_STRETCHER_CTL =
        BLK_CTRL_WAKEUPMIX_BISS1_GETSENS_PULSE_STRETCHER_CTL_value(3) |
        BLK_CTRL_WAKEUPMIX_BISS1_GETSENS_PULSE_STRETCHER_CTL_stretcher_en(1);
*/

    /* Select Motor controller 1 */
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, ETH3_SEL);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, ETH3_SEL);
    SDK_DelayAtLeastUs(100U, SystemCoreClock);
}
/*${function:end}*/