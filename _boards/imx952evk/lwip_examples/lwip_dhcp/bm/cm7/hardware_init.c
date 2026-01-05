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
#include "fsl_netc_endpoint.h"
#include "fsl_netc_mdio.h"
#include "fsl_netc_phy_wrapper.h"
#include "fsl_msgintr.h"

/*${header:end}*/

/*${macro:start}*/
/*${macro:end}*/

/*${variable:start}*/
static status_t ENETC0_PHY_Init(phy_handle_t *phy_handle, const phy_config_t *config);
static status_t ENETC1_PHY_Init(phy_handle_t *phy_handle, const phy_config_t *config);

static netc_mdio_handle_t s_mdio_handle;
static netc_mdio_handle_t s_emdio_handle;

phy_gpy215_resource_t g_phy_gpy215_resource;
const phy_operations_t g_app_phy_gpy215_ops = {.phyInit             = ENETC1_PHY_Init,
                                                .phyWrite            = PHY_GPY215_Write,
                                                .phyRead             = PHY_GPY215_Read,
                                                .phyWriteC45         = PHY_GPY215_WriteC45,
                                                .phyReadC45          = PHY_GPY215_ReadC45,
                                                .getAutoNegoStatus   = PHY_GPY215_GetAutoNegotiationStatus,
                                                .getLinkStatus       = PHY_GPY215_GetLinkStatus,
                                                .getLinkSpeedDuplex  = PHY_GPY215_GetLinkSpeedDuplex,
                                                .setLinkSpeedDuplex  = PHY_GPY215_SetLinkSpeedDuplex,
                                                .enableLoopback      = PHY_GPY215_EnableLoopback,
                                                .enableLinkInterrupt = PHY_GPY215_EnableLinkInterrupt,
                                                .clearInterrupt      = PHY_GPY215_ClearInterrupt};

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
        .srcClockHz        = CLOCK_GetRate(kCLOCK_enet),
    };

    /* EMDIO init */
    mdioConfig.mdio.type = kNETC_EMdio;
    return  NETC_MDIOInit(&s_emdio_handle, &mdioConfig);
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

static status_t ENETC1_MDIO_Init(void)
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

static status_t ENETC1_PHY_Init(phy_handle_t *phy_handle, const phy_config_t *config)
{
    status_t result            = kStatus_Success;

    /* MDIO init */
    result = ENETC1_MDIO_Init();
    if (result != kStatus_Success)
    {
        return result;
    }

    /* PHY WRAPPER Init */
    NETC_PHYInit(&s_mdio_handle, kNETC_SGMII2G5);

    /* Initialize PHY */
    result = PHY_GPY215_Init(phy_handle, config);

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

static status_t ENETC_EMDIOC45Write(uint8_t portAddr, uint8_t devAddr, uint16_t regAddr, uint16_t data)
{
    return NETC_MDIOC45Write(&s_emdio_handle, portAddr, devAddr, regAddr, data);
}

static status_t ENETC_EMDIOC45Read(uint8_t portAddr, uint8_t devAddr, uint16_t regAddr, uint16_t *pData)
{
    return NETC_MDIOC45Read(&s_emdio_handle, portAddr, devAddr, regAddr, pData);
}

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
    /* NETCMIX power up */
    pwr_s_t pwrst = {
        .did = POWER_MIX_SLICE_IDX_NETC,
        .st = SCMI_POWER_DOMAIN_STATE_ON,
    };
    /* lpi2c7Clk 24MHz */
    clk_t lpi2cClkCfg = {
        .clkId = kCLOCK_lpi2c7,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
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

    g_phy_rtl8211f_resource.write = ENETC_EMDIOWrite;
    g_phy_rtl8211f_resource.read  = ENETC_EMDIORead;

    g_phy_gpy215_resource.write = ENETC_EMDIOWrite;
    g_phy_gpy215_resource.read  = ENETC_EMDIORead;
    g_phy_gpy215_resource.writeExt = ENETC_EMDIOC45Write;
    g_phy_gpy215_resource.readExt  = ENETC_EMDIOC45Read;
}

/*${function:end}*/
