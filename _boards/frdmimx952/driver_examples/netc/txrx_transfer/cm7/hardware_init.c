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
#include "fsl_clock.h"
#include "fsl_power.h"
/*${header:end}*/

/*${variable:start}*/
/* PHY operation. */
static netc_mdio_handle_t s_emdio_handle;
static phy_yt8521_resource_t s_phy_yt8521_resource;
static phy_handle_t s_phy_handle[EXAMPLE_EP_NUM];
static uint8_t s_phy_addr[EXAMPLE_EP_NUM] = EXAMPLE_EP_PHY_ADDR;


/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */

    /* enetClk initialized in SM */

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

    /* Pins init */
    BOARD_InitBootPins();
    BOARD_InitNETPins();

    /* Clocks init */
    BOARD_BootClockRUN();

    CLOCK_SetParent(&enetrefclk);
    CLOCK_SetRate(&enetrefclk);
    CLOCK_EnableClock(enetrefclk.clkId);

    CLOCK_SetParent(&enettimer1clk);
    CLOCK_SetRate(&enettimer1clk);
    CLOCK_EnableClock(enettimer1clk.clkId);

    /* Console init */
    BOARD_InitDebugConsoleForCM7WithSM();

    BOARD_ConfigMPU();

    /* Protocol configure */
    BLK_CTRL_NETCMIX->CFG_LINK_MII_PROT = 0x00000022;

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

status_t APP_PHY_Init(void)
{
    status_t result            = kStatus_Success;

    phy_config_t phyyt8521Config = {
        .autoNeg   = true,
        .speed     = kPHY_Speed1000M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phyyt8521_ops,
    };

    /* EP0 PHY Init */

    s_phy_yt8521_resource.write = APP_EMDIOWrite;
    s_phy_yt8521_resource.read  = APP_EMDIORead;
    phyyt8521Config.resource = &s_phy_yt8521_resource;
    phyyt8521Config.phyAddr  = s_phy_addr[EXAMPLE_EP0_PORT];

    result = PHY_Init(&s_phy_handle[EXAMPLE_EP0_PORT], &phyyt8521Config);
    if (result != kStatus_Success)
    {
        return result;
    }

    result = PHY_EnableLoopback(&s_phy_handle[EXAMPLE_EP0_PORT], kPHY_LocalLoop, phyyt8521Config.speed, true);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* EP1 PHY Init */

    phyyt8521Config.phyAddr  = s_phy_addr[EXAMPLE_EP1_PORT];

    result = PHY_Init(&s_phy_handle[EXAMPLE_EP1_PORT], &phyyt8521Config);
    if (result != kStatus_Success)
    {
        return result;
    }

    result = PHY_EnableLoopback(&s_phy_handle[EXAMPLE_EP1_PORT], kPHY_LocalLoop, phyyt8521Config.speed, true);

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
            *mode = kNETC_RgmiiMode;
            break;
        default:
            assert(false);
            break;
    }

    return PHY_GetLinkSpeedDuplex(&s_phy_handle[port], (phy_speed_t *)speed, (phy_duplex_t *)duplex);
}
/*${function:end}*/
