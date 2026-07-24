/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "board.h"
#include "fsl_enet_qos.h"
#include "fsl_phyyt8521.h"
#include "fsl_phytenbaset.h"
/*${header:end}*/

/*${macro:start}*/
#if BOARD_NETWORK_USE_TENBASET_PHY
extern phy_tenbaset_resource_t g_phy_resource;
extern const phy_operations_t phy_ops;
#define EXAMPLE_PHY_OPS       &phy_ops
#define EXAMPLE_PHY_RESOURCE  &g_phy_resource
#define EXAMPLE_MII_MODE      kENET_QOS_MiiMode
#else
extern phy_yt8521_resource_t g_phy_resource;
#define EXAMPLE_PHY_OPS       &phyyt8521_ops
#define EXAMPLE_PHY_RESOURCE  &g_phy_resource
/* mimxrt2660evk wires COMM_ENET_QOS to a gigabit YT8531 PHY over RGMII. The YT8521 SDK driver is reused because YT8531 is register-compatible. */
#define EXAMPLE_MII_MODE      kENET_QOS_RgmiiMode
#endif
#define EXAMPLE_ENET_QOS_BASE COMM__ENET_QOS
#define EXAMPLE_PHY_ADDR      0x00U
#define CORE_CLK_FREQ         CLOCK_GetRootClockFreq(kCLOCK_Root_COMM_comm_clk)
#define ENET_PTP_REF_CLK      50000000UL
/*${macro:end}*/

/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
