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
#include "fsl_power.h"
#include "fsl_netc_endpoint.h"
#include "fsl_netc_mdio.h"
#include "fsl_netc_switch.h"
#include "fsl_netc_phy_wrapper.h"
#include "fsl_phygpy215.h"
#include "rsc_table.h"
#include "FreeRTOS.h"
#include "task.h"
/*${header:end}*/

/*${macro:start}*/
/* GPY215 PHY Regisgers*/
#define PHY_MMD_VSPEC1         (0x1eU)
#define VSPEC1_SGMII_CTRL_REG  (0x8U)

#define EXAMPLE_FRAME_FID            1U
#define EXAMPLE_EP_BD_ALIGN          128U
#define EXAMPLE_EP_TXBD_NUM          8U
/*${macro:end}*/

/*${variable:start}*/

/* ENETC3 dummy definitons */
static status_t ENETC3_PHY_Init(phy_handle_t *phy_handle, const phy_config_t *config);
static status_t ENETC3_PHY_GetLinkStatus(phy_handle_t *handle, bool *status);
static status_t ENETC3_PHY_GetLinkSpeedDuplex(phy_handle_t *handle, phy_speed_t *speed, phy_duplex_t *duplex);

const phy_operations_t g_app_phy_dummy_ops = {   .phyInit             = ENETC3_PHY_Init,
                                                 .getLinkStatus       = ENETC3_PHY_GetLinkStatus,
                                                 .getLinkSpeedDuplex  = ENETC3_PHY_GetLinkSpeedDuplex};
/* Switch */
static swt_handle_t g_swt_handle;
static swt_config_t g_swt_config;
static netc_mdio_handle_t s_emdio_handle;
static netc_mdio_handle_t s_mac0_mdio_handle;
static netc_mdio_handle_t s_mac1_mdio_handle;
static phy_handle_t s_phy_handle[EXAMPLE_SWT_PORT_NUM];
static phy_gpy215_resource_t s_phy_gpy215_resource;
static uint8_t s_phy_addr[EXAMPLE_SWT_PORT_NUM] = EXAMPLE_SWT_PHY_ADDR;
AT_NONCACHEABLE_SECTION_ALIGN(static netc_cmd_bd_t g_cmdBuffDescrip[EXAMPLE_EP_TXBD_NUM], EXAMPLE_EP_BD_ALIGN);

static bool s_phy_link[EXAMPLE_SWT_PORT_NUM];
static phy_speed_t s_phy_speed[EXAMPLE_SWT_PORT_NUM];
static phy_duplex_t s_phy_duplex[EXAMPLE_SWT_PORT_NUM];
/*${variable:end}*/

/*${function:start}*/

/* ENETC3 dummy definitions */
static status_t ENETC3_PHY_Init(phy_handle_t *phy_handle, const phy_config_t *config)
{
    return kStatus_Success;
}

static status_t ENETC3_PHY_GetLinkStatus(phy_handle_t *handle, bool *status)
{
    *status = true;
    return kStatus_Success;
}

static status_t ENETC3_PHY_GetLinkSpeedDuplex(phy_handle_t *handle, phy_speed_t *speed, phy_duplex_t *duplex)
{
    *speed = kPHY_Speed1000M;
    *duplex = kPHY_FullDuplex;
    return kStatus_Success;
}

/* Switch functions */
static status_t SWT_MDIO_Init(void)
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
static status_t SWT_EMDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    return NETC_MDIOWrite(&s_emdio_handle, phyAddr, regAddr, data);
}

static status_t SWT_EMDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    return NETC_MDIORead(&s_emdio_handle, phyAddr, regAddr, pData);
}

static status_t SWT_EMDIOC45Write(uint8_t portAddr, uint8_t devAddr, uint16_t regAddr, uint16_t data)
{
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    return NETC_MDIOC45Write(&s_emdio_handle, portAddr, devAddr, regAddr, data);
}

