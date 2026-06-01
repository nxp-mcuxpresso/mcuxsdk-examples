/*
 * Copyright 2026 NXP
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

/*${macro:start}*/
#define EXAMPLE_PORT_NUM (EXAMPLE_EP_NUM + EXAMPLE_SWT_MAX_PORT_NUM)

/* GPY215 PHY Regisgers*/
#define PHY_MMD_VSPEC1         (0x1eU)
#define VSPEC1_SGMII_CTRL_REG  (0x8U)
/*${macro:end}*/

/*${variable:start}*/
/* PHY operation. */
static netc_mdio_handle_t s_emdio_handle;
static netc_mdio_handle_t s_mac0_mdio_handle;
static netc_mdio_handle_t s_mac1_mdio_handle;
#if defined(EXAMPLE_USE_PHY_GPY215)
static phy_gpy215_resource_t s_phy_gpy215_resource;
#else
static phy_dp8384x_resource_t s_phy_dp8384x_resource;
#endif
static phy_rtl8211f_resource_t s_phy_rtl8211f_resource;
static uint8_t s_phy_addr[EXAMPLE_PORT_NUM] = EXAMPLE_PHY_ADDR;
static phy_handle_t s_phy_handle[EXAMPLE_PORT_NUM];
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{

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
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    POWER_SetState(&pwrst);
    assert(POWER_GetState(&pwrst) == SCMI_POWER_DOMAIN_STATE_ON);

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
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C3_S5_21_ID, SGMII5_RST_B);
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C3_S5_21_ID, SGMII6_RST_B);

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

    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C3_S5_21_ID, SGMII5_RST_B);
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C3_S5_21_ID, SGMII5_RST_B);
    SDK_DelayAtLeastUs(100000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    BOARD_EXPANDER_SetPinToLow(BOARD_PCA6416_I2C3_S5_21_ID, SGMII6_RST_B);
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    BOARD_EXPANDER_SetPinToHigh(BOARD_PCA6416_I2C3_S5_21_ID, SGMII6_RST_B);
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
#if defined(EXAMPLE_USE_PHY_GPY215)
    BLK_CTRL_NETCMIX->NETC_LINK_CFG0 |= BLK_CTRL_NETCMIX_NETC_LINK_CFG0_MII_PROT(0x3U); /* SGMII */
    BLK_CTRL_NETCMIX->NETC_LINK_CFG1 |= BLK_CTRL_NETCMIX_NETC_LINK_CFG1_MII_PROT(0x3U); /* SGMII */
#else
    BLK_CTRL_NETCMIX->NETC_LINK_CFG0 |= BLK_CTRL_NETCMIX_NETC_LINK_CFG0_MII_PROT(0x0U); /* MII */
    BLK_CTRL_NETCMIX->NETC_LINK_CFG1 |= BLK_CTRL_NETCMIX_NETC_LINK_CFG1_MII_PROT(0x0U); /* MII */
#endif
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
}

