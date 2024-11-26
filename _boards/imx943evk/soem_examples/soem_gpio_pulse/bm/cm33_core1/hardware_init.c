/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "sm_platform.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "hal_clock.h"
#include "hal_power.h"
/*${header:end}*/

/*${variable:start}*/
/* PHY operation. */
#ifdef EXAMPLE_PHY_USE_PORT_MDIO
static netc_mdio_handle_t s_emdio_handle[5];
#else
static netc_mdio_handle_t s_emdio_handle;
#endif
/*${variable:end}*/


void BOARD_InitHardware(void)
{
    status_t result = kStatus_Success;
	bool link;
    hal_pwr_st_e st = hal_power_state_off;
    
    /* clang-format off */
    /* busNetcMixClk 133MHz */
    hal_clk_t hal_busmixClk = {
        .clk_id = hal_clock_busnetcmix,
        .pclk_id = hal_clock_syspll1dfs1div2, /* 400 MHz */
        .clk_round_opt = hal_clk_round_auto,
        .rate = 133333333UL,
    };
    /* enetClk 666 MHz */
    hal_clk_t hal_enetClk = {
        .clk_id = hal_clock_enet,
        .pclk_id = hal_clock_syspll1dfs2, /* 666 MHz */
        .clk_round_opt = hal_clk_round_auto,
        .rate = 666000000UL,
    };
    /* enetRefClk 250MHz */
    hal_clk_t hal_enetrefClk = {
        .clk_id = hal_clock_enetref,
        .pclk_id = hal_clock_syspll1dfs0, /* 1 GHz */
        .clk_round_opt = hal_clk_round_auto,
        .rate = 250000000UL,
    };
    /* enetTimer1Clk 100MHz */
    hal_clk_t hal_enettimer1Clk = {
        .clk_id = hal_clock_enettimer1,
        .pclk_id = hal_clock_syspll1dfs0div2, /* 500 MHz */
        .clk_round_opt = hal_clk_round_auto,
        .rate = 100000000UL,
    };

    /* mac3Clk(enetc0) 250MHz */
    hal_clk_t hal_mac3Clk = {
        .clk_id = hal_clock_mac3,
        .pclk_id = hal_clock_syspll1dfs0, /* syspll1 dfs0 = 1 GHz */
        .clk_round_opt = hal_clk_round_auto,
        .rate = 250000000UL,
    };

    /* mac4Clk(enetc1) 250MHz */
    hal_clk_t hal_mac4Clk = {
        .clk_id = hal_clock_mac4,
        .pclk_id = hal_clock_syspll1dfs0, /* syspll1 dfs0 = 1 GHz */
        .clk_round_opt = hal_clk_round_auto,
        .rate = 250000000UL,
    };

    /* mac5Clk(enetc2) 250MHz */
    hal_clk_t hal_mac5Clk = {
        .clk_id = hal_clock_mac5,
        .pclk_id = hal_clock_syspll1dfs0, /* syspll1 dfs0 = 1 GHz */
        .clk_round_opt = hal_clk_round_auto,
        .rate = 250000000UL,
    };

    hal_pwr_s_t pwrst = {
        .did = HAL_POWER_PLATFORM_MIX_SLICE_IDX_NETC,
        .st = hal_power_state_on,
    };
    /* clang-format on */

    SM_Platform_Init();
    
	HAL_PowerSetState(&pwrst);
    st = HAL_PowerGetState(&pwrst);
    assert(st == hal_power_state_on);
	
	BOARD_InitDebugConsolePins();	
	BOARD_ConfigMPU();
    BOARD_InitBootPins();
	BOARD_BootClockRUN();	
    BOARD_InitDebugConsole();
    
    rgpio_pin_config_t led_config = {
        kRGPIO_DigitalOutput,
        0,
    };
	RGPIO_PinInit(BOARD_LED_RGPIO, BOARD_LED_RGPIO_PIN, &led_config);
    
	HAL_ClockSetParent(&hal_busmixClk);
    HAL_ClockSetRate(&hal_busmixClk);
    HAL_ClockEnable(&hal_busmixClk);
    HAL_ClockSetParent(&hal_enetClk);
    HAL_ClockSetRate(&hal_enetClk);
    HAL_ClockEnable(&hal_enetClk);
    HAL_ClockSetParent(&hal_enetrefClk);
    HAL_ClockSetRate(&hal_enetrefClk);
    HAL_ClockEnable(&hal_enetrefClk);
    HAL_ClockSetParent(&hal_enettimer1Clk);
    HAL_ClockSetRate(&hal_enettimer1Clk);
    HAL_ClockEnable(&hal_enettimer1Clk);
    HAL_ClockSetParent(&hal_mac3Clk);
    HAL_ClockSetRate(&hal_mac3Clk);
    HAL_ClockEnable(&hal_mac3Clk);
    HAL_ClockSetParent(&hal_mac4Clk);
    HAL_ClockSetRate(&hal_mac4Clk);
    HAL_ClockEnable(&hal_mac4Clk);
    HAL_ClockSetParent(&hal_mac5Clk);
    HAL_ClockSetRate(&hal_mac5Clk);
    HAL_ClockEnable(&hal_mac5Clk);

	BLK_CTRL_NETCMIX->CFG_LINK_PCS_PROT_0 |= BLK_CTRL_NETCMIX_CFG_LINK_PCS_PROT_0_CFG_LINK_PCS_PROT_0(2U); /* OverClock 2.5 G SGMII */
	BLK_CTRL_NETCMIX->CFG_LINK_PCS_PROT_1 |= BLK_CTRL_NETCMIX_CFG_LINK_PCS_PROT_1_CFG_LINK_PCS_PROT_1(2U); /* OverClock 2.5 G SGMII */
	BLK_CTRL_NETCMIX->CFG_LINK_PCS_PROT_2 |= BLK_CTRL_NETCMIX_CFG_LINK_PCS_PROT_2_CFG_LINK_PCS_PROT_2(1U); /* 1G SGMII */
	BLK_CTRL_NETCMIX->CFG_LINK_PCS_PROT_3 |= BLK_CTRL_NETCMIX_CFG_LINK_PCS_PROT_3_CFG_LINK_PCS_PROT_3(1U); /* 1G SGMII */
	BLK_CTRL_NETCMIX->CFG_LINK_PCS_PROT_4 |= BLK_CTRL_NETCMIX_CFG_LINK_PCS_PROT_4_CFG_LINK_PCS_PROT_4(1U); /* 1G SGMII */ 
	BLK_CTRL_NETCMIX->CFG_LINK_PCS_PROT_5 |= BLK_CTRL_NETCMIX_CFG_LINK_PCS_PROT_5_CFG_LINK_PCS_PROT_5(1U); /* 1G SGMII */
	
	BLK_CTRL_NETCMIX->NETC_LINK_CFG0 |= BLK_CTRL_NETCMIX_NETC_LINK_CFG0_MII_PROT(0x3U); /* SGMII */
	BLK_CTRL_NETCMIX->NETC_LINK_CFG1 |= BLK_CTRL_NETCMIX_NETC_LINK_CFG1_MII_PROT(0x3U); /* SGMII */
	BLK_CTRL_NETCMIX->NETC_LINK_CFG2 |= BLK_CTRL_NETCMIX_NETC_LINK_CFG2_MII_PROT(0x2U); /* RGMII */
	BLK_CTRL_NETCMIX->NETC_LINK_CFG3 |= BLK_CTRL_NETCMIX_NETC_LINK_CFG3_MII_PROT(0x1U); /* RGMII */
	BLK_CTRL_NETCMIX->NETC_LINK_CFG4 |= BLK_CTRL_NETCMIX_NETC_LINK_CFG4_MII_PROT(0x1U); /* RGMII */
	BLK_CTRL_NETCMIX->NETC_LINK_CFG5 |= BLK_CTRL_NETCMIX_NETC_LINK_CFG5_MII_PROT(0x1U); /* RGMII */

#if NETC_ETH2_SEL_MAC == NETC_ETH2_ROUTE_TO_MAC3
    BLK_CTRL_NETCMIX->EXT_PIN_CONTROL |= BLK_CTRL_NETCMIX_EXT_PIN_CONTROL_mac2_mac3_sel(1U);
#elif NETC_ETH2_SEL_MAC == NETC_ETH2_ROUTE_TO_MAC2
    BLK_CTRL_NETCMIX->EXT_PIN_CONTROL &= ~BLK_CTRL_NETCMIX_EXT_PIN_CONTROL_mac2_mac3_sel(1U);
#else
#error "Pls define macro NETC_ETH2_SEL_MAC!"
#endif

    /* Unlock the IERB. It will warm reset whole NETC. */
    NETC_PRIV->NETCRR &= ~NETC_PRIV_NETCRR_LOCK_MASK;
    while ((NETC_PRIV->NETCRR & NETC_PRIV_NETCRR_LOCK_MASK) != 0U)
    {
    }

#ifdef EXAMPLE_PHY_USE_PORT_MDIO
    /* Set PHY address in IERB to use MAC port MDIO, otherwise the access will be blocked. */
    NETC_IERB->L4BCR = NETC_IERB_L0BCR_MDIO_PHYAD_PRTAD(EXAMPLE_EP0_PHY_ADDR);
#endif

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
		NETC_EP_PHY_GetLinkStatus(&if_port.phy_handle, &link);
    } while (!link);

}

