/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "board.h"
#include "fsl_enet.h"
#include "fsl_phylan8741.h"
#include "fsl_phytenbaset.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#if BOARD_NETWORK_USE_TENBASET_PHY
extern phy_tenbaset_resource_t g_phy_resource;
extern const phy_operations_t phy_ops;
#define EXAMPLE_PHY_OPS     &phy_ops
#define EXAMPLE_PHY_ADDRESS 0x00U /* Not really used by TENBASET_PHY */
#define EXAMPLE_MII_MODE    kENET_MiiMode
#else
extern phy_lan8741_resource_t g_phy_resource;
#define EXAMPLE_PHY_OPS     &phylan8741_ops
#define EXAMPLE_PHY_ADDRESS 0x00U
#define EXAMPLE_MII_MODE    kENET_RmiiMode
#endif
#define EXAMPLE_PHY_RESOURCE &g_phy_resource
#define EXAMPLE_ENET_BASE    ENET0
#define ENET_EXAMPLE_IRQ     ETHERNET_IRQn
#define EXAMPLE_CLOCK_FREQ   CLOCK_GetCoreSysClkFreq()

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
