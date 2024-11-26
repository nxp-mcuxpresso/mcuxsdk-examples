/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_xbar.h"
#include "fsl_iomuxc.h"
#include "fsl_ecat.h"
#include "fsl_gpt.h"
#include "fsl_rgpio.h"
#include "hal_reset.h"

#include "ecat_def.h"
#include "ecatslv.h"
#include "ecat_hw.h"
#include "ecatappl.h"

#define TIMER_IRQ_ID     GPT2_IRQn
#define TIMER            GPT2
#define TIMER_IRQHandler GPT2_IRQHandler
#define TIMER_CLK_FREQ   HAL_ClockGetRate(hal_clock_gpt2)

UINT32 EcatTimerCnt;

static void Ecat_KickOff(void)
{
    hal_rst_t hal_ecat_disable = {
        .id = 24,
        .flags = 2,
        .resetState = 0,
    };
    HAL_Reset(&hal_ecat_disable);

	SRC_XSPR_NETCMIX->IRST_REQ_CTRL |= 1 << 0x01;  // disable ecat
    BLK_CTRL_NETCMIX->CFG_ECAT |= BLK_CTRL_NETCMIX_CFG_ECAT_ecat_pt0_tx_clk_obe_MASK |
                                         BLK_CTRL_NETCMIX_CFG_ECAT_ecat_pt1_tx_clk_obe_MASK;

    BLK_CTRL_NETCMIX->CFG_ECAT &= ~(1 << BLK_CTRL_NETCMIX_CFG_ECAT_ECAT_LINK_ACT0_POL_SHIFT);
    BLK_CTRL_NETCMIX->CFG_ECAT &= ~(1 << BLK_CTRL_NETCMIX_CFG_ECAT_ECAT_LINK_ACT1_POL_SHIFT);

    BLK_CTRL_NETCMIX->CFG_ECAT &= ~(1 << BLK_CTRL_NETCMIX_CFG_ECAT_PROM_SIZE_SHIFT);

    hal_rst_t hal_ecat_enable = {
        .id = 24,
        .flags = 0,
        .resetState = 0,
    };
    HAL_Reset(&hal_ecat_enable);
}

UINT16 HW_Init(void)
{
    UINT32 intMask;
    uint32_t gptFreq;
    gpt_config_t gptConfig;
	SM_Platform_Init();	

    hal_clk_t hal_ecatClk = {
        .clk_id = hal_clock_ecat,
        .pclk_id = hal_clock_syspll1dfs1div2, /* 400 MHz */
        .clk_round_opt = hal_clk_round_auto,
        .rate = 100000000UL,
    };

    hal_clk_t hal_gpt2Clk = {
        .clk_id = hal_clock_gpt2,
        .pclk_id = hal_clock_syspll1dfs1div2, /* 400 MHz */
        .clk_round_opt = hal_clk_round_auto,
        .rate = 50000000UL,
    };

    HAL_ClockSetParent(&hal_ecatClk);
    HAL_ClockSetRate(&hal_ecatClk);
    HAL_ClockEnable(&hal_ecatClk);
    HAL_ClockSetParent(&hal_gpt2Clk);
    HAL_ClockSetRate(&hal_gpt2Clk);
    HAL_ClockEnable(&hal_gpt2Clk);

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("Start the SSC digital_io example...\r\n");
    /* Reset ecat PHY */
	/*
	 * Phy reset via I2C GPIO chip. todo 
	*/
    Ecat_KickOff();

#if BOARD_IMX943_TYPE == BOARD_IMX943_EVK
    SDK_DelayAtLeastUs(90000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
 
	/*Select Extended Register Page 0*/
    ECAT_EscMdioWrite(ETHERCAT, 0x00, 0x13, 0x00);
   
	 /*Select Mode 1: LED_LINK = ON for Good Link, OFF for No Link*/
    ECAT_EscMdioWrite(ETHERCAT, 0x00, 0x19, (1 << 5));
	
	/*Select Extended Register Page 0*/
    ECAT_EscMdioWrite(ETHERCAT, 0x01, 0x13, 0x00);

    /*Select Mode 1: LED_LINK = ON for Good Link, OFF for No Link*/
    ECAT_EscMdioWrite(ETHERCAT, 0x01, 0x19, (1 << 5));
#endif

 //   do
//    {
//        intMask = 0x93;
//        HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
//        intMask = 0;
//        HW_EscReadDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
//    } while (intMask != 0x93);

    intMask = 0x00;

    HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);

	PRINTF("IRST_REQ_CTRL4 = 0x%x\r\n", SRC_XSPR_NETCMIX->IRST_REQ_CTRL);
    /*Enable GPT*/
    GPT_GetDefaultConfig(&gptConfig);

    GPT_Init(TIMER, &gptConfig);

    gptFreq = TIMER_CLK_FREQ;
    GPT_SetClockDivider(TIMER, 100);

    GPT_SetOutputCompareValue(TIMER, kGPT_OutputCompare_Channel1, gptFreq / 100000);
    GPT_EnableInterrupts(TIMER, kGPT_OutputCompare1InterruptEnable);
    EnableIRQ(TIMER_IRQ_ID);

    /*Enable PDI IRQ*/
    EnableIRQ(Reserved192_IRQn);
    EnableIRQ(Reserved194_IRQn);
    EnableIRQ(Reserved195_IRQn);
    GPT_StartTimer(TIMER);
    return 0;
}

void Reserved192_IRQHandler(void)
{
    PDI_Isr();

    SDK_ISR_EXIT_BARRIER;
}

/*config Sync0 IRQ*/
void Reserved194_IRQHandler(void)
{
	Sync0_Isr();
    SDK_ISR_EXIT_BARRIER;
}

/*config Sync1 IRQ*/
void Reserved195_IRQHandler(void)
{
	Sync0_Isr();
    SDK_ISR_EXIT_BARRIER;
}

void HW_Release(void)
{
}

void TIMER_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    GPT_ClearStatusFlags(TIMER, kGPT_OutputCompare1Flag);
#if ECAT_TIMER_INT
    ECAT_CheckTimer();
#endif
    EcatTimerCnt++;

    SDK_ISR_EXIT_BARRIER;
}

UINT16 HW_GetTimer(void)
{
    return EcatTimerCnt;
}

void HW_ClearTimer(void)
{
    EcatTimerCnt = 0;
}

void ENABLE_ESC_INT(void)
{
    NVIC_EnableIRQ(Reserved192_IRQn);
    NVIC_EnableIRQ(Reserved194_IRQn);
    NVIC_EnableIRQ(Reserved195_IRQn);
    NVIC_EnableIRQ(TIMER_IRQ_ID);
}

void DISABLE_ESC_INT(void)
{
    NVIC_DisableIRQ(XBAR1_CH0_CH1_IRQn);
    NVIC_DisableIRQ(Reserved194_IRQn);
    NVIC_DisableIRQ(Reserved195_IRQn);
    NVIC_DisableIRQ(TIMER_IRQ_ID);
}

void HW_SetLed(UINT8 RunLed, UINT8 ErrorLed)
{
		return kStatus_Success;
}
