/*
 * Copyright 2025 NXP
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

#include "ecat_def.h"
#include "ecatslv.h"
#include "ecat_hw.h"
#include "ecatappl.h"

#define TIMER_IRQ_ID     GPT1_IRQn
#define TIMER            (GPT_Type *) GPT1
#define TIMER_IRQHandler GPT1_IRQHandler
#define TIMER_CLK_FREQ   CLOCK_GetRate(kCLOCK_Gpt1)
#define ECAT_CHANNEL            SCMI_A2P
#define ECAT_DOMAIN_ID          24
#define ECAT_DISABLE_FLAGS      0x2
#define ECAT_ENABLE_FLAGS       0x0
#define ECAT_REST_STATE         0

UINT32 EcatTimerCnt;

static void Ecat_KickOff(void)
{
    int32_t result = SCMI_ERR_SUCCESS;
    
	result = SCMI_Reset(ECAT_CHANNEL, ECAT_DOMAIN_ID, ECAT_DISABLE_FLAGS, ECAT_REST_STATE);
    if (result != SCMI_ERR_SUCCESS)
    {
        PRINTF("%s: %d, Failed to reset ecat\r\n", __func__, __LINE__);
    }
    SDK_DelayAtLeastUs(10000U, SystemCoreClock);

    /* EtherCAT port0 is in MII mode */
    BLK_CTRL_NETCMIX->CFG_ECAT &= ~(1 << BLK_CTRL_NETCMIX_CFG_ECAT_RMII_SEL0_SHIFT);
    /* EtherCAT port1 is in MII mode */
    BLK_CTRL_NETCMIX->CFG_ECAT &= ~(1 << BLK_CTRL_NETCMIX_CFG_ECAT_RMII_SEL1_SHIFT);

    /* ECAT_LINK_ACT[0] polarity control: Active LOW */
    BLK_CTRL_NETCMIX->CFG_ECAT &= ~(1 << BLK_CTRL_NETCMIX_CFG_ECAT_ECAT_LINK_ACT0_POL_SHIFT);
    /* ECAT_LINK_ACT[1] polarity control: Active LOW */
    BLK_CTRL_NETCMIX->CFG_ECAT &= ~(1 << BLK_CTRL_NETCMIX_CFG_ECAT_ECAT_LINK_ACT1_POL_SHIFT);

    /* 16 bit I2C memory address range */
    BLK_CTRL_NETCMIX->CFG_ECAT |= (1 << BLK_CTRL_NETCMIX_CFG_ECAT_PROM_SIZE_SHIFT);

    /* EtherCAT PHY_OFFSET_VEC */
    BLK_CTRL_NETCMIX->CFG_ECAT |= (BLK_CTRL_NETCMIX_CFG_ECAT_PHY_OFFSET_VEC(2));

    SDK_DelayAtLeastUs(10000U, SystemCoreClock);
    
    result = SCMI_Reset(ECAT_CHANNEL, ECAT_DOMAIN_ID, ECAT_ENABLE_FLAGS, ECAT_REST_STATE);
    if (result != SCMI_ERR_SUCCESS)
    {
        PRINTF("%s: %d, Failed to reset ecat\r\n", __func__, __LINE__);
    }
    SDK_DelayAtLeastUs(10000U, SystemCoreClock);
}

UINT16 HW_Init(void)
{
    UINT32 intMask;
    uint32_t gptFreq;
    gpt_config_t gptConfig;
	SystemPlatformInit();
    BOARD_InitDebugConsolePins();	

    clk_t ecatClk = {
        .clkId = kCLOCK_Ecat,
        .pclkId = kCLOCK_Syspll1dfs1div2, /* 400 MHz */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 100000000UL,
    };

    clk_t gpt2Clk = {
        .clkId = kCLOCK_Gpt2,
        .pclkId = kCLOCK_Syspll1dfs1div2, /* 400 MHz */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 50000000UL,
    };

    CLOCK_SetParent(&ecatClk);
    CLOCK_SetRate(&ecatClk);
    CLOCK_EnableClock(ecatClk.clkId);
    CLOCK_SetParent(&gpt2Clk);
    CLOCK_SetRate(&gpt2Clk);
    CLOCK_EnableClock(gpt2Clk.clkId);

    /* Init board hardware. */
    BOARD_InitEcatLinkPins();
#ifdef ECAT_RMII_PORT
    BOARD_InitEcatPortRmiiPins();
#else
    BOARD_InitEcatPortMiiPins();
#endif
    BOARD_InitEcatI2CPins();
    BOARD_InitEcatMDIOPins();
    BOARD_InitEcatCLKPins();
    BOARD_InitEcatResetPins();
    BOARD_InitI2C6Pins();
    BOARD_InitGPIOPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    PRINTF("Start the SSC EoE example...\r\n");

    /*Select ECAT1_CLK25*/
#if USING_ECAT_CLK25
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S1_ID, ETHD_REFCLK_A0);
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S1_ID, ETHD_REFCLK_A1);

    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C6_S1_ID, ETHD_REFCLK_A0);
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S1_ID, ETHD_REFCLK_A0);
    SDK_DelayAtLeastUs(100000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C6_S1_ID, ETHD_REFCLK_A1);
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S1_ID, ETHD_REFCLK_A1);
    SDK_DelayAtLeastUs(100000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
#else
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S1_ID, ETHD_REFCLK_A0);
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S1_ID, ETHD_REFCLK_A1);

    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C6_S1_ID, ETHD_REFCLK_A0);
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C6_S1_ID, ETHD_REFCLK_A1);
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
#endif

	/* Select ECAT_MII_SEL */
	BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S1_ID, ECAT_MII_SEL);

    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C6_S1_ID, ECAT_MII_SEL);
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S1_ID, ECAT_MII_SEL);
    SDK_DelayAtLeastUs(100000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
 
    /*Open ECAT EEPROM*/
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, CAN2_SEL);

    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C6_S3_ID, CAN2_SEL);
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C6_S3_ID, CAN2_SEL);
    SDK_DelayAtLeastUs(100000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

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

    intMask = 0x00;

    HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);

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
	Sync1_Isr();
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
    NVIC_DisableIRQ(Reserved192_IRQn);
    NVIC_DisableIRQ(Reserved194_IRQn);
    NVIC_DisableIRQ(Reserved195_IRQn);
    NVIC_DisableIRQ(TIMER_IRQ_ID);
}

void HW_SetLed(UINT8 RunLed, UINT8 ErrorLed)
{
}
