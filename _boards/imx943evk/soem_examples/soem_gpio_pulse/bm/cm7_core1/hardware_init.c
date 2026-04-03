/*
 * Copyright 2024 NXP
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

/*${variable:start}*/
struct netc_ep_if_port if_port;

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

    pwr_s_t pwrst = {
        .did =  POWER_MIX_SLICE_IDX_NETC,
        .st = SCMI_POWER_DOMAIN_STATE_ON,
    };
    /* clang-format on */

    SystemPlatformInit();
	
	BOARD_InitDebugConsolePins();	
    BOARD_InitBootPins();
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

