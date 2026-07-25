/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_enet_qos.h"
#include "fsl_clock.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_adapter_uart.h"
#include "fsl_modcon.h"
#include "fsl_trdc_soc.h"
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
/* Board-provided hooks expected by the SDK ENET_QOS driver. */
void ENET_QOS_EnableClock(bool enable)
{
    if (enable)
    {
        (void)CLOCK_EnableClock(kCLOCK_COMM_eth0);
    }
    else
    {
        (void)CLOCK_DisableClock(kCLOCK_COMM_eth0);
    }
}

/* Select the ENET_QOS PHY interface in ETH0_ENET_QOS_CFG (RM 27.9.5). INTF_SEL is sampled at the
 * DMA_MODE.SWR reset that ENET_QOS_Init() asserts right after this hook. */
void ENET_QOS_SetSYSControl(enet_qos_mii_mode_t miiMode)
{
    uint32_t primask = DisableGlobalIRQ();
    uint32_t v       = MODCON_GetCFG((uint32_t)kModCon_COMM_ETH0, 0U);

    v &= ~(MODCON_CFG_ENET_QOS_INTF_SEL_MASK | MODCON_CFG_ENET_QOS_RGMII_EN_MASK |
           MODCON_CFG_ENET_QOS_REF_CLK_DIR_MASK);
    v |= MODCON_CFG_ENET_QOS_CLKGEN_EN(1U); /* clock generator - required in all modes */

    switch (miiMode)
    {
        case kENET_QOS_MiiMode:
            /* INTF_SEL=0 (MII), RGMII_EN=0 (TX_CLK is input in MII mode) */
            break;
        case kENET_QOS_RmiiMode:
            v |= MODCON_CFG_ENET_QOS_INTF_SEL(4U);
            break;
        case kENET_QOS_RgmiiMode:
            v |= MODCON_CFG_ENET_QOS_INTF_SEL(1U) | MODCON_CFG_ENET_QOS_RGMII_EN(1U);
            break;
        default:
            break;
    }
    MODCON_SetCFG((uint32_t)kModCon_COMM_ETH0, 0U, v);
    EnableGlobalIRQ(primask);
}

