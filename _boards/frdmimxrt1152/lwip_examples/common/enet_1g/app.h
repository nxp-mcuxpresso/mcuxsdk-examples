/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "board.h"
#include "fsl_phyyt8521.h"
#include "pin_mux.h"
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

extern phy_yt8521_resource_t g_phy_resource;
#define EXAMPLE_ENET          ENET_1G
#define EXAMPLE_PHY_ADDRESS   BOARD_ENET1_PHY_ADDRESS
#define EXAMPLE_PHY_OPS       &phyyt8521_ops
#define EXAMPLE_PHY_RESOURCE  &g_phy_resource
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#define EXAMPLE_CLOCK_FREQ    CLOCK_GetRootClockFreq(kCLOCK_Root_Bus)

#define EXAMPLE_PHY_INT_PORT BOARD_INITENET_1GPINS_ETH0_INT_B_GPIO
#define EXAMPLE_PHY_INT_PIN  BOARD_INITENET_1GPINS_ETH0_INT_B_GPIO_PIN

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
