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

/* @TEST_ANCHOR */

/* IP address configuration. */
#ifndef configIP_ADDR0
#define configIP_ADDR0 192
#endif
#ifndef configIP_ADDR1
#define configIP_ADDR1 168
#endif
#ifndef configIP_ADDR2
#define configIP_ADDR2 0
#endif
#ifndef configIP_ADDR3
#define configIP_ADDR3 102
#endif

/* Netmask configuration. */
#ifndef configNET_MASK0
#define configNET_MASK0 255
#endif
#ifndef configNET_MASK1
#define configNET_MASK1 255
#endif
#ifndef configNET_MASK2
#define configNET_MASK2 255
#endif
#ifndef configNET_MASK3
#define configNET_MASK3 0
#endif

/* Gateway address configuration. */
#ifndef configGW_ADDR0
#define configGW_ADDR0 192
#endif
#ifndef configGW_ADDR1
#define configGW_ADDR1 168
#endif
#ifndef configGW_ADDR2
#define configGW_ADDR2 0
#endif
#ifndef configGW_ADDR3
#define configGW_ADDR3 100
#endif

#define EXAMPLE_NETIF_INIT_FN ethernetif0_init

#if BOARD_NETWORK_USE_TENBASET_PHY
extern phy_tenbaset_resource_t g_phy_resource;
#define EXAMPLE_PHY_OPS     &phytenbaset_ops
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
