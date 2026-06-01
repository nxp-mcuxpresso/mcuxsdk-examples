/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_power.h"
/*${header:end}*/

/* Rx buffer memeory type. */
typedef uint8_t rx_buffer_t[EP_RXBUFF_SIZE_ALIGN];

/* dc sync time */
uint8_t  dc_started = 0;
int32_t  dc_diff_ns = 0;
int32_t  prev_dc_diff_ns = 0;
int64_t  dc_adjust_ns;
int64_t  system_time_offset = 0LL;

struct netc_ep_if_port if_port;

volatile uint64_t system_time_ns = 0;

/* Buffer descriptor resource. */
AT_NONCACHEABLE_SECTION_ALIGN(static netc_rx_bd_t g_rxBuffDescrip[EP_RING_NUM][EP_RXBD_NUM],
                              EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static rx_buffer_t g_rxDataBuff[EP_RING_NUM][EP_RXBD_NUM],
                              EP_BUFF_SIZE_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t g_txFrame[EP_TXBUFF_SIZE], EP_BUFF_SIZE_ALIGN);
#if !(defined(EXAMPLE_NETC_HAS_NO_SWITCH) && EXAMPLE_NETC_HAS_NO_SWITCH)
AT_NONCACHEABLE_SECTION_ALIGN(static netc_tx_bd_t g_mgmtTxBuffDescrip[EP_TXBD_NUM], EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static netc_cmd_bd_t g_cmdBuffDescrip[EP_TXBD_NUM], EP_BD_ALIGN);
#endif
static uint64_t rxBuffAddrArray[EP_RING_NUM][EP_RXBD_NUM];
#if !(defined(EXAMPLE_NETC_HAS_NO_SWITCH) && EXAMPLE_NETC_HAS_NO_SWITCH)
static netc_tx_frame_info_t g_mgmtTxDirty[EP_TXBD_NUM];
#endif
#if defined(EXAMPLE_EP_NUM) && EXAMPLE_EP_NUM
AT_NONCACHEABLE_SECTION_ALIGN(static netc_tx_bd_t g_txBuffDescrip[EP_RING_NUM][EP_TXBD_NUM],
                              EP_BD_ALIGN);
static netc_tx_frame_info_t g_txDirty[EP_RING_NUM][EP_TXBD_NUM];
#endif

void irq_wake_task(void)
{
    return;
}

void OSAL_TIMER_IRQHandler(void)
{
	/* Clear interrupt flag. */
	GPT_ClearStatusFlags(OSAL_TIMER, kGPT_OutputCompare1Flag);

	system_time_ns += CLOCK_INCREASE_PER_SEC;

	irq_wake_task();
	/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F, Cortex-M7, Cortex-M7F Store immediate overlapping
 	 exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U || __CORTEX_M == 7U)
	__DSB();
#endif
}

void osal_timer_init(uint32_t priority)
{
	uint32_t gptFreq;
	gpt_config_t gptConfig;

	assert(priority < (1UL << __NVIC_PRIO_BITS));

	GPT_GetDefaultConfig(&gptConfig);
	GPT_Init(OSAL_TIMER, &gptConfig);
	gptFreq = OSAL_TIMER_CLK_FREQ;
	/* Divide GPT clock source frequency to 1MHz */
	GPT_SetClockDivider(OSAL_TIMER, gptFreq / CLOCK_GRANULARITY_FRE);
	/* Set both GPT modules to 1 second duration */
	GPT_SetOutputCompareValue(OSAL_TIMER, kGPT_OutputCompare_Channel1, CLOCK_GRANULARITY_FRE);
	/* Enable GPT Output Compare1 interrupt */
	GPT_EnableInterrupts(OSAL_TIMER, kGPT_OutputCompare1InterruptEnable);
	/* Enable at the Interrupt */
	NVIC_SetPriority(OSAL_TIMER_IRQ_ID, priority);
	EnableIRQ(OSAL_TIMER_IRQ_ID);
	GPT_StartTimer(OSAL_TIMER);
}

void msgintrCallback(MSGINTR_Type *base, uint8_t channel, uint32_t pendingIntr)
{
    /* Transmit interrupt */
    if ((pendingIntr & (1U << TX_INTR_MSG_DATA)) != 0U)
    {
        EP_CleanTxIntrFlags(&if_port.g_ep_handle, 1, 0);
        EP_ReclaimTxDescriptor(&if_port.g_ep_handle, 0);
    }
    /* Receive interrupt */
    if ((pendingIntr & (1U << RX_INTR_MSG_DATA)) != 0U)
    {
        EP_CleanRxIntrFlags(&if_port.g_ep_handle, 1);
    }
}

