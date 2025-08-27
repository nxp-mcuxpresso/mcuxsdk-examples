/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "fsl_port.h"
#include "board.h"
#include "app.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void APP_InitROSC(void)
{
    uint32_t i = 0;

    for (i = 0; i < 3; i++)
    {
        AON__SMM->RTC_DCDC_CNTRL = SMM_RTC_DCDC_CNTRL_ISO_MASK | SMM_RTC_DCDC_CNTRL_DGTL_RST_N_MASK;
        SDK_DelayAtLeastUs(1000, SystemCoreClock);
    }

    for (i = 0; i < 3; i++)
    {
        AON__SMM->RTC_DCDC_CNTRL =
            SMM_RTC_DCDC_CNTRL_ISO_MASK | SMM_RTC_DCDC_CNTRL_DGTL_RST_N_MASK | SMM_RTC_DCDC_CNTRL_ANA_RESET_N_MASK;
        SDK_DelayAtLeastUs(1000, SystemCoreClock);
    }

    AON__SMM->RTC_XTAL_CONFG1 = SMM_RTC_XTAL_CONFG1_CRNT_MROR_EN_MASK;
    SDK_DelayAtLeastUs(1000, SystemCoreClock);
    AON__SMM->RTC_XTAL_CONFG2 = SMM_RTC_XTAL_CONFG2_SUPDET_TM_SOX(2) | SMM_RTC_XTAL_CONFG2_HYSTEL_MASK;
    SDK_DelayAtLeastUs(1000, SystemCoreClock);
    AON__SMM->RTC_XTAL_CONFG1 = 0x80d8;
    SDK_DelayAtLeastUs(1000, SystemCoreClock);
    AON__SMM->BIAS_CTRL = SMM_BIAS_CTRL_XTAL_SOX_4P_DIS_MASK;
    SDK_DelayAtLeastUs(1000, SystemCoreClock);
    AON__SMM->RTC_XTAL_CONFG1 = 0x80d9;
    SDK_DelayAtLeastUs(1000, SystemCoreClock);
    AON__SMM->RTC_XTAL_CONFG2 =
        SMM_RTC_XTAL_CONFG2_GMSEL_MASK | SMM_RTC_XTAL_CONFG2_SOX_EN_MASK | SMM_RTC_XTAL_CONFG2_CAP_BNK_EN_MASK;
    SDK_DelayAtLeastUs(1000, SystemCoreClock);
    AON__SMM->RTC_DCDC_CNTRL = SMM_RTC_DCDC_CNTRL_ISO_MASK | SMM_RTC_DCDC_CNTRL_DGTL_RST_N_MASK |
                               SMM_RTC_DCDC_CNTRL_ANA_RESET_N_MASK | SMM_RTC_DCDC_CNTRL_LDO_EN_MASK |
                               SMM_RTC_DCDC_CNTRL_LDO_CONFIG(0x18);
    SDK_DelayAtLeastUs(1000, SystemCoreClock);

    AON__SMM->RTC_DCDC_CNTRL |= SMM_RTC_DCDC_CNTRL_LDO_EN_MASK | SMM_RTC_DCDC_CNTRL_LDO_CONFIG(0x18);

    AON__SMM->RTC_DCDC_CNTRL &= ~SMM_RTC_DCDC_CNTRL_ISO_MASK;

    AON__RTC_AON->RTC_ALV_DTCT &= ~RTC_RTC_ALV_DTCT_BYPASS_MASK;
    AON__RTC_AON->RTC_ALV_DTCT |= RTC_RTC_ALV_DTCT_DTCT_EN_MASK;

    SDK_DelayAtLeastUs(6400, SystemCoreClock);
    SDK_DelayAtLeastUs(6400, SystemCoreClock);

    /* wait until XTTAL FAIL goes away */
    do
    {
        AON__RTC_AON->INT = 0x001F; // clear all interrupts
        SDK_DelayAtLeastUs(6400, SystemCoreClock);
        // AON__SMM->RTC_XTAL_CONFG2 &= ~SMM_RTC_XTAL_CONFG2_XTM_MASK;
        // SDK_DelayAtLeastUs(6400, SystemCoreClock);
        // AON__SMM->RTC_XTAL_CONFG2 = SMM_RTC_XTAL_CONFG2_XTM_MASK;
    } while (AON__RTC_AON->INT & RTC_INT_XTAL_FAIL_IF_MASK);

    AON__RTC_AON->CONFIG = RTC_CONFIG_K_EN_MASK | RTC_CONFIG_XTAL32_EN_MASK | RTC_CONFIG_FREE_RUNNING_MASK;

    AON__SMM->CNFG &= 0xff3f;
    SDK_DelayAtLeastUs(100000, SystemCoreClock);
    AON__SMM->CNFG |= 0x00C0;
    SDK_DelayAtLeastUs(100000, SystemCoreClock);
}