#if !BOARD_NETWORK_USE_TENBASET_PHY
static void MDIO_Init(void)
{
    (void)CLOCK_EnableClock(s_enetqosClock[ENET_QOS_GetInstance(EXAMPLE_ENET_QOS)]);
    ENET_QOS_SetSMI(EXAMPLE_ENET_QOS, EXAMPLE_CLOCK_FREQ);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_QOS_MDIOWrite(EXAMPLE_ENET_QOS, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_QOS_MDIORead(EXAMPLE_ENET_QOS, phyAddr, regAddr, pData);
}
#endif

void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();

    NVIC_SetPriority(HSP_LPUART0_IRQn, HAL_UART_ISR_PRIORITY);
#if BOARD_NETWORK_USE_TENBASET_PHY
    BOARD_InitTenBaseT1S0Pins();
#else
    BOARD_InitENETQOSPins();
    BOARD_Init6524Pins();
    BOARD_InitPCAL6524(&s_pcal6524Handle);
#endif
    /* Refresh SystemCoreClock from the configured clock tree; otherwise it stays at the static
     * DEFAULT_SYSTEM_CLOCK and SysTick-based timing (e.g. iperf throughput) reads at the wrong rate. */
    SystemCoreClockUpdate();

    /* The shared clock tree leaves ETH0_TRXCLK at 250MHz; the ENET_QOS RGMII TX clock generator
     * needs a 125MHz reference, so re-divide it here rather than in the board-wide clock config. */
    {
        clock_root_config_t trxCfg = {.mux = kCLOCK_ETH0_TRXCLK_ClockRoot_MAINPLLDIV8, .div = 2U, .sndDiv = 1U};
        CLOCK_SetRootClock(kCLOCK_Root_COMM_eth0_trxclk, &trxCfg);
    }

    /* Grant the ENET COMM masters TRDC domain 0; without this the ENET DMA bus accesses fault. */
    COMM__TRDC->MDA_DFMT1[kTRDC_COMM_MasterENET0].MDA_W_DFMT1[0]     = TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;
    COMM__TRDC->MDA_DFMT1[kTRDC_COMM_MasterENET1_M0R].MDA_W_DFMT1[0] = TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;
    COMM__TRDC->MDA_DFMT1[kTRDC_COMM_MasterENET1_M0T].MDA_W_DFMT1[0] = TRDC_MDA_W_DFMT1_DID(0) | TRDC_MDA_W_DFMT1_VLD_MASK;

#if BOARD_NETWORK_USE_TENBASET_PHY
    /* Route ENET_QOS MAC to XENOPHY0 internally */
    MODCON_SetCFG((uint32_t)kModCon_COMM_ETH1, 0, MODCON_CFG_ENET_QOS_2_XENO_PINSEL(1));

    /* Enable XENOPHY0 clock */
    CLOCK_EnableClock(kCLOCK_COMM_xenophy0);

    /* Enable the TENBASET_PHY0 interrupt */
    EnableIRQ(COMM_TENBASET_PHY0_IRQn);

    /* Configure TENBASET_PHY0 */
    memset(&g_phy_resource, 0, sizeof(phy_tenbaset_resource_t));
    TENBASET_PHY_GetDefaultConfig(&g_phy_resource.config);
    g_phy_resource.base                     = COMM__TENBASET_PHY_0;
    g_phy_resource.config.plcaConfig.enable = true;
    g_phy_resource.config.plcaConfig.nodeId = 1U;
#else
    /* Reset YT8531 PHY via PCAL6524 P2_1 (BOARD_PCAL6524_ETH0_RST_B); release ETH1_RST_B with it,
     * the two YT8531 share resources. */
    {
        uint32_t rstMask = (1U << BOARD_PCAL6524_ETH0_RST_B) | (1U << BOARD_PCAL6524_ETH1_RST_B);
        (void)PCAL6524_SetDirection(&s_pcal6524Handle, rstMask, kPCAL6524_Output);
        (void)PCAL6524_ClearPins(&s_pcal6524Handle, rstMask);
        SDK_DelayAtLeastUs(10000U, CLOCK_GetRootClockFreq(kCLOCK_Root_CMPT_cpu_clk));
        (void)PCAL6524_SetPins(&s_pcal6524Handle, rstMask);
        SDK_DelayAtLeastUs(30000U, CLOCK_GetRootClockFreq(kCLOCK_Root_CMPT_cpu_clk));
    }

    NVIC_SetPriority(COMM_ENET_QOS_IRQn, ENET_PRIORITY);

    MDIO_Init();
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;

    /* ENET_QOS (DWC IP) has no MAC-side RGMII TX delay line like ENET1G's, so set the TX delay on
     * the YT8531 PHY: ext reg 0xA003, GE_TX_DELAY[3:0], 150ps/step. GE_TX_DELAY=0x9 sits in the
     * centre of the clean RGMII-TX timing window; the old 0x2 sat on the window edge and corrupted
     * ~1.1% of board->PC gigabit frames -- invisible to the light-traffic ENET_QOS examples but
     * enough to make lwip_iperf_enet_qos collapse (TCP dual-mode TCP_ABORTED + unstable gigabit
     * auto-negotiation). 0x9 -> 0 CRC. Mirrors frdmimxrt1186 (same PHY). */
    {
        uint16_t rgmiiCfg1 = 0U;
        (void)MDIO_Write(EXAMPLE_PHY_ADDRESS, 0x1EU, 0xA003U);
        (void)MDIO_Read(EXAMPLE_PHY_ADDRESS, 0x1FU, &rgmiiCfg1);
        rgmiiCfg1 = (uint16_t)((rgmiiCfg1 & ~0xFU) | 0x9U);
        (void)MDIO_Write(EXAMPLE_PHY_ADDRESS, 0x1FU, rgmiiCfg1);
    }
#endif
}
/*${function:end}*/
