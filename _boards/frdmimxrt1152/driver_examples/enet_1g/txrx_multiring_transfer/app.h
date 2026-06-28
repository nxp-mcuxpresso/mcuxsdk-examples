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
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
extern phy_yt8521_resource_t g_phy_resource;
#define EXAMPLE_ENET         ENET_1G
#define EXAMPLE_PHY_ADDRESS  BOARD_ENET1_PHY_ADDRESS
#define EXAMPLE_PHY_OPS      &phyyt8521_ops
#define EXAMPLE_PHY_RESOURCE &g_phy_resource
#define EXAMPLE_CLOCK_FREQ   CLOCK_GetRootClockFreq(kCLOCK_Root_Bus)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
