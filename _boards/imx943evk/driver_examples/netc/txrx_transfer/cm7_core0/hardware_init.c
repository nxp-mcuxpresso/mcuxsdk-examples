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
#include "fsl_irqsteer.h"
/*${header:end}*/

/*${macro:start}*/
#define PHY_PAGE_SELECT_REG 0x1FU /*!< The PHY page select register. */
/* Select MAC2 or MAC3 */
#define NETC_ETH2_ROUTE_TO_MAC2 0
#define NETC_ETH2_ROUTE_TO_MAC3 1
#define NETC_ETH2_SEL_MAC NETC_ETH2_ROUTE_TO_MAC2
/*${macro:end}*/

/*${variable:start}*/
/* PHY operation. */
#ifdef EXAMPLE_PHY_USE_PORT_MDIO
static netc_mdio_handle_t s_mdio_handle[5];
#else
static netc_mdio_handle_t s_mdio_handle[EXAMPLE_PORT_NUM];
#endif
static phy_rtl8211f_resource_t s_phy_resource[5];
static phy_handle_t s_phy_handle[5];
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
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

    /* mac0Clk(netc_switch_port0) 250MHz */
    hal_clk_t hal_mac0Clk = {
        .clk_id = hal_clock_mac0,
        .pclk_id = hal_clock_syspll1dfs0, /* syspll1 dfs0 = 1 GHz */
        .clk_round_opt = hal_clk_round_auto,
        .rate = 250000000UL,
    };

    /* mac1Clk(netc_switch_port1) 250MHz */
    hal_clk_t hal_mac1Clk = {
        .clk_id = hal_clock_mac1,
        .pclk_id = hal_clock_syspll1dfs0, /* syspll1 dfs0 = 1 GHz */
        .clk_round_opt = hal_clk_round_auto,
        .rate = 250000000UL,
    };

    /* mac2Clk(netc_switch_port2) 250MHz */
    hal_clk_t hal_mac2Clk = {
        .clk_id = hal_clock_mac2,
        .pclk_id = hal_clock_syspll1dfs0, /* syspll1 dfs0 = 1 GHz */
        .clk_round_opt = hal_clk_round_auto,
        .rate = 250000000UL,
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
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

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
    HAL_ClockSetParent(&hal_mac0Clk);
    HAL_ClockSetRate(&hal_mac0Clk);
    HAL_ClockEnable(&hal_mac0Clk);
    HAL_ClockSetParent(&hal_mac1Clk);
    HAL_ClockSetRate(&hal_mac1Clk);
    HAL_ClockEnable(&hal_mac1Clk);
    HAL_ClockSetParent(&hal_mac2Clk);
    HAL_ClockSetRate(&hal_mac2Clk);
    HAL_ClockEnable(&hal_mac2Clk);
    HAL_ClockSetParent(&hal_mac3Clk);
    HAL_ClockSetRate(&hal_mac3Clk);
    HAL_ClockEnable(&hal_mac3Clk);
    HAL_ClockSetParent(&hal_mac4Clk);
    HAL_ClockSetRate(&hal_mac4Clk);
    HAL_ClockEnable(&hal_mac4Clk);
    HAL_ClockSetParent(&hal_mac5Clk);
    HAL_ClockSetRate(&hal_mac5Clk);
    HAL_ClockEnable(&hal_mac5Clk);

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
     * Selection for TSN MAC2 or MAC3 port
     * 0b - MAC2 selected
     * 1b - MAC3 selected
     * enetc1 <-> MAC3
     */
#if NETC_ETH2_SEL_MAC == NETC_ETH2_ROUTE_TO_MAC3
    BLK_CTRL_NETCMIX->EXT_PIN_CONTROL |= BLK_CTRL_NETCMIX_EXT_PIN_CONTROL_mac2_mac3_sel(1U);
#elif NETC_ETH2_SEL_MAC == NETC_ETH2_ROUTE_TO_MAC2
    BLK_CTRL_NETCMIX->EXT_PIN_CONTROL &= ~BLK_CTRL_NETCMIX_EXT_PIN_CONTROL_mac2_mac3_sel(1U);
#else
#error "Pls define macro NETC_ETH2_SEL_MAC!"
#endif
    //BLK_CTRL_NETCMIX->CFG_LINK_MII_PROT = 0x00000222;

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

    IRQSTEER_Init(IRQSTEERM7_INST);
    IRQSTEER_EnableInterrupt(IRQSTEERM7_INST, EXAMPLE_MSGINTR_IRQ);
}

