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
#include "hal_power.h"
#include "fsl_irqsteer.h"
#include "fsl_netc_endpoint.h"
#include "fsl_netc_mdio.h"
#include "fsl_netc_phy_wrapper.h"
#include "fsl_msgintr.h"

/*${header:end}*/

/*${macro:start}*/
/*${macro:end}*/

/*${variable:start}*/
static status_t ENETC0_PHY_Init(phy_handle_t *phy_handle, const phy_config_t *config);
static status_t ENETC2_PHY_Init(phy_handle_t *phy_handle, const phy_config_t *config);

static netc_mdio_handle_t s_mdio_handle;
static netc_mdio_handle_t s_emdio_handle;

phy_aqr113c_resource_t g_phy_aqr113c_resource;
const phy_operations_t g_app_phy_aqr113c_ops = {.phyInit             = ENETC2_PHY_Init,
                                                .phyWrite            = NULL,
                                                .phyRead             = NULL,
                                                .phyWriteC45         = PHY_AQR113C_Write,
                                                .phyReadC45          = PHY_AQR113C_Read,
                                                .getAutoNegoStatus   = PHY_AQR113C_GetAutoNegotiationStatus,
                                                .getLinkStatus       = PHY_AQR113C_GetLinkStatus,
                                                .getLinkSpeedDuplex  = PHY_AQR113C_GetLinkSpeedDuplex,
                                                .setLinkSpeedDuplex  = PHY_AQR113C_SetLinkSpeedDuplex,
                                                .enableLoopback      = PHY_AQR113C_EnableLoopback,
                                                .enableLinkInterrupt = NULL,
                                                .clearInterrupt      = NULL};

phy_rtl8211f_resource_t g_phy_rtl8211f_resource;
const phy_operations_t g_app_phy_rtl8211f_ops = {.phyInit             = ENETC0_PHY_Init,
                                                 .phyWrite            = PHY_RTL8211F_Write,
                                                 .phyRead             = PHY_RTL8211F_Read,
                                                 .getAutoNegoStatus   = PHY_RTL8211F_GetAutoNegotiationStatus,
                                                 .getLinkStatus       = PHY_RTL8211F_GetLinkStatus,
                                                 .getLinkSpeedDuplex  = PHY_RTL8211F_GetLinkSpeedDuplex,
                                                 .setLinkSpeedDuplex  = PHY_RTL8211F_SetLinkSpeedDuplex,
                                                 .enableLoopback      = PHY_RTL8211F_EnableLoopback,
                                                 .enableLinkInterrupt = PHY_RTL8211F_EnableLinkInterrupt,
                                                 .clearInterrupt      = PHY_RTL8211F_ClearInterrupt};
/*${variable:end}*/

/*${function:start}*/
static status_t ENETC0_MDIO_Init(void)
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

static status_t ENETC0_EMDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return NETC_MDIOWrite(&s_emdio_handle, phyAddr, regAddr, data);
}

static status_t ENETC0_EMDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return NETC_MDIORead(&s_emdio_handle, phyAddr, regAddr, pData);
}

static status_t ENETC0_PHY_Init(phy_handle_t *phy_handle, const phy_config_t *config)
{
    status_t result            = kStatus_Success;

    /* MDIO init */
    result = ENETC0_MDIO_Init();
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Initialize PHY */
    result = PHY_RTL8211F_Init(phy_handle, config);
    return result;
}

static status_t ENETC2_MDIO_Init(void)
{
    status_t result = kStatus_Success;

    netc_mdio_config_t mdioConfig = {
        .isPreambleDisable = false,
        .isNegativeDriven  = false,
        .srcClockHz        = HAL_ClockGetIpFreq(hal_clock_enet),
    };

    /* EMDIO init */
    mdioConfig.mdio.type = kNETC_EMdio;
    result               = NETC_MDIOInit(&s_emdio_handle, &mdioConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Internal MDIO init */
    ENETC2_PCI_HDR_TYPE0->PCI_CFH_CMD |=
        (ENETC_PCI_TYPE0_PCI_CFH_CMD_MEM_ACCESS_MASK | ENETC_PCI_TYPE0_PCI_CFH_CMD_BUS_MASTER_EN_MASK);

    mdioConfig.mdio.type = kNETC_InternalMdio;
    mdioConfig.mdio.port = kNETC_ENETC2EthPort;
    result               = NETC_MDIOInit(&s_mdio_handle, &mdioConfig);
    return result;
}

static status_t ENETC2_EMDIOC45Write(uint8_t portAddr, uint8_t devAddr, uint16_t regAddr, uint16_t data)
{
    return NETC_MDIOC45Write(&s_emdio_handle, portAddr, devAddr, regAddr, data);
}

static status_t ENETC2_EMDIOC45Read(uint8_t portAddr, uint8_t devAddr, uint16_t regAddr, uint16_t *pData)
{
    return NETC_MDIOC45Read(&s_emdio_handle, portAddr, devAddr, regAddr, pData);
}

static status_t ENETC2_PHY_Init(phy_handle_t *phy_handle, const phy_config_t *config)
{
    status_t result            = kStatus_Success;

    /* MDIO init */
    result = ENETC2_MDIO_Init();
    if (result != kStatus_Success)
    {
        return result;
    }

    /* PHY WRAPPER Init */
    NETC_PHYInit(&s_mdio_handle, kNETC_XGMII10G);

    /* Power up and reset */
    pcal6524_handle_t handle;
    BOARD_InitPCAL6524(&handle);
    
    PCAL6524_SetDirection(&handle, (1 << BOARD_PCAL6524_PMIC_PWRON), kPCAL6524_Output);
    PCAL6524_SetPins(&handle, (1 << BOARD_PCAL6524_PMIC_PWRON));
    SDK_DelayAtLeastUs(80000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    
    /* For a complete PHY reset of AQR113C-B1-C, this pin must be asserted low for at least 10ms. And
     * wait for a further 72ms(for internal circuits settling time) before accessing the PHY register */
    PCAL6524_SetDirection(&handle, (1 << BOARD_PCAL6524_ETH3_RESET_B), kPCAL6524_Output);
    PCAL6524_ClearPins(&handle, (1 << BOARD_PCAL6524_ETH3_RESET_B));
    SDK_DelayAtLeastUs(20000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    PCAL6524_SetPins(&handle, (1 << BOARD_PCAL6524_ETH3_RESET_B));
    SDK_DelayAtLeastUs(80000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* Initialize PHY */
    result = PHY_AQR113C_Init(phy_handle, config);

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
    /* lpi2c5Clk 24MHz */
    hal_clk_t hal_lpi2c5ClkCfg = {
        .clk_id = hal_clock_lpi2c5,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };
    /* lpi2c7Clk 24MHz */
    hal_clk_t hal_lpi2c7ClkCfg = {
        .clk_id = hal_clock_lpi2c7,
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
    HAL_ClockSetRootClk(&hal_lpi2c5ClkCfg);
    HAL_ClockSetRootClk(&hal_lpi2c7ClkCfg);

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

    g_phy_rtl8211f_resource.write = ENETC0_EMDIOWrite;
    g_phy_rtl8211f_resource.read  = ENETC0_EMDIORead;

    g_phy_aqr113c_resource.write = ENETC2_EMDIOC45Write;
    g_phy_aqr113c_resource.read  = ENETC2_EMDIOC45Read;
}

/*${function:end}*/
