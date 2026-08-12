/*
 * Copyright 2022-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_iomuxc.h"
#include "fsl_enet.h"
/*${header:end}*/

/*${variable:start}*/
/* dc sync time */
uint8_t  dc_started = 0;
int32_t  dc_diff_ns = 0;
int32_t  prev_dc_diff_ns = 0;
int64_t  dc_adjust_ns;
int64_t  system_time_offset = 0LL;

phy_rtl8211f_resource_t g_phy_resource;
double rate_counter_ns = 0;

volatile uint64_t system_time_ns = 0;

/*! @brief Buffer descriptors should be in non-cacheable region and should be align to "ENET_BUFF_ALIGNMENT". */
AT_NONCACHEABLE_SECTION_ALIGN(enet_rx_bd_struct_t g_rxBuffDescrip[ENET_RXBD_NUM], ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(enet_tx_bd_struct_t g_txBuffDescrip[ENET_TXBD_NUM], ENET_BUFF_ALIGNMENT);
/*! @brief The data buffers can be in cacheable region or in non-cacheable region.
 * If use cacheable region, the alignment size should be the maximum size of "CACHE LINE SIZE" and "ENET_BUFF_ALIGNMENT"
 * If use non-cache region, the alignment size is the "ENET_BUFF_ALIGNMENT".
 */

AT_NONCACHEABLE_SECTION_ALIGN(uint8_t g_rxDataBuff[ENET_RXBD_NUM][SDK_SIZEALIGN(ENET_RXBUFF_SIZE, ENET_BUFF_ALIGNMENT)],
          ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t g_txDataBuff[ENET_TXBD_NUM][SDK_SIZEALIGN(ENET_TXBUFF_SIZE, ENET_BUFF_ALIGNMENT)],
          ENET_BUFF_ALIGNMENT);

enet_buffer_config_t buffConfig[] = {{
    ENET_RXBD_NUM,
    ENET_TXBD_NUM,
    SDK_SIZEALIGN(ENET_RXBUFF_SIZE, ENET_BUFF_ALIGNMENT),
    SDK_SIZEALIGN(ENET_TXBUFF_SIZE, ENET_BUFF_ALIGNMENT),
    &g_rxBuffDescrip[0],
    &g_txBuffDescrip[0],
    &g_rxDataBuff[0][0],
    &g_txDataBuff[0][0],
    true,
    true,
    NULL,
}};
/*${variable:end}*/

/*${function:start}*/
static void MDIO_Init(void)
{
    (void)CLOCK_EnableClock(s_enetClock[ENET_GetInstance(EXAMPLE_ENET)]);
    ENET_SetSMI(EXAMPLE_ENET, EXAMPLE_CLOCK_FREQ, false);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(EXAMPLE_ENET, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(EXAMPLE_ENET, phyAddr, regAddr, pData);
}

uint64_t system_time64_ns()
{
	uint64_t nsec_base;
	uint32_t cur_nsec;

	nsec_base  = system_time_ns;
	cur_nsec   = TPM_GetCurrentTimerCount(BOARD_TPM);

	if (nsec_base != system_time_ns)
	{
		nsec_base  = system_time_ns;
		cur_nsec   = TPM_GetCurrentTimerCount(BOARD_TPM);
	}
	return nsec_base + cur_nsec * rate_counter_ns - system_time_offset;
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

void irq_wake_task(void)
{
    return;
}

void OSAL_TIMER_IRQHandler(void)
{
	/* Clear interrupt flag. */
	TPM_ClearStatusFlags(BOARD_TPM, kTPM_TimeOverflowFlag);

	system_time_ns += CLOCK_INCREASE_PER_SEC;

	irq_wake_task();
	/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F, Cortex-M7, Cortex-M7F Store immediate overlapping
 	 exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U || __CORTEX_M == 7U)
	__DSB();
#endif
}

void BOARD_TPM_HANDLER(void)
{
    /* Clear interrupt flag.*/
    TPM_ClearStatusFlags(BOARD_TPM, kTPM_TimeOverflowFlag);
	system_time_ns += CLOCK_INCREASE_PER_SEC;

    SDK_ISR_EXIT_BARRIER;
}

void osal_timer_init(uint32_t priority)
{
	tpm_config_t tpmInfo;
	TPM_GetDefaultConfig(&tpmInfo);

	/* TPM clock divide by TPM_PRESCALER */
    tpmInfo.prescale = TPM_PRESCALER;

	/* Initialize TPM module */
    TPM_Init(BOARD_TPM, &tpmInfo);

	rate_counter_ns = 1000000000.0 / (TPM_SOURCE_CLOCK / (1U << tpmInfo.prescale));

	/* Set timer period */
    TPM_SetTimerPeriod(BOARD_TPM, USEC_TO_COUNT(DEMO_TIMER_PERIOD_US, TPM_SOURCE_CLOCK / (1U << tpmInfo.prescale)));

    TPM_EnableInterrupts(BOARD_TPM, kTPM_TimeOverflowInterruptEnable);

	/* Enable at the Interrupt */
	EnableIRQ(BOARD_TPM_IRQ_NUM);
	TPM_StartTimer(BOARD_TPM, kTPM_SystemClock);

	system_time_ns = TPM_GetCurrentTimerCount(BOARD_TPM);
}

struct enet_if_port if_port;

int if_port_init()
{
	struct soem_if_port soem_port;
	memset(&if_port, 0, sizeof(if_port));
	if_port.bufferConfig = buffConfig;
	if_port.base = EXAMPLE_ENET;
/* The miiMode should be set according to the different PHY interfaces. */
#ifdef EXAMPLE_PHY_INTERFACE_RGMII
	if_port.mii_mode = kENET_RgmiiMode;
#else
	if_port.mii_mode = kENET_RmiiMode;
#endif
	g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;

    if_port.phy_config.autoNeg = true;
    if_port.phy_config.phyAddr = EXAMPLE_PHY_ADDRESS;
	if_port.phy_config.ops = &phyrtl8211f_ops;
	if_port.phy_config.resource = &g_phy_resource;

    if_port.srcClock_Hz = EXAMPLE_CLOCK_FREQ;
    if_port.phy_autonego_timeout_count = PHY_AUTONEGO_TIMEOUT_COUNT;
    if_port.phy_stability_delay_us = PHY_STABILITY_DELAY_US;

    soem_port.port_init = enet_init;
    soem_port.port_send = enet_send;
    soem_port.port_recv = enet_recv;
    soem_port.port_link_status = enet_link_status;
    soem_port.port_close = enet_close;
    strncpy(soem_port.ifname, SOEM_PORT_NAME, SOEM_IF_NAME_MAXLEN);
    strncpy(soem_port.dev_name, "enet", SOEM_DEV_NAME_MAXLEN);
    soem_port.port_pri = &if_port;
    return register_soem_port(&soem_port);
}

void BOARD_InitHardware()
{
    /* Hardware Initialization. */
    /* clang-format off */
    /* enetClk 250MHz */
    const clock_root_config_t enetClkCfg = {
        .clockOff = false,
	.mux = kCLOCK_WAKEUPAXI_ClockRoot_MuxSysPll1Pfd0, // 1000MHz
	.div = 4
    };

    /* enetRefClk 250MHz (For 125MHz TX_CLK ) */
    const clock_root_config_t enetRefClkCfg = {
        .clockOff = false,
	.mux = kCLOCK_ENETREF_ClockRoot_MuxSysPll1Pfd0Div2, // 500MHz
	.div = 2
    };

    const clock_root_config_t lpi2cClkCfg = {
        .clockOff = false,
	.mux = 0, // 24MHz oscillator source
	.div = 1
    };
    /* clang-format on */    

	const clock_root_config_t tpmClkCfg = {
        .clockOff = false,
        .mux = 0, /* 24MHz oscillator source */
        .div = 1
    };

    /* Hardware Initialization. */
	BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRootClock(kCLOCK_Root_WakeupAxi, &enetClkCfg);
    CLOCK_SetRootClock(kCLOCK_Root_EnetRef, &enetRefClkCfg);
    CLOCK_EnableClock(kCLOCK_Enet1);
    CLOCK_SetRootClock(BOARD_PCAL6524_I2C_CLOCK_ROOT, &lpi2cClkCfg);
    CLOCK_EnableClock(kCLOCK_Lpi2c2);
	CLOCK_SetRootClock(LPTPM_CLOCK_ROOT, &tpmClkCfg);
    CLOCK_EnableClock(LPTPM_CLOCK_GATE);

    /* For a complete PHY reset of RTL8211FDI-CG, this pin must be asserted low for at least 10ms. And
     * wait for a further 30ms(for internal circuits settling time) before accessing the PHY register */
    pcal6524_handle_t handle;
    BOARD_InitPCAL6524(&handle);
    PCAL6524_SetDirection(&handle, (1 << BOARD_PCAL6524_ENET2_NRST), kPCAL6524_Output);
    PCAL6524_ClearPins(&handle, (1 << BOARD_PCAL6524_ENET2_NRST));
    SDK_DelayAtLeastUs(10000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    PCAL6524_SetPins(&handle, (1 << BOARD_PCAL6524_ENET2_NRST));
	SDK_DelayAtLeastUs(30000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

	EnableIRQ(ENET_MAC0_Rx_Tx_Done1_IRQn);
    EnableIRQ(ENET_MAC0_Rx_Tx_Done2_IRQn);
    EnableIRQ(ENET_IRQn);

    MDIO_Init();
}

/*${function:end}*/
