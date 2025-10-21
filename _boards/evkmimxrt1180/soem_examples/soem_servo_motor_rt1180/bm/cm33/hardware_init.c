/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"

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
        if_port.txOver = true;
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

status_t NETC_EP_MDIO_Init(void)
{
    status_t result = kStatus_Success;

    netc_mdio_config_t mdioConfig = {
        .isPreambleDisable = false,
        .isNegativeDriven  = false,
        .srcClockHz        = NETC_FREQ,
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

static status_t NETC_EP_Phy8201SetUp(phy_handle_t *handle)
{
    status_t result;
    uint16_t data;

    result = PHY_Write(handle, PHY_PAGE_SELECT_REG, 7);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = PHY_Read(handle, 16, &data);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* CRS/DV pin is RXDV signal. */
    data |= (1U << 2);
    result = PHY_Write(handle, 16, data);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = PHY_Write(handle, PHY_PAGE_SELECT_REG, 0);

    return result;
}

phy_rtl8201_resource_t s_phy_resource;
status_t NETC_EP_PHY_Init(void)
{
    status_t result            = kStatus_Success;
    phy_config_t phy8201Config = {
        .autoNeg   = false,
        .speed     = kPHY_Speed100M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phyrtl8201_ops,
    };
    rgpio_pin_config_t pinConfig = {.pinDirection = kRGPIO_DigitalOutput, .outputLogic = 0};

    /* Reset PHY8201 for ETH4(EP), ETH0(Switch port0). Power on 150ms, reset 10ms, wait 150ms. */
    /* Reset PHY8211 for ETH1(Switch port1). Reset 10ms, wait 72ms. */
    RGPIO_PinInit(RGPIO4, 13, &pinConfig);
    RGPIO_PinInit(RGPIO4, 25, &pinConfig);
    SDK_DelayAtLeastUs(10000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    RGPIO_PinWrite(RGPIO4, 13, 1);
    RGPIO_PinWrite(RGPIO4, 25, 1);
    SDK_DelayAtLeastUs(150000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* Initialize PHY for EP. */
    s_phy_resource.write = NETC_EP_EMDIOWrite;
    s_phy_resource.read  = NETC_EP_EMDIORead;
    
	phy8201Config.resource = &s_phy_resource;
    phy8201Config.phyAddr  = EP0_PHY_ADDR;
    result                 = PHY_Init(&if_port.phy_handle, &phy8201Config);
    if (result != kStatus_Success)
    {
        return result;
    }

    result = NETC_EP_Phy8201SetUp(&if_port.phy_handle);
    if (result != kStatus_Success)
    {
        return result;
    }

    return result;
}

status_t BOARD_InitHardware(void)
{
    bool link;
    status_t result = kStatus_Success;
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    
    /* RMII mode */
    BLK_CTRL_WAKEUPMIX->NETC_LINK_CFG[0] = BLK_CTRL_WAKEUPMIX_NETC_LINK_CFG_MII_PROT(1);
    BLK_CTRL_WAKEUPMIX->NETC_LINK_CFG[4] = BLK_CTRL_WAKEUPMIX_NETC_LINK_CFG_MII_PROT(1);

    /* RGMII mode */
    BLK_CTRL_WAKEUPMIX->NETC_LINK_CFG[1] = BLK_CTRL_WAKEUPMIX_NETC_LINK_CFG_MII_PROT(2);

    /* Output reference clock for RMII */
    BLK_CTRL_WAKEUPMIX->NETC_PORT_MISC_CFG |= BLK_CTRL_WAKEUPMIX_NETC_PORT_MISC_CFG_PORT0_RMII_REF_CLK_DIR_MASK |
                                              BLK_CTRL_WAKEUPMIX_NETC_PORT_MISC_CFG_PORT4_RMII_REF_CLK_DIR_MASK;

    /* Unlock the IERB. It will warm reset whole NETC. */
    NETC_PRIV->NETCRR &= ~NETC_PRIV_NETCRR_LOCK_MASK;
    while ((NETC_PRIV->NETCRR & NETC_PRIV_NETCRR_LOCK_MASK) != 0U)
    {
    }

    /* Set the access attribute, otherwise MSIX access will be blocked. */
    NETC_IERB->ARRAY_NUM_RC[0].RCMSIAMQR &= ~(7U << 27);
    NETC_IERB->ARRAY_NUM_RC[0].RCMSIAMQR |= (1U << 27);

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
        PRINTF("\r\nPHY Init failed!\r\n");
        return result;
    }

    do
    {
		PHY_GetLinkStatus(&if_port.phy_handle, &link);
    } while (!link);

    return result;
}
