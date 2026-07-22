/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "board.h"
#include "fsl_phyjl1111.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
extern phy_jl1111_resource_t g_phy_resource;
/* Board local PHY ops, copy of phyjl1111_ops with enableLoopback clamped
 * to 100M. */
extern phy_operations_t g_board_phy_ops;
#define EXAMPLE_ENET_QOS_BASE ENET_QOS
#define EXAMPLE_PHY_ADDR      BOARD_ENET0_PHY_ADDRESS
#define EXAMPLE_PHY_OPS       &g_board_phy_ops
#define EXAMPLE_PHY_RESOURCE  &g_phy_resource
#define EXAMPLE_MII_MODE      kENET_QOS_RmiiMode
#define CORE_CLK_FREQ         CLOCK_GetRootClockFreq(kCLOCK_Root_Bus)
#define ENET_PTP_REF_CLK      50000000UL
/* Enable PHY internal loopback so the example can transmit and receive without
 * a link partenr or a physical loopback cable */
#define EXAMPLE_PHY_LOOPBACK_ENABLE 1U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