uint64_t system_time64_ns()
{
	uint64_t nsec_base;
	uint32_t cur_nsec;

	nsec_base  = system_time_ns;
	cur_nsec   = GPT_GetCurrentTimerCount(OSAL_TIMER);

	if (nsec_base != system_time_ns)
	{
		nsec_base  = system_time_ns;
		cur_nsec   = GPT_GetCurrentTimerCount(OSAL_TIMER);
	}
	return nsec_base + cur_nsec * CLOCK_GRANULARITY_NS - system_time_offset;
}

void nsleep_to (uint64_t nsec_target)
{
	while (nsec_target > system_time64_ns());
}

void osal_gettime(struct timeval *current_time)
{
	uint64_t nsec = system_time64_ns();
	
	current_time->tv_sec  = nsec / CLOCK_INCREASE_PER_SEC;
	current_time->tv_usec = (nsec % CLOCK_INCREASE_PER_SEC) / 1000;

	return;
}

static netc_rx_bdr_config_t rxBdrConfig = {0};
static netc_tx_bdr_config_t txBdrConfig = {0};
static netc_bdr_config_t bdrConfig;
static netc_msix_entry_t msixEntry[2];
static ep_config_t ep_config;

/* OSHW: register enet port to SOEM stack */
int if_port_init(void)
{
	struct soem_if_port soem_port;

    bdrConfig.rxBdrConfig = &rxBdrConfig;
	bdrConfig.txBdrConfig = &txBdrConfig;
    uint32_t msgAddr;

    PRINTF("\r\nNETC EP frame loopback example start.\r\n");

    /* MSIX and interrupt configuration. */
    MSGINTR_Init(MSGINTR1, &msgintrCallback);
    msgAddr              = MSGINTR_GetIntrSelectAddr(MSGINTR1, 0);
    msixEntry[0].control = kNETC_MsixIntrMaskBit;
    msixEntry[0].msgAddr = msgAddr;
    msixEntry[0].msgData = TX_INTR_MSG_DATA;
    msixEntry[1].control = kNETC_MsixIntrMaskBit;
    msixEntry[1].msgAddr = msgAddr;
    msixEntry[1].msgData = RX_INTR_MSG_DATA;

    /* BD ring configuration. */
    bdrConfig.rxBdrConfig[0].bdArray       = &g_rxBuffDescrip[0][0];
    bdrConfig.rxBdrConfig[0].len           = EP_RXBD_NUM;
    bdrConfig.rxBdrConfig[0].buffAddrArray = &rxBuffAddrArray[0][0];
    bdrConfig.rxBdrConfig[0].buffSize      = EP_RXBUFF_SIZE_ALIGN;
    bdrConfig.rxBdrConfig[0].msixEntryIdx  = RX_MSIX_ENTRY_IDX;
    bdrConfig.rxBdrConfig[0].extendDescEn  = false;
    bdrConfig.rxBdrConfig[0].enThresIntr   = true;
    bdrConfig.rxBdrConfig[0].enCoalIntr    = true;
    bdrConfig.rxBdrConfig[0].intrThreshold = 1;

    bdrConfig.txBdrConfig[0].bdArray      = &g_txBuffDescrip[0][0];
    bdrConfig.txBdrConfig[0].len          = EP_TXBD_NUM;
    bdrConfig.txBdrConfig[0].dirtyArray   = &g_txDirty[0][0];
    bdrConfig.txBdrConfig[0].msixEntryIdx = TX_MSIX_ENTRY_IDX;
    bdrConfig.txBdrConfig[0].enIntr       = true;

    /* Endpoint configuration. */
    (void)EP_GetDefaultConfig(&ep_config);
    ep_config.si                    = KNETC_EP_CONFIG_SI;
    ep_config.siConfig.txRingUse    = 1;
    ep_config.siConfig.rxRingUse    = 1;
    //ep_config.reclaimCallback       = ReclaimCallback;
    ep_config.msixEntry             = &msixEntry[0];
    ep_config.entryNum              = 2;
    ep_config.port.ethMac.miiMode   = KNETC_HW_MII_MODE;
    ep_config.port.ethMac.miiSpeed  = kNETC_MiiSpeed100M;
    ep_config.port.ethMac.miiDuplex = kNETC_MiiFullDuplex;
#ifdef EXAMPLE_ENABLE_CACHE_MAINTAIN
    ep_config.rxCacheMaintain = true;
    ep_config.txCacheMaintain = true;
#endif
    
	for (uint8_t ring = 0U; ring < EP_RING_NUM; ring++)
    {
        for (uint8_t index = 0U; index < EP_RXBD_NUM; index++)
        {
            rxBuffAddrArray[ring][index] = (uint64_t)(uintptr_t)&g_rxDataBuff[ring][index];
        }
    }
	if_port.ep_config = &ep_config;
	if_port.bdrConfig = &bdrConfig;
	if_port.g_txFrame = g_txFrame;
	strncpy(soem_port.ifname, SOEM_PORT_NAME, SOEM_IF_NAME_MAXLEN);
	strncpy(soem_port.dev_name, "netc_ep", SOEM_DEV_NAME_MAXLEN);
	soem_port.port_init = netc_ep_init;
	soem_port.port_send = netc_ep_send;
	soem_port.port_recv = netc_ep_recv;
	soem_port.port_close = netc_ep_close;
	soem_port.port_link_status= netc_ep_link_status;
	soem_port.port_pri = &if_port;
    return register_soem_port(&soem_port);
}