status_t APP_MDIO_Init(void)
{
    status_t result = kStatus_Success;
    netc_hw_eth_port_idx_t ethernet_mac_ports[] = EXAMPLE_PORTS;

    netc_mdio_config_t mdioConfig = {
        .isPreambleDisable = false,
        .isNegativeDriven  = false,
        .srcClockHz        = HAL_ClockGetRate(hal_clock_enet),
    };

#ifdef EXAMPLE_PHY_USE_PORT_MDIO
    /* Usually should call EP_Init/SWT_Init then init port MDIO, here just an quick enablement example. */
    NETC_F2_PCI_HDR_TYPE0->PCI_CFH_CMD |=
        (ENETC_PCI_TYPE0_PCI_CFH_CMD_MEM_ACCESS_MASK | ENETC_PCI_TYPE0_PCI_CFH_CMD_BUS_MASTER_EN_MASK);
    NETC_F3_PCI_HDR_TYPE0->PCI_CFH_CMD |=
        (ENETC_PCI_TYPE0_PCI_CFH_CMD_MEM_ACCESS_MASK | ENETC_PCI_TYPE0_PCI_CFH_CMD_BUS_MASTER_EN_MASK);

    for (int i = 0U; i < 5U; i++)
    {
        mdioConfig.mdio.port = (netc_hw_eth_port_idx_t)((uint32_t)kNETC_ENETC0EthPort + i);
        result               = NETC_MDIOInit(&s_mdio_handle[i], &mdioConfig);
        if (result != kStatus_Success)
        {
            return result;
        }
    }
#else
    for (int i = 0U; i < EXAMPLE_PORT_NUM; i++)
    {
        mdioConfig.mdio.type = kNETC_EMdio;
        mdioConfig.mdio.port = ethernet_mac_ports[i];
        result               = NETC_MDIOInit(&s_mdio_handle[i], &mdioConfig);
        if (result != kStatus_Success)
        {
            return result;
        }
    }
#endif

    return result;
}

