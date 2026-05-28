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
#include "fsl_irqsteer.h"
#include "fsl_netc_endpoint.h"
#include "fsl_netc_mdio.h"
#include "fsl_netc_phy_wrapper.h"
#include "fsl_msgintr.h"
#include "rsc_table.h"

/*${header:end}*/

/*${macro:start}*/
#define IERB_E0FAUXR 0x4cd23044
#define IERB_V0FAUXR 0x4cd24004
/*${macro:end}*/

/*${variable:start}*/
static netc_mdio_handle_t s_mdio_handle;
static netc_mdio_handle_t s_emdio_handle;

phy_rtl8211f_resource_t g_phy_rtl8211f_resource;

const phy_operations_t g_app_phy_rtl8211f_ops = {.phyInit             = APP_PHY_Init,
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
static status_t APP_MDIO_Init(void)
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
    ENETC0_PCI_HDR_TYPE0->PCI_CFH_CMD |=
        (ENETC_PCI_TYPE0_PCI_CFH_CMD_MEM_ACCESS_MASK | ENETC_PCI_TYPE0_PCI_CFH_CMD_BUS_MASTER_EN_MASK);

    mdioConfig.mdio.type = kNETC_InternalMdio;
    mdioConfig.mdio.port = kNETC_ENETC0EthPort;
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

status_t APP_PHY_Init(phy_handle_t *phy_handle, const phy_config_t *config)
{
    status_t result            = kStatus_Success;

    /* MDIO init */
    result = APP_MDIO_Init();
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Initialize PHY */

    /* For a complete PHY reset of RTL8211FDI-CG, this pin must be asserted low for at least 10ms. And
     * wait for a further 72ms(for internal circuits settling time) before accessing the PHY register */
    /* TODO */

    result = PHY_RTL8211F_Init(phy_handle, config);

    return result;
}

status_t APP_NETC_PreinitVsi(netc_enetc_hw_t *hw, netc_hw_si_idx_t si)
{
    uint8_t macAddr0[] = { 0x00, 0x00, 0xfa, 0xfa, 0xdd, 0xa0 };
    netc_hw_enetc_si_config_t vsi0Config = {
        .txRingUse = 3U, .rxRingUse = 3U, .vlanCtrl = (uint32_t)kNETC_ENETC_StanCVlan | (uint32_t)kNETC_ENETC_StanSVlan};
    netc_si_l2vf_config_t vlanConfig = {
        .acceptUntagged = true, .enPromis = true, .useOuterVlanTag = true};
    uint32_t vsi0MsixNum = 4U;
    uint8_t vsi0Num;
    status_t result;

    switch (si)
    {
        case kNETC_ENETC0PSI0:
            vsi0Num = getSiNum(kNETC_ENETC0VSI0);
            break;
        default:
            assert(false);
            break;
    }

    /* RSS key init with generated random values */
    hw->base->PRSSKR0 = 0x995770eaU;
    hw->base->PRSSKR1 = 0x7e8cbbe3U;
    hw->base->PRSSKR2 = 0x9ac8285bU;
    hw->base->PRSSKR3 = 0xce20c189U;
    hw->base->PRSSKR4 = 0xb7ece3ebU;
    hw->base->PRSSKR5 = 0xf8bc2513U;
    hw->base->PRSSKR6 = 0x62613353U;
    hw->base->PRSSKR7 = 0x4c84ae76U;
    hw->base->PRSSKR8 = 0x72c890f1U;
    hw->base->PRSSKR9 = 0xd7a8144bU;

    /* Preinit vsi0 for mac address, BDR num, and MSIX interrupt num */
    NETC_EnetcSetSIMacAddr(hw->base, vsi0Num, macAddr0);

    result = NETC_EnetcSetMsixEntryNum(hw->base, vsi0Num, vsi0MsixNum);
    if (result != kStatus_Success)
    {
        return result;
    }

    result = NETC_EnetcConfigureSI(hw->base, vsi0Num, &vsi0Config);
    if (result != kStatus_Success)
    {
        return result;
    }

    NETC_EnetcEnableSI(hw->base, vsi0Num, true);

    NETC_EnetcConfigureVlanFilter(hw->base, getSiNum(si), &vlanConfig);
    return result;
}

void BOARD_InitHardware(void)
{
    /* clang-format off */
    /* enetClk 666.66MHz */
    clk_t enetclk = {
        .clkId = kCLOCK_enet,
        .pclkId = kCLOCK_syspll1dfs2,
        .rate = 666666666UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    /* enetRefClk 250MHz */
    clk_t enetrefclk = {
        .clkId = kCLOCK_enetref,
        .pclkId = kCLOCK_syspll1dfs0,
        .rate = 250000000UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };
    /* enetTimer1Clk 100MHz */
    clk_t enettimer1clk = {
        .clkId = kCLOCK_enettimer1,
        .pclkId = kCLOCK_syspll1dfs0div2,
        .rate = 100000000UL,
        //.enable_clk = true,
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
    BOARD_InitNETPins();
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

    /* Console init */
    BOARD_InitDebugConsoleForCM7WithSM();

    /* Protocol configure */
    BLK_CTRL_NETCMIX->CFG_LINK_MII_PROT = 0x00000022;

    /* Unlock the IERB. It will warm reset whole NETC. */
    NETC_PRIV->NETCRR &= ~NETC_PRIV_NETCRR_LOCK_MASK;
    while ((NETC_PRIV->NETCRR & NETC_PRIV_NETCRR_LOCK_MASK) != 0U)
    {
    }

    /* ENETC0 PF */
    *((volatile uint32_t *)IERB_E0FAUXR) = 0;

    /* ENETC0 VF0 */
    *((volatile uint32_t *)IERB_V0FAUXR) = 4;

    /* Lock the IERB. */
    NETC_PRIV->NETCRR |= NETC_PRIV_NETCRR_LOCK_MASK;
    while ((NETC_PRIV->NETCSR & NETC_PRIV_NETCSR_STATE_MASK) != 0U)
    {
    }

    g_phy_rtl8211f_resource.write = APP_EMDIOWrite;
    g_phy_rtl8211f_resource.read  = APP_EMDIORead;

    /* copy resource table to destination address (TCM and DRAM) */
    copyResourceTable();

    APP_SRTM_Init();
    APP_SRTM_StartCommunication();
}

/*${function:end}*/