void BOARD_Init96MClocksBoot(void)
{
    /* Config 32k Crystal Oscillator */
    /* Monitor is disabled */
    CLOCK_SetRoscMonitorMode(kSCG_RoscMonitorDisable);
    CLOCK_SetupFROHFClocking(96000000U, 0U); /* Setup FRO HF clock */

    CLOCK_SetupFRO12MClocking();             /* Setup FRO12M clock */

    CLOCK_EnableClock(kCLOCK_GateAonAPB);    /* Required for access to AON*/
    CLOCK_SetupFROAonClocking(2000000U);

    /*!< Set up clock selectors - Attach clocks to the peripheries */
    CLOCK_AttachClk(kFIRC_to_MAIN_CLK);            /* !< Switch MAIN_CLK to FIRC */
    CLOCK_AttachClk(kFIRC_to_FRO_HF);              /* !< Switch FRO_HF to FIRC */
    CLOCK_AttachClk(kCPU_CLK_to_SYSTICK);          /* !< Switch SYSTICK to CPU_CLK */
    CLOCK_AttachClk(kFRO_16k_to_OSTIMER0);         /* !< Switch OSTIMER0 to FRO_16k */
    CLOCK_AttachClk(kSLOW_CLK_to_CLKOUT);          /* !< Switch CLKOUT to SLOW_CLK */
    CLOCK_AttachClk(kFRO_HF_DIV_to_ADC0);          /* !< Switch ADC0 to FRO_HF_DIV */
    CLOCK_AttachClk(kCLK_1M_to_UTICK0);            /* !< Switch UTICK0 to CLK_1M */
    CLOCK_AttachClk(kFRO12M_to_CMP0);              /* !< Switch CMP0 to FRO_HF_DIV */
    CLOCK_AttachClk(kFRO12M_to_PERIPH_GROUP0);     /* !< Switch PERIPH_GROUP0 to FRO_12M */
    CLOCK_AttachClk(kFRO_HF_DIV_to_PERIPH_GROUP1); /* !< Switch PERIPH_GROUP1 to FRO_HF_DIV */

    /*!< Set up dividers */
    CLOCK_SetClockDiv(kCLOCK_DivAHBCLK, 1U);       /* !< Set AHBCLKDIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivFRO_HF_DIV, 1U);   /* !< Set MRCC_FRO_HF_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivWWDT0, 1U);        /* !< Set MRCC_WWDT0_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivCMP0_FUNC, 1U);    /* !< Set MRCC_CMP0_FUNC_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivTRACE, 1U);        /* !< Set MRCC_DBG_TRACE_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivADC0, 1U);         /* !< Set MRCC_ADC0_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivPeriphGroup0, 1U); /* !< Set MRCC_LPUART_DIV divider to value 1 */
    CLOCK_SetClockDiv(kCLOCK_DivPeriphGroup1, 1U); /* !< Set MRCC_LPUART_DIV divider to value 1 */

    CLOCK_EnableClock(kCLOCK_GatePERIPH_GROUP0);
    CLOCK_EnableClock(kCLOCK_GatePERIPH_GROUP1);

    /* Set SystemCoreClock variable */
    SystemCoreClock = 96000000U;
}

void APP_InitTamperPins()
{
    /* Write to PORT0: Peripheral clock is enabled */
    CLOCK_EnableClock(kCLOCK_GateAonPORT);
    CLOCK_EnableClock(kCLOCK_GateAonGPIO);

    /* Tamper0 */
    const port_pin_config_t port0_4_config = {/* Internal pull-up/down resistor is pullup */
                                              kPORT_PullDown,
                                              /* Low internal pull resistor value is selected. */
                                              kPORT_LowPullResistor,
                                              /* Fast slew rate is configured */
                                              kPORT_FastSlewRate,
                                              /* Passive input filter is disabled */
                                              kPORT_PassiveFilterEnable,
                                              /* Open drain output is disabled */
                                              kPORT_OpenDrainDisable,
                                              /* Low drive strength is configured */
                                              kPORT_HighDriveStrength,
                                              /* Normal drive strength is configured */
                                              kPORT_NormalDriveStrength,
                                              /* Pin is configured as GPIO */
                                              kPORT_MuxAlt0,
                                              /* Digital input enabled */
                                              kPORT_InputBufferEnable,
                                              /* Digital input is not inverted */
                                              kPORT_InputNormal,
                                              /* Pin Control Register fields [15:0] are not locked */
                                              kPORT_UnlockRegister};
    /* PORT0_4 is configured as GPIO input for tamper0 */
    PORT_SetPinConfig(AON__PORT0, 4U, &port0_4_config);
}

void BOARD_InitHardware(void)
{
    BOARD_InitSWD_DEBUGPins();
    BOARD_InitDEBUG_UARTPins();
    BOARD_Init96MClocksBoot();
    BOARD_InitDebugConsole();
    APP_InitTamperPins();
    APP_InitROSC();
}
/*${function:end}*/
