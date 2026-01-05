/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_clock.h"
#include "fsl_power.h"
/*${header:end}*/

/*${variable:start}*/
/* PHY operation. */
static netc_mdio_handle_t s_mdio_handle;
static netc_mdio_handle_t s_emdio_handle;
static phy_rtl8211f_resource_t s_phy_rtl8211f_resource;
#if (EXAMPLE_EP_NUM == 2)
static phy_gpy215_resource_t s_phy_gpy215_resource;
#endif
static phy_handle_t s_phy_handle[EXAMPLE_EP_NUM];
static uint8_t s_phy_addr[EXAMPLE_EP_NUM] = EXAMPLE_EP_PHY_ADDR;
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    pcal6524_handle_t handle1;

    /* clang-format off */
    /* enetClk 666.66MHz */
    clk_t enetclk = {
        .clkId = kCLOCK_enet,
        .pclkId = kCLOCK_syspll1dfs2,
        .rate = 666666666,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    /* enetRefClk 250MHz */
    clk_t enetrefclk = {
        .clkId = kCLOCK_enetref,
        .pclkId = kCLOCK_syspll1dfs0,
        .rate = 250000000,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    /* enetTimer1Clk 100MHz */
    clk_t enettimer1clk = {
        .clkId = kCLOCK_enettimer1,
        .pclkId = kCLOCK_syspll1dfs0div2,
        .rate = 100000000,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    /* lpi2c7Clk 24MHz */
    clk_t lpi2cClkCfg = {
        .clkId = kCLOCK_lpi2c7,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    /* NETCMIX power up */
    pwr_s_t pwrst = {
        .did = POWER_MIX_SLICE_IDX_NETC,
        .st = SCMI_POWER_DOMAIN_STATE_ON,
    };
    /* clang-format on */

    SystemPlatformInit();

    /* Power up NETCMIX */
    POWER_SetState(&pwrst);
    while(POWER_GetState(&pwrst))
    {
    }

    /* Pins and clocks init */
    BOARD_InitBootPins();
    BOARD_BootClockRUN();

    CLOCK_SetParent(&enetclk);
    CLOCK_SetRate(&enetclk);
    CLOCK_EnableClock(enetclk.clkId);

    CLOCK_SetParent(&enetrefclk);
    CLOCK_SetRate(&enetrefclk);
    CLOCK_EnableClock(enetrefclk.clkId);

    CLOCK_SetParent(&enettimer1clk);
    CLOCK_SetRate(&enettimer1clk);
    CLOCK_EnableClock(enettimer1clk.clkId);

    CLOCK_SetParent(&lpi2cClkCfg);
    CLOCK_SetRate(&lpi2cClkCfg);
    CLOCK_EnableClock(lpi2cClkCfg.clkId);

    /* Console init */
    BOARD_InitDebugConsoleForCM7WithSM();

    /* Select M.2 2.5G ETH card */
    BOARD_InitPCAL6524(&handle1);
    PCAL6524_SetDirection(&handle1, (1 << BOARD_PCAL6524_PCIE1_SEL), kPCAL6524_Output);
    PCAL6524_ClearPins(&handle1, (1 << BOARD_PCAL6524_PCIE1_SEL));
    PCAL6524_SetDirection(&handle1, (1 << BOARD_PCAL6524_WIFI_ETH_SEL), kPCAL6524_Output);
    PCAL6524_SetPins(&handle1, (1 << BOARD_PCAL6524_WIFI_ETH_SEL));
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* Protocol configure */
    BLK_CTRL_NETCMIX->CFG_LINK_MII_PROT = 0x00000042;
    BLK_CTRL_NETCMIX->CFG_LINK_PCS_PROT_1 = 0x00000002;

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
        .srcClockHz        = CLOCK_GetRate(kCLOCK_enet),
    };

    /* EMDIO init */
    mdioConfig.mdio.type = kNETC_EMdio;
    result               = NETC_MDIOInit(&s_emdio_handle, &mdioConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Internal MDIO init */
    ENETC1_PCI_HDR_TYPE0->PCI_CFH_CMD |=
        (ENETC_PCI_TYPE0_PCI_CFH_CMD_MEM_ACCESS_MASK | ENETC_PCI_TYPE0_PCI_CFH_CMD_BUS_MASTER_EN_MASK);

    mdioConfig.mdio.type = kNETC_InternalMdio;
    mdioConfig.mdio.port = kNETC_ENETC1EthPort;
    result               = NETC_MDIOInit(&s_mdio_handle, &mdioConfig);

    return result;
}

static status_t APP_EMDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return NETC_MDIOWrite(&s_emdio_handle, phyAddr, regAddr, data);
}

static status_t APP_EMDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return NETC_MDIORead(&s_emdio_handle, phyAddr, regAddr, pData);
}

#if (EXAMPLE_EP_NUM == 2)
static status_t APP_EMDIOC45Write(uint8_t portAddr, uint8_t devAddr, uint16_t regAddr, uint16_t data)
{
    return NETC_MDIOC45Write(&s_emdio_handle, portAddr, devAddr, regAddr, data);
}

static status_t APP_EMDIOC45Read(uint8_t portAddr, uint8_t devAddr, uint16_t regAddr, uint16_t *pData)
{
    return NETC_MDIOC45Read(&s_emdio_handle, portAddr, devAddr, regAddr, pData);
}
#endif

status_t APP_PHY_Init(void)
{
    status_t result            = kStatus_Success;

    phy_config_t phy8211Config = {
        .autoNeg   = true,
        .speed     = kPHY_Speed1000M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phyrtl8211f_ops,
    };
#if (EXAMPLE_EP_NUM == 2)
    phy_config_t phygpy215Config = {
        .autoNeg   = true,
        .speed     = kPHY_Speed2500M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phygpy215_ops,
    };
#endif
    /* EP0 PHY Init */

    s_phy_rtl8211f_resource.write = APP_EMDIOWrite;
    s_phy_rtl8211f_resource.read  = APP_EMDIORead;
    phy8211Config.resource = &s_phy_rtl8211f_resource;
    phy8211Config.phyAddr  = s_phy_addr[EXAMPLE_EP0_PORT];
    result = PHY_Init(&s_phy_handle[EXAMPLE_EP0_PORT], &phy8211Config);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = PHY_EnableLoopback(&s_phy_handle[EXAMPLE_EP0_PORT], kPHY_LocalLoop, phy8211Config.speed, true);
    if (result != kStatus_Success)
    {
        return result;
    }

#if (EXAMPLE_EP_NUM == 2)
    /* EP1 PHY Init */

    NETC_PHYInit(&s_mdio_handle, kNETC_SGMII2G5);

    s_phy_gpy215_resource.write = APP_EMDIOWrite;
    s_phy_gpy215_resource.read  = APP_EMDIORead;
    s_phy_gpy215_resource.writeExt = APP_EMDIOC45Write;
    s_phy_gpy215_resource.readExt = APP_EMDIOC45Read;
    phygpy215Config.resource = &s_phy_gpy215_resource;
    phygpy215Config.phyAddr  = s_phy_addr[EXAMPLE_EP1_PORT];
    result = PHY_Init(&s_phy_handle[EXAMPLE_EP1_PORT], &phygpy215Config);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = PHY_EnableLoopback(&s_phy_handle[EXAMPLE_EP1_PORT], kPHY_LocalLoop, phygpy215Config.speed, true);
    if (result != kStatus_Success)
    {
        return result;
    }
#endif
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
            *mode = kNETC_RgmiiMode;
            break;
        case EXAMPLE_EP1_PORT:
            *mode = kNETC_SgmiiMode;
            break;
        default:
            assert(false);
            break;
    }

    return PHY_GetLinkSpeedDuplex(&s_phy_handle[port], (phy_speed_t *)speed, (phy_duplex_t *)duplex);
}
/*${function:end}*/
