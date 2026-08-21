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
#include "fsl_netc_endpoint.h"
#include "fsl_netc_mdio.h"
#include "fsl_msgintr.h"

/*${header:end}*/

/*${macro:start}*/
/*${macro:end}*/

/*${variable:start}*/
static status_t ENETC_PHY_Init(phy_handle_t *phy_handle, const phy_config_t *config);

static netc_mdio_handle_t s_emdio_handle;

phy_yt8521_resource_t g_phy_yt8521_resource;
const phy_operations_t g_app_phy_yt8521_ops = {.phyInit             = ENETC_PHY_Init,
                                                 .phyWrite            = PHY_YT8521_Write,
                                                 .phyRead             = PHY_YT8521_Read,
                                                 .getAutoNegoStatus   = PHY_YT8521_GetAutoNegotiationStatus,
                                                 .getLinkStatus       = PHY_YT8521_GetLinkStatus,
                                                 .getLinkSpeedDuplex  = PHY_YT8521_GetLinkSpeedDuplex,
                                                 .setLinkSpeedDuplex  = PHY_YT8521_SetLinkSpeedDuplex,
                                                 .enableLoopback      = PHY_YT8521_EnableLoopback,
                                                 .enableLinkInterrupt = PHY_YT8521_EnableLinkInterrupt,
                                                 .clearInterrupt      = PHY_YT8521_ClearInterrupt};
/*${variable:end}*/

/*${function:start}*/
static status_t ENETC_MDIO_Init(void)
{
    netc_mdio_config_t mdioConfig = {
        .isPreambleDisable = false,
        .isNegativeDriven  = false,
        .srcClockHz        = CLOCK_GetRate(kCLOCK_enet),
    };

    /* EMDIO init */
    mdioConfig.mdio.type = kNETC_EMdio;
    return  NETC_MDIOInit(&s_emdio_handle, &mdioConfig);
}

static status_t ENETC_PHY_Init(phy_handle_t *phy_handle, const phy_config_t *config)
{
    status_t result            = kStatus_Success;

    /* MDIO init */
    result = ENETC_MDIO_Init();
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Initialize PHY */
    result = PHY_YT8521_Init(phy_handle, config);
    return result;
}

static status_t ENETC_EMDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return NETC_MDIOWrite(&s_emdio_handle, phyAddr, regAddr, data);
}

static status_t ENETC_EMDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return NETC_MDIORead(&s_emdio_handle, phyAddr, regAddr, pData);
}

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

    g_phy_yt8521_resource.write = ENETC_EMDIOWrite;
    g_phy_yt8521_resource.read  = ENETC_EMDIORead;
}

/*${function:end}*/