status_t APP_MDIO_Init(void)
{
    status_t result = kStatus_Success;

    netc_mdio_config_t mdioConfig = {
        .isPreambleDisable = false,
        .isNegativeDriven  = false,
        .srcClockHz        = CLOCK_GetRate(kCLOCK_Enet),
    };

    mdioConfig.mdio.type = kNETC_EMdio;
    result               = NETC_MDIOInit(&s_emdio_handle, &mdioConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Internal MDIO init */
    SW0_PCI_HDR_TYPE0->PCI_CFH_CMD |=
        (ENETC_PCI_TYPE0_PCI_CFH_CMD_MEM_ACCESS_MASK | ENETC_PCI_TYPE0_PCI_CFH_CMD_BUS_MASTER_EN_MASK);

    mdioConfig.mdio.type = kNETC_InternalMdio;
    mdioConfig.mdio.port = kNETC_SWITCH0EthPort0;
    result               = NETC_MDIOInit(&s_mac0_mdio_handle, &mdioConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    mdioConfig.mdio.type = kNETC_InternalMdio;
    mdioConfig.mdio.port = kNETC_SWITCH0EthPort1;
    result               = NETC_MDIOInit(&s_mac1_mdio_handle, &mdioConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    return result;
}

/* Board issue is causing mdio access error. Workaround is adding delay between mdio accesses. */
static status_t APP_EMDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    return NETC_MDIOWrite(&s_emdio_handle, phyAddr, regAddr, data);
}

static status_t APP_EMDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    return NETC_MDIORead(&s_emdio_handle, phyAddr, regAddr, pData);
}

#if defined(EXAMPLE_USE_PHY_GPY215)
static status_t APP_EMDIOC45Write(uint8_t portAddr, uint8_t devAddr, uint16_t regAddr, uint16_t data)
{
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    return NETC_MDIOC45Write(&s_emdio_handle, portAddr, devAddr, regAddr, data);
}

static status_t APP_EMDIOC45Read(uint8_t portAddr, uint8_t devAddr, uint16_t regAddr, uint16_t *pData)
{
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    return NETC_MDIOC45Read(&s_emdio_handle, portAddr, devAddr, regAddr, pData);
}
#endif

status_t APP_PHY_Init(void)
{
    status_t result            = kStatus_Success;

    /* phyrtl8211f */
    phy_config_t phy8211Config = {
        .autoNeg   = true,
        .speed     = kPHY_Speed1000M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phyrtl8211f_ops,
    };

    s_phy_rtl8211f_resource.write = APP_EMDIOWrite;
    s_phy_rtl8211f_resource.read  = APP_EMDIORead;
    phy8211Config.resource = &s_phy_rtl8211f_resource;

#if defined(EXAMPLE_USE_PHY_GPY215)
    /* phygpy215 */
    phy_config_t phygpy215Config = {
        .autoNeg   = true,
        .speed     = kPHY_Speed2500M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phygpy215_ops,
    };

    s_phy_gpy215_resource.write = APP_EMDIOWrite;
    s_phy_gpy215_resource.read  = APP_EMDIORead;
    s_phy_gpy215_resource.writeExt = APP_EMDIOC45Write;
    s_phy_gpy215_resource.readExt = APP_EMDIOC45Read;
    phygpy215Config.resource = &s_phy_gpy215_resource;
#else
    /* phydp8384x */
    phy_config_t phy8384xConfig = {
        .autoNeg   = true,
        .speed     = kPHY_Speed100M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phydp8384x_ops,
    };

    s_phy_dp8384x_resource.write = APP_EMDIOWrite;
    s_phy_dp8384x_resource.read  = APP_EMDIORead;
    phy8384xConfig.resource = &s_phy_dp8384x_resource.write;
#endif
    for (int i = 0; i < EXAMPLE_PORT_NUM; i++) {
	phy_config_t *phyConfig;
        switch (i)
        {
            case EXAMPLE_EP0_PORT:
            case EXAMPLE_EP1_PORT:
            case EXAMPLE_SWT_PORT2:
                phyConfig = &phy8211Config;
                break;
            case EXAMPLE_SWT_PORT0:
#if defined(EXAMPLE_USE_PHY_GPY215)
                phyConfig = &phygpy215Config;
#else
                phyConfig = &phy8384xConfig;
#endif
                break;
            case EXAMPLE_SWT_PORT1:
#if defined(EXAMPLE_USE_PHY_GPY215)
                phyConfig = &phygpy215Config;
#else
                phyConfig = &phy8384xConfig;
#endif
                break;
            default:
                assert(false);
                break;
        }

        phyConfig->phyAddr = s_phy_addr[i];

#if defined(EXAMPLE_USE_PHY_GPY215)
	/* Need phywrapper init */
	if (i == EXAMPLE_SWT_PORT0)
	{
            NETC_PHYInit(&s_mac0_mdio_handle, kNETC_SGMII2G5);
	}
	else if (i == EXAMPLE_SWT_PORT1)
	{
            NETC_PHYInit(&s_mac1_mdio_handle, kNETC_SGMII2G5);
	}
#endif
        result = PHY_Init(&s_phy_handle[i], phyConfig);
        if (result != kStatus_Success)
        {
            return result;
        }

#if defined(EXAMPLE_USE_PHY_GPY215)
	/* Need to configure PHY SGMII mode */
	if (i == EXAMPLE_SWT_PORT0 || i == EXAMPLE_SWT_PORT1)
        {
            PHY_GPY215_WriteC45(&s_phy_handle[i], PHY_MMD_VSPEC1, VSPEC1_SGMII_CTRL_REG, 0x24faU);
        }
#endif
   /*     result = PHY_EnableLoopback(&s_phy_handle[i], kPHY_LocalLoop, phyConfig->speed, true);
        if (result != kStatus_Success)
        {
            return result;
        }
	*/
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
        case EXAMPLE_EP1_PORT:
        case EXAMPLE_SWT_PORT2:
            *mode = kNETC_RgmiiMode;
            break;
        case EXAMPLE_SWT_PORT0:
        case EXAMPLE_SWT_PORT1:
#if defined(EXAMPLE_USE_PHY_GPY215)
            *mode = kNETC_SgmiiMode;
#else
            *mode = kNETC_MiiMode;
#endif
            break;
        default:
            assert(false);
            break;
    }

    return PHY_GetLinkSpeedDuplex(&s_phy_handle[port], (phy_speed_t *)speed, (phy_duplex_t *)duplex);
}
/*${function:end}*/