status_t NETC_EP_MDIO_Init(void)
{
    status_t result = kStatus_Success;
    netc_mdio_config_t mdioConfig = {
        .isPreambleDisable = false,
        .isNegativeDriven  = false,
        .srcClockHz        = HAL_ClockGetIpFreq(hal_clock_enet),
    };
#ifdef EXAMPLE_PHY_USE_PORT_MDIO
	if_port.s_emdio_handle = &s_emdio_handle[0];
    /* Usually should call EP_Init/SWT_Init then init port MDIO, here just an quick enablement example. */
    NETC_F2_PCI_HDR_TYPE0->PCI_CFH_CMD |=
        (ENETC_PCI_TYPE0_PCI_CFH_CMD_MEM_ACCESS_MASK | ENETC_PCI_TYPE0_PCI_CFH_CMD_BUS_MASTER_EN_MASK);
    NETC_F3_PCI_HDR_TYPE0->PCI_CFH_CMD |=
        (ENETC_PCI_TYPE0_PCI_CFH_CMD_MEM_ACCESS_MASK | ENETC_PCI_TYPE0_PCI_CFH_CMD_BUS_MASTER_EN_MASK);

    for (int i = 0U; i < 5U; i++)
    {
        mdioConfig.mdio.port = (netc_hw_eth_port_idx_t)((uint32_t)kNETC_ENETC0EthPort + i);
        result               = NETC_MDIOInit(&if_port.s_emdio_handle[i], &mdioConfig);
        if (result != kStatus_Success)
        {
            return result;
        }
    }
#else
    mdioConfig.mdio.type = kNETC_EMdio;
    result               = NETC_MDIOInit(&if_port.s_emdio_handle, &mdioConfig);
    if (result != kStatus_Success)
    {
        return result;
    }
#endif
    return result;
}