#ifdef EXAMPLE_PHY_USE_PORT_MDIO
static status_t APP_PMDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    status_t result = kStatus_Success;
    netc_mdio_handle_t *mdioHandle;

    switch (phyAddr)
    {
        case BOARD_EP0_PHY_ADDR:
            mdioHandle = &s_mdio_handle[0];
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

static status_t APP_PMDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    status_t result = kStatus_Success;
    netc_mdio_handle_t *mdioHandle;

    switch (phyAddr)
    {
        case BOARD_EP0_PHY_ADDR:
            mdioHandle = &s_mdio_handle[0];
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
static status_t APP_EMDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    status_t result = kStatus_Success;
    netc_mdio_handle_t *mdioHandle;

    switch (phyAddr)
    {
        case BOARD_EP0_PHY_ADDR:
            mdioHandle = &s_mdio_handle[EXAMPLE_EP0_PORT];
            break;
        case BOARD_EP1_PHY_ADDR:
            mdioHandle = &s_mdio_handle[EXAMPLE_EP1_PORT];
            break;
        case BOARD_EP2_PHY_ADDR:
        /* case BOARD_SWT_PORT2_PHY_ADDR: */
            /*
             * Selection for TSN MAC2 or MAC3 port
             * 0b - MAC2 selected
             * 1b - MAC3 selected
             * enetc0 <-> MAC3 <-> eth2
             * or
             * switch(enetc3)<-> MAC2 <-> eth2
             */
            if (BLK_CTRL_NETCMIX->EXT_PIN_CONTROL & BLK_CTRL_NETCMIX_EXT_PIN_CONTROL_mac2_mac3_sel(1U))
            {
                mdioHandle = &s_mdio_handle[EXAMPLE_EP2_PORT];
            }
            else
            {
                mdioHandle = &s_mdio_handle[EXAMPLE_SWT_PORT2];
            }
            break;
        case BOARD_SWT_PORT0_PHY_ADDR:
            mdioHandle = &s_mdio_handle[EXAMPLE_SWT_PORT0];
            break;
        case BOARD_SWT_PORT1_PHY_ADDR:
            mdioHandle = &s_mdio_handle[EXAMPLE_SWT_PORT1];
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

static status_t APP_EMDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    status_t result = kStatus_Success;
    netc_mdio_handle_t *mdioHandle;

    switch (phyAddr)
    {
        case BOARD_EP0_PHY_ADDR:
            mdioHandle = &s_mdio_handle[EXAMPLE_EP0_PORT];
            break;
        case BOARD_EP1_PHY_ADDR:
            mdioHandle = &s_mdio_handle[EXAMPLE_EP1_PORT];
            break;
        case BOARD_EP2_PHY_ADDR:
        /* case BOARD_SWT_PORT2_PHY_ADDR: */
            /*
             * Selection for TSN MAC2 or MAC3 port
             * 0b - MAC2 selected
             * 1b - MAC3 selected
             * enetc0 <-> MAC3 <-> eth2
             * or
             * switch(enetc3)<-> MAC2 <-> eth2
             */
            if (BLK_CTRL_NETCMIX->EXT_PIN_CONTROL & BLK_CTRL_NETCMIX_EXT_PIN_CONTROL_mac2_mac3_sel(1U))
            {
                mdioHandle = &s_mdio_handle[EXAMPLE_EP2_PORT];
            }
            else
            {
                mdioHandle = &s_mdio_handle[EXAMPLE_SWT_PORT2];
            }
            break;
        case BOARD_SWT_PORT0_PHY_ADDR:
            mdioHandle = &s_mdio_handle[EXAMPLE_SWT_PORT0];
            break;
        case BOARD_SWT_PORT1_PHY_ADDR:
            mdioHandle = &s_mdio_handle[EXAMPLE_SWT_PORT1];
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
#endif

#if BOARD_IMX943_TYPE == BOARD_IMX943_EMULATOR
static status_t APP_Phy8201SetUp(phy_handle_t *handle)
{
    return kStatus_Success;
}

static status_t APP_PHY_SetPort(uint32_t port, phy_config_t *phyConfig)
{
    return kStatus_Success;
}

status_t APP_PHY_Init(void)
{
    return kStatus_Success;
}

status_t APP_PHY_GetLinkStatus(uint32_t port, bool *link)
{
    *link = true;

    return kStatus_Success;
}

status_t APP_PHY_GetLinkModeSpeedDuplex(uint32_t port, netc_hw_mii_mode_t *mode, netc_hw_mii_speed_t *speed, netc_hw_mii_duplex_t *duplex)
{
    switch (port)
    {
        case EXAMPLE_EP0_PORT:
            *mode = kNETC_RgmiiMode;
            *speed = kNETC_MiiSpeed1000M;
            *duplex = kNETC_MiiFullDuplex;
            break;
        case EXAMPLE_EP1_PORT:
            *mode = kNETC_RgmiiMode;
            *speed = kNETC_MiiSpeed1000M;
            *duplex = kNETC_MiiFullDuplex;
            break;
        case EXAMPLE_SWT_PORT0:
            *mode = kNETC_RgmiiMode;
            *speed = kNETC_MiiSpeed1000M;
            *duplex = kNETC_MiiFullDuplex;
            break;
        case EXAMPLE_SWT_PORT1:
            *mode = kNETC_RgmiiMode;
            *speed = kNETC_MiiSpeed1000M;
            *duplex = kNETC_MiiFullDuplex;
            break;
        case EXAMPLE_SWT_PORT2:
            *mode = kNETC_RgmiiMode;
            *speed = kNETC_MiiSpeed1000M;
            *duplex = kNETC_MiiFullDuplex;
            break;
        default:
            assert(false);
            break;
    }

    return kStatus_Success;
}
#elif BOARD_IMX943_TYPE == BOARD_IMX943_EVK
static status_t APP_Phy8201SetUp(phy_handle_t *handle)
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

static status_t APP_PHY_SetPort(uint32_t port, phy_config_t *phyConfig)
{
    status_t result = kStatus_Success;

#ifdef EXAMPLE_PHY_USE_PORT_MDIO
    s_phy_resource[port].write = APP_PMDIOWrite;
    s_phy_resource[port].read  = APP_PMDIORead;
#else
    s_phy_resource[port].write = APP_EMDIOWrite;
    s_phy_resource[port].read  = APP_EMDIORead;
#endif
    result = PHY_Init(&s_phy_handle[port], phyConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    return PHY_EnableLoopback(&s_phy_handle[port], kPHY_LocalLoop, phyConfig->speed, true);
}

status_t APP_PHY_Init(void)
{
    status_t result            = kStatus_Success;
    phy_config_t phy8201Config = {
        .autoNeg   = false,
        .speed     = kPHY_Speed100M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phyrtl8201_ops,
    };

#if 0
    /* Reset PHY8201 for ETH4(EP), ETH0(Switch port0). Power on 150ms, reset 10ms, wait 150ms. */
    RGPIO_PinWrite(EXAMPLE_EP0_PORT_PHY_RESET_PIN, 0);
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    RGPIO_PinWrite(EXAMPLE_EP0_PORT_PHY_RESET_PIN, 1);
    SDK_DelayAtLeastUs(150000, CLOCK_GetFreq(kCLOCK_CpuClk));
#endif
    /* Initialize PHY for EP. */
    phy8201Config.resource = &s_phy_resource[EXAMPLE_EP0_PORT];
    phy8201Config.phyAddr  = BOARD_EP0_PHY_ADDR;
    result = APP_PHY_SetPort(EXAMPLE_EP0_PORT, &phy8201Config);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = APP_Phy8201SetUp(&s_phy_handle[EXAMPLE_EP0_PORT]);
    if (result != kStatus_Success)
    {
        return result;
    }

    return result;
}

status_t APP_PHY_GetLinkStatus(uint32_t port, bool *link)
{
    return PHY_GetLinkStatus(&s_phy_handle[port], link);
}

status_t APP_PHY_GetLinkModeSpeedDuplex(uint32_t port, netc_hw_mii_mode_t *mode, netc_hw_mii_speed_t *speed, netc_hw_mii_duplex_t *duplex)
{
    switch (port)
    {
        case EXAMPLE_EP0_PORT:
            *mode = kNETC_RmiiMode;
            break;
        default:
            assert(false);
            break;
    }

    return PHY_GetLinkSpeedDuplex(&s_phy_handle[port], (phy_speed_t *)speed, (phy_duplex_t *)duplex);
}
#endif
/*${function:end}*/
