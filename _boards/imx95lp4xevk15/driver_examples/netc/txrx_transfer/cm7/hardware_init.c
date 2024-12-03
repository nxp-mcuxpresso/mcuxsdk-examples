/*
 * Copyright 2022-2023 NXP
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

/*${variable:start}*/
/* PHY operation. */
static netc_mdio_handle_t s_emdio_handle;
static phy_rtl8211f_resource_t s_phy_rtl8211f_resource;
static phy_handle_t s_phy_handle[EXAMPLE_EP_NUM];
static uint8_t s_phy_addr[EXAMPLE_EP_NUM] = EXAMPLE_EP_PHY_ADDR;
/*${variable:end}*/

/*${function:start}*/
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
    /* lpi2c2Clk 24MHz */
    hal_clk_t hal_lpi2c2ClkCfg = {
        .clk_id = hal_clock_lpi2c2,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
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
    HAL_ClockSetRootClk(&hal_lpi2c2ClkCfg);

    /* Console init */
    BOARD_InitDebugConsoleForCM7WithSM();

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
}

status_t APP_MDIO_Init(void)
{
    netc_mdio_config_t mdioConfig = {
        .isPreambleDisable = false,
        .isNegativeDriven  = false,
        .srcClockHz        = HAL_ClockGetIpFreq(hal_clock_enet),
    };

    /* EMDIO init */
    mdioConfig.mdio.type = kNETC_EMdio;
    return NETC_MDIOInit(&s_emdio_handle, &mdioConfig);
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
    pcal6524_handle_t handle;

    /* EP0 PHY Init */
    phy_config_t ep0PhyConfig = {
        .autoNeg   = true,
        .speed     = kPHY_Speed1000M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phyrtl8211f_ops,
    };

    /* For a complete PHY reset of RTL8211F(I)-VD/RTL8211FD(I)-VD, this pin must be
     * asserted low for at least 10ms for the internal regulator. Wait for at least
     * 72ms (for internal circuits settling time) before accessing the PHY register. */
    BOARD_InitPCAL6524(&handle);
    PCAL6524_SetDirection(&handle, (1 << BOARD_PCAL6524_ENET1_RST_B), kPCAL6524_Output);
    PCAL6524_ClearPins(&handle, (1 << BOARD_PCAL6524_ENET1_RST_B));
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    PCAL6524_SetPins(&handle, (1 << BOARD_PCAL6524_ENET1_RST_B));
    SDK_DelayAtLeastUs(80000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* Initialize PHY for EP0. */
    s_phy_rtl8211f_resource.write = APP_EMDIOWrite;
    s_phy_rtl8211f_resource.read  = APP_EMDIORead;
    ep0PhyConfig.resource = &s_phy_rtl8211f_resource;
    ep0PhyConfig.phyAddr  = s_phy_addr[EXAMPLE_EP0_PORT];
    result = PHY_Init(&s_phy_handle[EXAMPLE_EP0_PORT], &ep0PhyConfig);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = PHY_EnableLoopback(&s_phy_handle[EXAMPLE_EP0_PORT], kPHY_LocalLoop, ep0PhyConfig.speed, true);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* EP1 PHY Init */
    phy_config_t ep1PhyConfig = {
        .autoNeg   = true,
        .speed     = kPHY_Speed1000M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phyrtl8211f_ops,
    };

    /* For a complete PHY reset of RTL8211F(I)-VD/RTL8211FD(I)-VD, this pin must be
     * asserted low for at least 10ms for the internal regulator. Wait for at least
     * 72ms (for internal circuits settling time) before accessing the PHY register. */
    PCAL6524_SetDirection(&handle, (1 << BOARD_PCAL6524_ENET2_RST_B), kPCAL6524_Output);
    PCAL6524_ClearPins(&handle, (1 << BOARD_PCAL6524_ENET2_RST_B));
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    PCAL6524_SetPins(&handle, (1 << BOARD_PCAL6524_ENET2_RST_B));
    SDK_DelayAtLeastUs(80000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* Initialize PHY for EP1. */
    ep1PhyConfig.resource = &s_phy_rtl8211f_resource;
    ep1PhyConfig.phyAddr  = s_phy_addr[EXAMPLE_EP1_PORT];
    result = PHY_Init(&s_phy_handle[EXAMPLE_EP1_PORT], &ep1PhyConfig);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = PHY_EnableLoopback(&s_phy_handle[EXAMPLE_EP1_PORT], kPHY_LocalLoop, ep1PhyConfig.speed, true);

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
            *mode = kNETC_RgmiiMode;
            break;
        default:
            assert(false);
            break;
    }

    return PHY_GetLinkSpeedDuplex(&s_phy_handle[port], (phy_speed_t *)speed, (phy_duplex_t *)duplex);
}
/*${function:end}*/