uint64_t dc_diff_accu = 0;
int32_t dc_diff_accu_arrary[DC_FILTER_CNT];
int32_t dc_diff_accu_index = 0;

static void dc_diff_init(int32_t dc_diff_ns)
{
	int i;
	for (i = 0; i < DC_FILTER_CNT; i++) {
		dc_diff_accu_arrary[i] = dc_diff_ns;
		dc_diff_accu += dc_diff_ns;
	}
}

static int32_t dc_diff_update(int32_t dc_diff_ns)
{
	dc_diff_accu += dc_diff_ns;
	dc_diff_accu -= dc_diff_accu_arrary[dc_diff_accu_index];
	dc_diff_accu_arrary[dc_diff_accu_index++] = dc_diff_ns;
	if (dc_diff_accu_index >= DC_FILTER_CNT) {
		dc_diff_accu_index = 0;
	}
	return dc_diff_accu / DC_FILTER_CNT;
}

#define PID_P  3 / 2
#define PID_I  3 / 2
#define PID_D  1 / 5 
void update_master_clock()
{
	int32_t dc_diff_ns_avg;
    // calc drift (via un-normalised time diff)
    int32_t delta = dc_diff_ns - prev_dc_diff_ns;
    prev_dc_diff_ns = dc_diff_ns;

    if (dc_started == 2) {
		dc_diff_ns_avg = dc_diff_update(dc_diff_ns);
       dc_adjust_ns = dc_diff_ns * PID_P  + dc_diff_ns_avg * PID_I  + delta * PID_D;
        if (dc_adjust_ns < -10000) {
            dc_adjust_ns = -10000;
        }
        if (dc_adjust_ns > 10000) {
            dc_adjust_ns =  10000;
        }
		// add cycles adjustment to time base (including a spot adjustment)
        system_time_offset += dc_adjust_ns;
    }
    else {
		if (dc_started == 0) {
        	dc_started = (dc_diff_ns != 0);
			system_time_offset = dc_diff_ns;
		} else {
			dc_started = 2;
			dc_diff_init(dc_diff_ns);
			system_time_offset += dc_diff_ns / 2;
		}
    }
}

