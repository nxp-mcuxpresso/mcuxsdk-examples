/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_enet.h"
#include "fsl_clock.h"
#include "fsl_trdc_soc.h"
#include "fsl_modcon.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
#if !BOARD_NETWORK_USE_TENBASET_PHY
#include "fsl_pcal6524.h"
#endif
/*${header:end}*/

/*${variable:start}*/
#if BOARD_NETWORK_USE_TENBASET_PHY
phy_tenbaset_resource_t g_phy_resource;
#else
phy_yt8521_resource_t g_phy_resource;
static pcal6524_handle_t s_pcal6524Handle;
#endif
/*${variable:end}*/

/*${function:start}*/
void BOARD_ENETFlexibleConfigure(enet_config_t *config)
{
#if BOARD_NETWORK_USE_TENBASET_PHY
    config->miiMode = kENET_MiiMode;
#else
    config->miiMode = kENET_RgmiiMode;
#endif
}

#if !BOARD_NETWORK_USE_TENBASET_PHY
static void MDIO_Init(void)
{
    (void)CLOCK_EnableClock(s_enetClock[ENET_GetInstance(EXAMPLE_ENET)]);
    ENET_SetSMI(EXAMPLE_ENET, EXAMPLE_CLOCK_FREQ, false);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(EXAMPLE_ENET, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(EXAMPLE_ENET, phyAddr, regAddr, pData);
}
#endif

void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
#if BOARD_NETWORK_USE_TENBASET_PHY
    BOARD_InitTenBaseT1S1Pins();
#else
    BOARD_InitENETPins();
    BOARD_Init6524Pins();
    BOARD_InitPCAL6524(&s_pcal6524Handle);
#endif
    /* Refresh SystemCoreClock from the configured clock tree; otherwise it stays at the static
     * DEFAULT_SYSTEM_CLOCK and SysTick-based timing (e.g. iperf throughput) reads at the wrong rate. */
    SystemCoreClockUpdate();

    /* The shared clock tree leaves ETH1_TRXCLK at 250MHz; the ENET1G RGMII RX delay-line reference
     * must be 125MHz (RM), so re-divide it here rather than in the board-wide clock config. */
    {
        clock_root_config_t trxCfg = {.mux = kCLOCK_ETH1_TRXCLK_ClockRoot_MAINPLLDIV8, .div = 2U, .sndDiv = 1U};
        CLOCK_SetRootClock(kCLOCK_Root_COMM_eth1_trxclk, &trxCfg);
    }

    /* Grant the ENET1 masters TRDC domain 0; without this the ENET DMA bus accesses fault. */
    COMM__TRDC->MDA_DFMT1[kTRDC_COMM_MasterENET1_M0R].MDA_W_DFMT1[0] = TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;
    COMM__TRDC->MDA_DFMT1[kTRDC_COMM_MasterENET1_M0T].MDA_W_DFMT1[0] = TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;

#if BOARD_NETWORK_USE_TENBASET_PHY

    /* Route ENET_1G MAC to XENOPHY1 internally */
    MODCON_SetCFG((uint32_t)kModCon_COMM_ETH0, 0, MODCON_CFG_ENET1G_2_XENO_PINSEL(1));

    /* Enable XENOPHY1 clock */
    CLOCK_EnableClock(kCLOCK_COMM_xenophy1);

    /* Enable the TENBASET_PHY1 interrupt */
    EnableIRQ(COMM_TENBASET_PHY1_IRQn);

    /* Configure TENBASET_PHY1 */
    memset(&g_phy_resource, 0, sizeof(phy_tenbaset_resource_t));
    TENBASET_PHY_GetDefaultConfig(&g_phy_resource.config);
    g_phy_resource.base                     = COMM__TENBASET_PHY_1;
    g_phy_resource.config.plcaConfig.enable = true;
    g_phy_resource.config.plcaConfig.nodeId = 1U;
#else
    /* Reset YT8531 PHY via PCAL6524 P2_2 (BOARD_PCAL6524_ETH1_RST_B) */
    {
        uint32_t rstMask = 1U << BOARD_PCAL6524_ETH1_RST_B;
        (void)PCAL6524_SetDirection(&s_pcal6524Handle, rstMask, kPCAL6524_Output);
        (void)PCAL6524_ClearPins(&s_pcal6524Handle, rstMask);
        SDK_DelayAtLeastUs(10000U, CLOCK_GetRootClockFreq(kCLOCK_Root_CMPT_cpu_clk));
        (void)PCAL6524_SetPins(&s_pcal6524Handle, rstMask);
        SDK_DelayAtLeastUs(30000U, CLOCK_GetRootClockFreq(kCLOCK_Root_CMPT_cpu_clk));
    }

    MDIO_Init();
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;

    /* Enable the ENET1G RGMII interface (MODCON ENET1G_CFG RGMII_EN powers up 0 = disabled). */
    MODCON_SetCFG((uint32_t)kModCon_COMM_ETH1, 0U, MODCON_CFG_ENET1G_RGMII_EN(1U));
#endif
}
/*${function:end}*/
