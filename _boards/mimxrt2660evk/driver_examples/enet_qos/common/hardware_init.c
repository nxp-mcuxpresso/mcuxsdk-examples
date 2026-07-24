/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_enet_qos.h"
#include "fsl_clock.h"
#include "fsl_phy.h"
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

static status_t PHY_Wrapped_Init(phy_handle_t *handle, const phy_config_t *config);

const phy_operations_t phy_ops = {.phyInit            = &PHY_Wrapped_Init,
                                  .getAutoNegoStatus  = &PHY_TENBASET_GetAutoNegotiationStatus,
                                  .getLinkStatus      = &PHY_TENBASET_GetLinkStatus,
                                  .getLinkSpeedDuplex = &PHY_TENBASET_GetLinkSpeedDuplex,
                                  .setLinkSpeedDuplex = &PHY_TENBASET_SetLinkSpeedDuplex,
                                  .enableLoopback     = &PHY_TENBASET_EnableLoopback};
#else
phy_yt8521_resource_t g_phy_resource;
static pcal6524_handle_t s_pcal6524Handle;
#endif
/*${variable:end}*/

/*${function:start}*/
/* Board-provided hooks expected by the SDK ENET_QOS driver. */
void ENET_QOS_EnableClock(bool enable)
{
    (void)enable;
}

void ENET_QOS_SetSYSControl(enet_qos_mii_mode_t miiMode)
{
    (void)miiMode;
}

#if !BOARD_NETWORK_USE_TENBASET_PHY
static void MDIO_Init(void)
{
    CLOCK_EnableClock(s_enetqosClock[ENET_QOS_GetInstance(EXAMPLE_ENET_QOS_BASE)]);
    ENET_QOS_SetSMI(EXAMPLE_ENET_QOS_BASE, CORE_CLK_FREQ);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_QOS_MDIOWrite(EXAMPLE_ENET_QOS_BASE, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_QOS_MDIORead(EXAMPLE_ENET_QOS_BASE, phyAddr, regAddr, pData);
}
#endif

void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
#if BOARD_NETWORK_USE_TENBASET_PHY
    BOARD_InitTenBaseT1S0Pins();
#else
    BOARD_InitENETQOSPins();
    BOARD_Init6524Pins();
    BOARD_InitPCAL6524(&s_pcal6524Handle);
#endif

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
    /* PHY reset via PCAL6524 P2_1 (BOARD_PCAL6524_ETH0_RST_B). */
    {
        uint32_t rstMask = 1U << BOARD_PCAL6524_ETH0_RST_B;
        (void)PCAL6524_SetDirection(&s_pcal6524Handle, rstMask, kPCAL6524_Output);
        (void)PCAL6524_ClearPins(&s_pcal6524Handle, rstMask);
        SDK_DelayAtLeastUs(10000U, CLOCK_GetRootClockFreq(kCLOCK_Root_CMPT_cpu_clk));
        (void)PCAL6524_SetPins(&s_pcal6524Handle, rstMask);
        SDK_DelayAtLeastUs(30000U, CLOCK_GetRootClockFreq(kCLOCK_Root_CMPT_cpu_clk));
    }

    MDIO_Init();
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;
#endif
}

#if BOARD_NETWORK_USE_TENBASET_PHY
/* Wrapper for PHY init to enable loopback */
static status_t PHY_Wrapped_Init(phy_handle_t *handle, const phy_config_t *config)
{
    status_t result;

    result = PHY_TENBASET_Init(handle, config);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Enable PCS loopback */
    return PHY_EnableLoopback(handle, kPHY_LocalLoop, kPHY_Speed10M, true);
}
#endif /* BOARD_NETWORK_USE_TENBASET_PHY */
/*${function:end}*/
