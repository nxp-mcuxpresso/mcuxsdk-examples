/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "board.h"
#include "fsl_enet_qos.h"
#include "fsl_phylan8741.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
extern phy_lan8741_resource_t g_phy_resource;
#define EXAMPLE_ENET_QOS_BASE EMAC
#define EXAMPLE_PHY_ADDR      BOARD_EMAC_PHY_ADDRESS
#define EXAMPLE_PHY_OPS       &phylan8741_ops
#define EXAMPLE_PHY_RESOURCE  &g_phy_resource
#define EXAMPLE_MII_MODE      kENET_QOS_RmiiMode
#define CORE_CLK_FREQ         CLOCK_GetAipsPlatClkFreq()
#define ENET_PTP_REF_CLK      CLOCK_GetEmacTsClkFreq()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
