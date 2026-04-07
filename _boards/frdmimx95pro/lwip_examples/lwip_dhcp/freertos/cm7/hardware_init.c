/*
 * Copyright 2024, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "sm_platform.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "hal_power.h"
#include "fsl_irqsteer.h"
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
        .srcClockHz        = HAL_ClockGetIpFreq(hal_clock_enet),
    };

    /* EMDIO init */
    mdioConfig.mdio.type = kNETC_EMdio;
    return  NETC_MDIOInit(&s_emdio_handle, &mdioConfig);
}

static status_t ENETC_EMDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return NETC_MDIOWrite(&s_emdio_handle, phyAddr, regAddr, data);
}

static status_t ENETC_EMDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return NETC_MDIORead(&s_emdio_handle, phyAddr, regAddr, pData);
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

void BOARD_InitHardware(void)
{
    /* clang-format off */
    /* enetClk 666.66MHz */
    hal_clk_t hal_enetclk = {
        .clk_id = hal_clock_enet,
        .pclk_id = hal_clock_syspll1dfs2,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* enetRefClk 250MHz */
    hal_clk_t hal_enetrefclk = {
        .clk_id = hal_clock_enetref,
        .pclk_id = hal_clock_syspll1dfs0,
        .div = 4,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* enetTimer1Clk 100MHz */
    hal_clk_t hal_enettimer1clk = {
        .clk_id = hal_clock_enettimer1,
        .pclk_id = hal_clock_syspll1dfs0div2,
        .div = 5,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* NETCMIX power up */
    hal_pwr_s_t pwrst = {
        .did = HAL_POWER_PLATFORM_MIX_SLICE_IDX_NETC,
        .st = hal_power_state_on,
    };
    /* clang-format on */

    SM_Platform_Init();

    /* Power up NETCMIX */
    HAL_PowerSetState(&pwrst);
    while(HAL_PowerGetState(&pwrst))
    {
    }

    /* Pins and clocks init */
    BOARD_InitBootPins();
    BOARD_BootClockRUN();

    HAL_ClockSetRootClk(&hal_enetclk);
    HAL_ClockSetRootClk(&hal_enetrefclk);
    HAL_ClockSetRootClk(&hal_enettimer1clk);

    /* Console init */
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();

    /* Protocol configure */
    BLK_CTRL_NETCMIX->CFG_LINK_MII_PROT = 0x00000522;
    BLK_CTRL_NETCMIX->CFG_LINK_PCS_PROT_2 = 0x00000040;

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

    IRQSTEER_Init(IRQSTEER);
    IRQSTEER_EnableInterrupt(IRQSTEER, MSGINTR2_IRQn);

    g_phy_yt8521_resource.write = ENETC_EMDIOWrite;
    g_phy_yt8521_resource.read  = ENETC_EMDIORead;
}

/*${function:end}*/
