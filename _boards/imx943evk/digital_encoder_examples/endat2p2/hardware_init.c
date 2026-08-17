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
    int32_t SCMI_status = 0;
    uint32_t blk_ctrl_size = 0;
    uint32_t blk_ctrl_value = 0;
    /* EnDat2.2 100MHz */
    clk_t endat2p2Clk = {
        .clkId = ENDAT2P2_SYS_CLK_ROOT,
        .pclkId = kCLOCK_Syspll1dfs1div2, /* 400 MHz */
        .rate = ENDAT2P2_SYS_CLOCK,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };

    SystemPlatformInit();
    BOARD_InitDebugConsolePins();
#if (ENDAT2_MUX == MOTOR_CTRL1)
    BOARD_InitEncoder2Pins();
#else
    BOARD_InitEncoder1Pins();
#endif
    BOARD_InitI2C6Pins();

    BOARD_InitPWM1Pins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    CLOCK_SetParent(&endat2p2Clk);
    CLOCK_SetRate(&endat2p2Clk);
    CLOCK_EnableClock(endat2p2Clk.clkId);


    /* DIAG_ENCODER_MUX_SEL will be updated */
    SCMI_status = SCMI_MiscControlGet(SCMI_A2P, DEV_SM_CTRL_ENC_DIAG_MUX_SEL, &blk_ctrl_size, &blk_ctrl_value);
    if (SCMI_status != SCMI_ERR_SUCCESS) {
        /* Encoder mux select configuration failed */
        return;
    }

#if (ENDAT2_MUX == MOTOR_CTRL1)
    /* Select Endat2 for encoder2 */
    blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_DIAG_ENCODER_MUX_SEL_diag_enc2_sel(DIG_ENCODER_MUX_ENDAT2P2);
#elif (ENDAT3_MUX == MOTOR_CTRL2)
    /* Select Endat2 for encoder1 */
    blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_DIAG_ENCODER_MUX_SEL_diag_enc1_sel(DIG_ENCODER_MUX_ENDAT2P2);
#endif

    /* Update DIAG_ENCODER_MUX_SEL */
    SCMI_status = SCMI_MiscControlSet(SCMI_A2P, DEV_SM_CTRL_ENC_DIAG_MUX_SEL, blk_ctrl_size,
                                      &blk_ctrl_value);
    if (SCMI_status != SCMI_ERR_SUCCESS) {
        /* Failed to set DIAG_ENCODER_MUX_SEL register  */
        return;
    }

    /* ENDAT_STRETCHER_CTRL will be updated */
    SCMI_status = SCMI_MiscControlGet(SCMI_A2P, DEV_SM_CTRL_ENDAT_STRETCH_CTRL, &blk_ctrl_size, &blk_ctrl_value);
    if (SCMI_status != SCMI_ERR_SUCCESS) {
        /* Encoder mux select configuration failed */
        return;
    }

#if (ENDAT2_MUX == MOTOR_CTRL1)
    /* Select Endat22 for encoder2 */
    blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_ENDAT_STRETCHER_CTRL_endat2p2_nstr_value(3) |
                      BLK_CTRL_WAKEUPMIX_ENDAT_STRETCHER_CTRL_endat2p2_nstr_ctrl(1);
#elif (ENDAT3_MUX == MOTOR_CTRL2)
    /* Select Endat21 for encoder1 */
    blk_ctrl_value |= BLK_CTRL_WAKEUPMIX_ENDAT_STRETCHER_CTRL_endat2p1_nstr_value(3) |
                      BLK_CTRL_WAKEUPMIX_ENDAT_STRETCHER_CTRL_endat2p1_nstr_ctrl(1);
#endif

    /* Update ENDAT_STRETCH_CTRL */
    SCMI_status = SCMI_MiscControlSet(SCMI_A2P, DEV_SM_CTRL_ENDAT_STRETCH_CTRL, blk_ctrl_size,
                                      &blk_ctrl_value);
    if (SCMI_status != SCMI_ERR_SUCCESS) {
        /* Failed to set DIAG_ENCODER_MUX_SEL register  */
        return;
    }

#if (ENDAT2_MUX == MOTOR_CTRL1)
    /* Select Motor controller 1 */
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, ETH2_SEL);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, ETH2_SEL);
#else
    /* Select Motor controller 2 */
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, ETH3_SEL);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, ETH3_SEL);
#endif

    SDK_DelayAtLeastUs(100U, SystemCoreClock);
}

void ENDAT2P2_EnableXbarPinTrigger(void)
{
    XBAR_Init(kXBAR_DSC1);
#if (ENDAT2_MUX == MOTOR_CTRL1)
    XBAR_SetSignalsConnection(kXBAR1_InputFlexpwm1Mux1Trigger0, kXBAR1_OutputEndat22StrN);
#else
    XBAR_SetSignalsConnection(kXBAR1_InputFlexpwm1Mux0Trigger0, kXBAR1_OutputEndat21StrN);
#endif
}
/*${function:end}*/