static status_t SWT_EMDIOC45Read(uint8_t portAddr, uint8_t devAddr, uint16_t regAddr, uint16_t *pData)
{
    SDK_DelayAtLeastUs(1000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    return NETC_MDIOC45Read(&s_emdio_handle, portAddr, devAddr, regAddr, pData);
}

static status_t SWT_PHY_Init(void)
{
    status_t result            = kStatus_Success;

    /* phygpy215 */
    phy_config_t phygpy215Config = {
        .autoNeg   = true,
        .speed     = kPHY_Speed2500M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phygpy215_ops,
    };

    s_phy_gpy215_resource.write = SWT_EMDIOWrite;
    s_phy_gpy215_resource.read  = SWT_EMDIORead;
    s_phy_gpy215_resource.writeExt = SWT_EMDIOC45Write;
    s_phy_gpy215_resource.readExt = SWT_EMDIOC45Read;
    phygpy215Config.resource = &s_phy_gpy215_resource;

    for (int i = 0; i < EXAMPLE_SWT_PORT_NUM; i++) {
	phy_config_t *phyConfig;

        phyConfig = &phygpy215Config;
        phyConfig->phyAddr = s_phy_addr[i];

	/* Need phywrapper init */
	if (i == EXAMPLE_SWT_PORT0)
	{
            NETC_PHYInit(&s_mac0_mdio_handle, kNETC_SGMII2G5);
	}
	else if (i == EXAMPLE_SWT_PORT1)
	{
            NETC_PHYInit(&s_mac1_mdio_handle, kNETC_SGMII2G5);
	}

        result = PHY_Init(&s_phy_handle[i], phyConfig);
        if (result != kStatus_Success)
        {
            return result;
        }

	/* Need to configure PHY SGMII mode */
        PHY_GPY215_WriteC45(&s_phy_handle[i], PHY_MMD_VSPEC1, VSPEC1_SGMII_CTRL_REG, 0x24faU);

        s_phy_link[i] = false;
        s_phy_speed[i] = kPHY_Speed2500M;
        s_phy_duplex[i] = kPHY_FullDuplex;
    }

    return result;
}

static status_t SWT_MacLearning_Forwarding(void)
{
    status_t result                  = kStatus_Success;

    SWT_GetDefaultConfig(&g_swt_config);

    for (int i = 0; i < EXAMPLE_SWT_PORT_NUM; i++) {
        g_swt_config.ports[i].ethMac.miiMode   = EXAMPLE_SWT_PORT_MODE;
        g_swt_config.ports[i].ethMac.miiSpeed  = EXAMPLE_SWT_PORT_SPEED;
        g_swt_config.ports[i].ethMac.miiDuplex = EXAMPLE_SWT_PORT_DUPLEX;
        g_swt_config.ports[i].bridgeCfg.isRxVlanAware = false;
    }
    g_swt_config.bridgeCfg.dVFCfg.portMembership = EXAMPLE_SWT_USED_PORT_BITMAP;
    g_swt_config.bridgeCfg.dVFCfg.enUseFilterID = true;
    g_swt_config.bridgeCfg.dVFCfg.filterID = EXAMPLE_FRAME_FID;
    g_swt_config.bridgeCfg.dVFCfg.mfo = kNETC_FDBLookUpWithFlood;
    g_swt_config.bridgeCfg.dVFCfg.mlo = kNETC_HardwareMACLearn;

    g_swt_config.cmdRingUse            = 1U;
    g_swt_config.cmdBdrCfg[0].bdBase   = &g_cmdBuffDescrip[0];
    g_swt_config.cmdBdrCfg[0].bdLength = 8U;

    result = SWT_Init(&g_swt_handle, &g_swt_config);
    if (result != kStatus_Success)
    {
        return result;
    }

    return result;
}

static void SWT_PollLinkTask(void *pvParameters)
{
    while (true) {
        for (int i = 0; i < EXAMPLE_SWT_PORT_NUM; i++) {
            phy_handle_t *phy = &s_phy_handle[i];
            bool link;
            phy_speed_t speed;
            phy_duplex_t duplex;
            status_t st = PHY_GetLinkStatus(phy, &link);

            if (st != kStatus_Success)
            {
                continue;

            }

            /* handle link down */
            if (!link)
            {
                if (s_phy_link[i] != link)
                {
                    PRINTF("Switch port %d link down\r\n", i);
                    s_phy_link[i] = link;
                }
                continue;
            }

            /* handle link up */
            st = PHY_GetLinkSpeedDuplex(phy, &speed, &duplex);
            if (st != kStatus_Success)
            {
                continue;
            }

            if (s_phy_link[i] != link || s_phy_speed[i] != speed || s_phy_duplex[i] != duplex)
            {
                st = SWT_SetEthPortMII(&g_swt_handle, i, (netc_hw_mii_speed_t)speed, (netc_hw_mii_duplex_t)duplex);
                if (st != kStatus_Success)
                {
                    continue;
                }

                PRINTF("Switch port %d link up\r\n", i);
                s_phy_link[i] = link;
                s_phy_speed[i] = speed;
                s_phy_duplex[i] = duplex;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

status_t APP_NETC_PreinitVsi(netc_enetc_hw_t *hw, netc_hw_si_idx_t si)
{
    uint8_t macAddr0[] = { 0x00, 0x00, 0xfa, 0xfa, 0xdd, 0xa0 };
    uint8_t macAddr1[] = { 0x00, 0x00, 0xfa, 0xfa, 0xdd, 0xa1 };
    uint8_t macAddr2[] = { 0x00, 0x00, 0xfa, 0xfa, 0xdd, 0xa2 };
    netc_hw_enetc_si_config_t vsi0Config = {
        .txRingUse = 4U, .rxRingUse = 4U, .vlanCtrl = (uint32_t)kNETC_ENETC_StanCVlan | (uint32_t)kNETC_ENETC_StanSVlan};
    netc_hw_enetc_si_config_t vsi1Config = {
        .txRingUse = 4U, .rxRingUse = 4U, .vlanCtrl = (uint32_t)kNETC_ENETC_StanCVlan | (uint32_t)kNETC_ENETC_StanSVlan};
    netc_hw_enetc_si_config_t vsi2Config = {
        .txRingUse = 4U, .rxRingUse = 4U, .vlanCtrl = (uint32_t)kNETC_ENETC_StanCVlan | (uint32_t)kNETC_ENETC_StanSVlan};
    netc_si_l2vf_config_t vlanConfig = {
        .acceptUntagged = true, .enPromis = true, .useOuterVlanTag = true};
    uint32_t vsi0MsixNum = 5U;
    uint32_t vsi1MsixNum = 5U;
    uint32_t vsi2MsixNum = 5U;
    uint8_t vsi0Num;
    uint8_t vsi1Num;
    uint8_t vsi2Num;
    status_t result;

    switch (si)
    {
        case kNETC_ENETC3PSI0:
            vsi0Num = getSiNum(kNETC_ENETC3VSI1);
            vsi1Num = getSiNum(kNETC_ENETC3VSI2);
            vsi2Num = getSiNum(kNETC_ENETC3VSI3);
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

    /* Preinit vsi1 for mac address, BDR num, and MSIX interrupt num */
    NETC_EnetcSetSIMacAddr(hw->base, vsi1Num, macAddr1);

    result = NETC_EnetcSetMsixEntryNum(hw->base, vsi1Num, vsi1MsixNum);
    if (result != kStatus_Success)
    {
        return result;
    }

    result = NETC_EnetcConfigureSI(hw->base, vsi1Num, &vsi1Config);
    if (result != kStatus_Success)
    {
        return result;
    }

    NETC_EnetcEnableSI(hw->base, vsi1Num, true);

    /* Preinit vsi2 for mac address, BDR num, and MSIX interrupt num */
    NETC_EnetcSetSIMacAddr(hw->base, vsi2Num, macAddr2);

    result = NETC_EnetcSetMsixEntryNum(hw->base, vsi2Num, vsi2MsixNum);
    if (result != kStatus_Success)
    {
        return result;
    }

    result = NETC_EnetcConfigureSI(hw->base, vsi2Num, &vsi2Config);
    if (result != kStatus_Success)
    {
        return result;
    }

    NETC_EnetcEnableSI(hw->base, vsi2Num, true);

    NETC_EnetcConfigureVlanFilter(hw->base, getSiNum(si), &vlanConfig);
    return result;
}

void BOARD_InitHardware(void)
{
    uint32_t st = SCMI_POWER_DOMAIN_STATE_OFF;
    status_t result;

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
    BOARD_ConfigMPU();

    POWER_SetState(&pwrst);
    st = POWER_GetState(&pwrst);
    assert(st == SCMI_POWER_DOMAIN_STATE_ON);
    (void)st;

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
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C3_S5_21_ID, SGMII5_RST_B);
    BOARD_EXPANDER_SetPinAsOutput(BOARD_PCA6416_I2C3_S5_21_ID, SGMII6_RST_B);

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
    BLK_CTRL_NETCMIX->EXT_PIN_CONTROL |= BLK_CTRL_NETCMIX_EXT_PIN_CONTROL_mac2_mac3_sel(1U);

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

    /* Init switch */
    result = SWT_MDIO_Init();
    if (result != kStatus_Success)
    {
        PRINTF("\r\nMDIO Init failed!\r\n");
    }

    result = SWT_PHY_Init();
    if (result != kStatus_Success)
    {
        PRINTF("\r\nPHY Init failed!\r\n");
    }

    result = SWT_MacLearning_Forwarding();
    if (result != kStatus_Success)
    {
        PRINTF("\r\nSwitch Init failed!\r\n");
    }

    PRINTF("\r\nSwitch is initialized.\r\n");

    xTaskCreate(SWT_PollLinkTask, "Switch link polling", 512U, NULL, 3U, NULL);

    /* Init SRTM services */
    copyResourceTable();
    APP_SRTM_Init();
    APP_SRTM_StartCommunication();

    PRINTF("\r\nInitializing ENETC for demo...\r\n");
}
/*${function:end}*/