status_t BOARD_InitHardware(void)
{
    status_t result = kStatus_Success;
	bool link;
    uint32_t st = SCMI_POWER_DOMAIN_STATE_OFF;
    
    /* clang-format off */
    /* busNetcMixClk 133MHz */
    clk_t busmixClk = {
        .clkId = kCLOCK_Busnetcmix,
        .pclkId = kCLOCK_Syspll1dfs1div2, /* 400 MHz */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 133333333UL,
    };
    /* enetClk 666 MHz */
    clk_t enetClk = {
        .clkId = kCLOCK_Enet,
        .pclkId = kCLOCK_Syspll1dfs2, /* 666 MHz */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 666000000UL,
    };
    /* enetRefClk 250MHz */
    clk_t enetrefClk = {
        .clkId = kCLOCK_Enetref,
        .pclkId = kCLOCK_Syspll1dfs0, /* 1 GHz */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 250000000UL,
    };
    /* enetTimer1Clk 100MHz */
    clk_t enettimer1Clk = {
        .clkId = kCLOCK_Enettimer1,
        .pclkId = kCLOCK_Syspll1dfs0div2, /* 500 MHz */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 100000000UL,
    };

    /* mac0Clk(netc_switch_port0) 250MHz */
    clk_t mac0Clk = {
        .clkId = kCLOCK_Mac0,
        .pclkId = kCLOCK_Syspll1dfs0, /* syspll1 dfs0 = 1 GHz */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 250000000UL,
    };

    /* mac1Clk(netc_switch_port1) 250MHz */
    clk_t mac1Clk = {
        .clkId = kCLOCK_Mac1,
        .pclkId = kCLOCK_Syspll1dfs0, /* syspll1 dfs0 = 1 GHz */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 250000000UL,
    };

    /* mac2Clk(netc_switch_port2) 250MHz */
    clk_t mac2Clk = {
        .clkId = kCLOCK_Mac2,
        .pclkId = kCLOCK_Syspll1dfs0, /* syspll1 dfs0 = 1 GHz */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 250000000UL,
    };

    /* mac3Clk(enetc0) 250MHz */
    clk_t mac3Clk = {
        .clkId = kCLOCK_Mac3,
        .pclkId = kCLOCK_Syspll1dfs0, /* syspll1 dfs0 = 1 GHz */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 250000000UL,
    };

    /* mac4Clk(enetc1) 250MHz */
    clk_t mac4Clk = {
        .clkId = kCLOCK_Mac4,
        .pclkId = kCLOCK_Syspll1dfs0, /* syspll1 dfs0 = 1 GHz */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 250000000UL,
    };

    /* mac5Clk(enetc2) 250MHz */
    clk_t mac5Clk = {
        .clkId = kCLOCK_Mac5,
        .pclkId = kCLOCK_Syspll1dfs0, /* syspll1 dfs0 = 1 GHz */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 250000000UL,
    };

    clk_t gptClk = {
        .clkId = kCLOCK_Gpt1,
        .pclkId = kCLOCK_Syspll1dfs1div2, /* 400 MHz */
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 100000000UL,
    };

    pwr_s_t pwrst = {
        .did =  POWER_MIX_SLICE_IDX_NETC,
        .st = SCMI_POWER_DOMAIN_STATE_ON,
    };
    /* clang-format on */

    SystemPlatformInit();
	
	BOARD_InitDebugConsolePins();	
    BOARD_InitNETC_ETH0_Pins();
    BOARD_InitNETC_ETH1_Pins();
    BOARD_InitNETC_ETH2_Pins();
    BOARD_InitNETC_ETH3_Pins();
    BOARD_InitNETC_ETH4_Pins();
    BOARD_InitI2C6Pins();
	
    BOARD_InitI2C3Pins();
	
    BOARD_InitGPIOPins();
	BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    PRINTF("\r\n BOARD_Init success\r\n");

    POWER_SetState(&pwrst);
    st = POWER_GetState(&pwrst);
    assert(st == SCMI_POWER_DOMAIN_STATE_ON);
    (void)st;
    
    rgpio_pin_config_t led_config = {
        kRGPIO_DigitalOutput,
        0,
    };
	RGPIO_PinInit(BOARD_LED_RGPIO, BOARD_LED_RGPIO_PIN, &led_config);
    
	CLOCK_SetParent(&busmixClk);
    CLOCK_SetRate(&busmixClk);
    CLOCK_EnableClock(busmixClk.clkId);

    CLOCK_SetParent(&enetClk);
    CLOCK_SetRate(&enetClk);
    CLOCK_EnableClock(enetClk.clkId);

    CLOCK_SetParent(&enetrefClk);
    CLOCK_SetRate(&enetrefClk);
    CLOCK_EnableClock(enetrefClk.clkId);

    CLOCK_SetParent(&enettimer1Clk);
    CLOCK_SetRate(&enettimer1Clk);
    CLOCK_EnableClock(enettimer1Clk.clkId);

    CLOCK_SetParent(&mac0Clk);
    CLOCK_SetRate(&mac0Clk);
    CLOCK_EnableClock(mac0Clk.clkId);

    CLOCK_SetParent(&mac1Clk);
    CLOCK_SetRate(&mac1Clk);
    CLOCK_EnableClock(mac1Clk.clkId);

    CLOCK_SetParent(&mac2Clk);
    CLOCK_SetRate(&mac2Clk);
    CLOCK_EnableClock(mac2Clk.clkId);

    CLOCK_SetParent(&mac3Clk);
    CLOCK_SetRate(&mac3Clk);
    CLOCK_EnableClock(mac3Clk.clkId);

    CLOCK_SetParent(&mac4Clk);
    CLOCK_SetRate(&mac4Clk);
    CLOCK_EnableClock(mac4Clk.clkId);

    CLOCK_SetParent(&mac5Clk);
    CLOCK_SetRate(&mac5Clk);
    CLOCK_EnableClock(mac5Clk.clkId);

    CLOCK_SetParent(&gptClk);
    CLOCK_SetRate(&gptClk);
    CLOCK_EnableClock(gptClk.clkId);

    /* Select ETH signals to use */
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, ETH2_SEL);
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, ETH3_SEL);
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C6_S3_ID, ETH4_SEL);

    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C6_S3_ID, ETH2_SEL);
    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C6_S3_ID, ETH3_SEL);
    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C6_S3_ID, ETH4_SEL);

    /* PHY reset */
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C3_S5_21_ID, ETH2_RST_B);
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C3_S5_21_ID, ETH3_RST_B);
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C3_S5_21_ID, ETH4_RST_B);

    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C3_S5_21_ID, ETH2_RST_B);
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C3_S5_21_ID, ETH2_RST_B);
    SDK_DelayAtLeastUs(100000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C3_S5_21_ID, ETH3_RST_B);
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C3_S5_21_ID, ETH3_RST_B);
    SDK_DelayAtLeastUs(100000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C3_S5_21_ID, ETH4_RST_B);
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C3_S5_21_ID, ETH4_RST_B);
    SDK_DelayAtLeastUs(100000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

	 /*
     * PCS(Physical Coding Sublayer) protocols on link0-5,
     * xxxx xxxx xxxx xxx1: 1G SGMII
     * xxxx xxxx xxxx xx1x: OC-SGMII(i.e.: OverClock 2.5 G SGMII)
     */
    BLK_CTRL_NETCMIX->CFG_LINK_PCS_PROT_0 |= BLK_CTRL_NETCMIX_CFG_LINK_PCS_PROT_0_CFG_LINK_PCS_PROT_0(2U); /* OverClock 2.5 G SGMII */
    BLK_CTRL_NETCMIX->CFG_LINK_PCS_PROT_1 |= BLK_CTRL_NETCMIX_CFG_LINK_PCS_PROT_1_CFG_LINK_PCS_PROT_1(2U); /* OverClock 2.5 G SGMII */
    BLK_CTRL_NETCMIX->CFG_LINK_PCS_PROT_2 |= BLK_CTRL_NETCMIX_CFG_LINK_PCS_PROT_2_CFG_LINK_PCS_PROT_2(1U); /* 1G SGMII */
    BLK_CTRL_NETCMIX->CFG_LINK_PCS_PROT_3 |= BLK_CTRL_NETCMIX_CFG_LINK_PCS_PROT_3_CFG_LINK_PCS_PROT_3(1U); /* 1G SGMII */
    BLK_CTRL_NETCMIX->CFG_LINK_PCS_PROT_4 |= BLK_CTRL_NETCMIX_CFG_LINK_PCS_PROT_4_CFG_LINK_PCS_PROT_4(1U); /* 1G SGMII */
    BLK_CTRL_NETCMIX->CFG_LINK_PCS_PROT_5 |= BLK_CTRL_NETCMIX_CFG_LINK_PCS_PROT_5_CFG_LINK_PCS_PROT_5(1U); /* 1G SGMII */

    /*
     * MII protocol for port0~5
     * 0b0000..MII
     * 0b0001..RMII
     * 0b0010..RGMII
     * 0b0011..SGMII
     * 0b0100~0b1111..Reserved
     */
    BLK_CTRL_NETCMIX->NETC_LINK_CFG0 |= BLK_CTRL_NETCMIX_NETC_LINK_CFG0_MII_PROT(0x3U); /* SGMII */
    BLK_CTRL_NETCMIX->NETC_LINK_CFG1 |= BLK_CTRL_NETCMIX_NETC_LINK_CFG1_MII_PROT(0x3U); /* SGMII */
    BLK_CTRL_NETCMIX->NETC_LINK_CFG2 |= BLK_CTRL_NETCMIX_NETC_LINK_CFG2_MII_PROT(0x2U); /* RGMII */
    BLK_CTRL_NETCMIX->NETC_LINK_CFG3 |= BLK_CTRL_NETCMIX_NETC_LINK_CFG3_MII_PROT(0x2U); /* RGMII */
    BLK_CTRL_NETCMIX->NETC_LINK_CFG4 |= BLK_CTRL_NETCMIX_NETC_LINK_CFG4_MII_PROT(0x2U); /* RGMII */
    BLK_CTRL_NETCMIX->NETC_LINK_CFG5 |= BLK_CTRL_NETCMIX_NETC_LINK_CFG5_MII_PROT(0x2U); /* RGMII */

    /*
     * ETH2 selection: MAC2(switch port2) or MAC3(enetc1)
     * 0b - MAC2 selected
     * 1b - MAC3 selected
     */
    BLK_CTRL_NETCMIX->EXT_PIN_CONTROL &= ~BLK_CTRL_NETCMIX_EXT_PIN_CONTROL_mac2_mac3_sel(1U);

    /* Unlock the IERB. It will warm reset whole NETC. */
    NETC_PRIV->NETCRR &= ~NETC_PRIV_NETCRR_LOCK_MASK;
    while ((NETC_PRIV->NETCRR & NETC_PRIV_NETCRR_LOCK_MASK) != 0U)
    {
    }

    /* Lock the IERB. */
    NETC_PRIV->NETCRR |= NETC_PRIV_NETCRR_LOCK_MASK;
    while ((NETC_PRIV->NETCSR & NETC_PRIV_NETCSR_STATE_MASK) != 0U)
    {
    }
    
    result = NETC_EP_MDIO_Init();
    if (result != kStatus_Success)
    {
        PRINTF("\r\nMDIO Init failed!\r\n");
        return result;
    }

    result = NETC_EP_PHY_Init();
    if (result != kStatus_Success)
    {
        PRINTF("\r\n PHY Init failed!\r\n");
        return result;
    }

    PRINTF("\r\n Wait PHY get link status \r\n");

    do
    {
		NETC_EP_PHY_GetLinkStatus(&if_port.phy_handle, &link);
    } while (!link);

    PRINTF("\r\n PHY get link status success\r\n");
    /* Wait a moment for PHY status to be stable. */
    SDK_DelayAtLeastUs(PHY_STABILITY_DELAY_US, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    return result;
}

status_t NETC_EP_MDIO_Init(void)
{
    status_t result = kStatus_Success;
    netc_mdio_config_t mdioConfig = {
        .isPreambleDisable = false,
        .isNegativeDriven  = false,
        .srcClockHz        = CLOCK_GetRate(kCLOCK_Enet),
    };

    mdioConfig.mdio.type = kNETC_EMdio;
    result               = NETC_MDIOInit(&if_port.s_emdio_handle, &mdioConfig);
    if (result != kStatus_Success)
    {
        return result;
    }
    
    return result;
}

static status_t NETC_EP_EMDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return NETC_MDIOWrite(&if_port.s_emdio_handle, phyAddr, regAddr, data);
}

static status_t NETC_EP_EMDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return NETC_MDIORead(&if_port.s_emdio_handle, phyAddr, regAddr, pData);
}

phy_rtl8211f_resource_t s_phy_resource;
status_t NETC_EP_PHY_Init(void)
{
    status_t result            = kStatus_Success;
    phy_config_t phy8211Config = {
        .autoNeg   = false,
        .speed     = kPHY_Speed100M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phyrtl8211f_ops,
    };

    /* Initialize PHY for EP. */
    s_phy_resource.write = NETC_EP_EMDIOWrite;
    s_phy_resource.read  = NETC_EP_EMDIORead;
    
	phy8211Config.resource = &s_phy_resource;
    phy8211Config.phyAddr  = EP0_PHY_ADDR;
    result = PHY_Init(&if_port.phy_handle, &phy8211Config);
    if (result != kStatus_Success)
    {
        return result;
    }

    return result;
}

status_t NETC_EP_PHY_GetLinkStatus(phy_handle_t *handler, bool *link)
{
	return PHY_GetLinkStatus(handler, link);
}

