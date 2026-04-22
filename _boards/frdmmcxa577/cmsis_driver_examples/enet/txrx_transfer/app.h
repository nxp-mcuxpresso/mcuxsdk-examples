/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#include "fsl_enet_cmsis.h"
#include "fsl_enet_phy_cmsis.h"
#include "board.h"
#include "fsl_enet.h"
#include "fsl_phytenbaset.h"

extern phy_tenbaset_resource_t g_phy_resource;

#define EXAMPLE_ENET     Driver_ETH_MAC0
#define EXAMPLE_ENET_PHY Driver_ETH_PHY0
#define EXAMPLE_ENET_BASE ENET0
#define EXAMPLE_CLOCK_FREQ CLOCK_GetCoreSysClkFreq()

void BOARD_InitHardware(void);
void BOARD_InitCmsisEnetResources(void);

#endif /* _APP_H_ */
