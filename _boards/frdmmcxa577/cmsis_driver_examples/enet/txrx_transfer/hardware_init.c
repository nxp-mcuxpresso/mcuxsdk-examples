/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include "fsl_enet_cmsis.h"
#include "fsl_enet_phy_cmsis.h"
#include "fsl_phytenbaset.h"
#include "fsl_enet.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"

cmsis_enet_mac_resource_t ENET0_Resource;
cmsis_enet_phy_resource_t ENETPHY0_Resource;
phy_tenbaset_resource_t g_phy_resource;

static status_t PHY_Wrapped_Init(phy_handle_t *handle, const phy_config_t *config);

const phy_operations_t phy_ops = {
    .phyInit            = &PHY_Wrapped_Init,
    .getAutoNegoStatus  = &PHY_TENBASET_GetAutoNegotiationStatus,
    .getLinkStatus      = &PHY_TENBASET_GetLinkStatus,
    .getLinkSpeedDuplex = &PHY_TENBASET_GetLinkSpeedDuplex,
    .setLinkSpeedDuplex = &PHY_TENBASET_SetLinkSpeedDuplex,
    .enableLoopback     = &PHY_TENBASET_EnableLoopback,
};

void ENET0_InitPins(void)
{
    BOARD_InitTenBaseT1SPins();
}

void ENET0_DeinitPins(void)
{
}

uint32_t ENET0_GetFreq(void)
{
    return CLOCK_GetCoreSysClkFreq();
}

void BOARD_InitCmsisEnetResources(void)
{
    ENET0_Resource.base    = ENET0;
    ENET0_Resource.GetFreq = ENET0_GetFreq;

    ENETPHY0_Resource.phyAddr     = RTE_ENET_PHY_ADDRESS;
    ENETPHY0_Resource.ops         = &phy_ops;
    ENETPHY0_Resource.opsResource = &g_phy_resource;
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_BootClockPLL200M();
    BOARD_InitDebugConsole();

    RESET_PeripheralReset(kENET0_RST_SHIFT_RSTn);

    CLOCK_AttachClk(kNONE_to_ENETRMII);
    CLOCK_EnableClock(s_enetClock[ENET_GetInstance(ENET0)]);

    CLOCK_AttachClk(kPll1Clk_to_TENBASET_PHY);
    CLOCK_SetClockDiv(kCLOCK_DivTENBASET_PHY0, 2);
    CLOCK_EnableClock(kCLOCK_GateTENBASET_PHY0);
    RESET_PeripheralReset(kT1S0_RST_SHIFT_RSTn);

    SYSCON->ENET_CTRL = SYSCON_ENET_CTRL_PHY_SEL(1) | SYSCON_ENET_CTRL_PHY_INTF(0);

    memset(&g_phy_resource, 0, sizeof(g_phy_resource));
    TENBASET_PHY_GetDefaultConfig(&g_phy_resource.config);
    g_phy_resource.base                            = T1S0;
    g_phy_resource.config.plcaConfig.enable        = RTE_ENET_T1S_PLCA_ENABLE;
    g_phy_resource.config.plcaConfig.nodeId        = RTE_ENET_T1S_PLCA_NODE_ID;
    g_phy_resource.config.plcaConfig.nodeCount     = RTE_ENET_T1S_PLCA_NODE_COUNT;
    g_phy_resource.config.plcaConfig.toTimer       = RTE_ENET_T1S_PLCA_TO_TIMER;
    g_phy_resource.config.plcaConfig.burstTimer    = RTE_ENET_T1S_PLCA_BURST_TIMER;
    g_phy_resource.config.plcaConfig.maxBurstCount = RTE_ENET_T1S_PLCA_MAX_BURST_COUNT;

    BOARD_InitCmsisEnetResources();
}

static status_t PHY_Wrapped_Init(phy_handle_t *handle, const phy_config_t *config)
{
    status_t result;

    result = PHY_TENBASET_Init(handle, config);
    if (result != kStatus_Success)
    {
        return result;
    }

    return PHY_EnableLoopback(handle, kPHY_LocalLoop, kPHY_Speed10M, true);
}