#ifdef EXAMPLE_PHY_USE_PORT_MDIO
static status_t NETC_EP_EMDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    status_t result = kStatus_Success;
    netc_mdio_handle_t *mdioHandle;

    switch (phyAddr)
    {
        case BOARD_EP0_PHY_ADDR:
            mdioHandle = &if_port.s_emdio_handle[0];
            break;
        default:
            result = kStatus_InvalidArgument;
            break;
    }

    if (result != kStatus_Success)
    {
        return result;
    }

    return NETC_MDIOWrite(mdioHandle, phyAddr, regAddr, data);



}

static status_t NETC_EP_EMDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
	status_t result = kStatus_Success;
    netc_mdio_handle_t *mdioHandle;

    switch (phyAddr)
    {
        case BOARD_EP0_PHY_ADDR:
            mdioHandle = &if_port.s_emdio_handle[0];
            break;
        default:
            result = kStatus_InvalidArgument;
            break;
    }

    if (result != kStatus_Success)
    {
        return result;
    }

    return NETC_MDIORead(mdioHandle, phyAddr, regAddr, pData);

}
#else
static status_t NETC_EP_EMDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return NETC_MDIOWrite(&if_port.s_emdio_handle, phyAddr, regAddr, data);
}

static status_t NETC_EP_EMDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return NETC_MDIORead(&if_port.s_emdio_handle, phyAddr, regAddr, pData);
}
#endif

#if BOARD_IMX943_TYPE == BOARD_IMX943_EMULATOR
static status_t NETC_EP_Phy8201SetUp(phy_handle_t *handle)
{
    return kStatus_Success;
}

status_t NETC_EP_PHY_Init(void)
{
	return kStatus_Success;
}

status_t NETC_EP_PHY_GetLinkStatus(phy_handle_t *handler, bool *link)
{
	*link = true;
	return true;
}
#else
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

status_t NETC_EP_PHY_GetLinkStatus(phy_handle_t *handler, bool *link)
{
	return PHY_GetLinkStatus(handler, link);
}
#endif

