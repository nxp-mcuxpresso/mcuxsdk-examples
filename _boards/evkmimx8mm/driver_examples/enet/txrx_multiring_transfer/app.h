/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_phyar8031.h"
#include "fsl_phyrtl8211f.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#ifndef EXAMPLE_ENET_PHY_PHYAR8031
#define EXAMPLE_ENET_PHY_PHYAR8031 0U
#endif

#if EXAMPLE_ENET_PHY_PHYAR8031
#define EXAMPLE_PHY_OPS &phyar8031_ops
extern phy_ar8031_resource_t g_phy_resource;
#else
#define EXAMPLE_PHY_OPS &phyrtl8211f_ops
extern phy_rtl8211f_resource_t g_phy_resource;
#endif /* EXAMPLE_ENET_PHY_PHYAR8031 */

#define EXAMPLE_ENET         ENET1
#define EXAMPLE_PHY_ADDRESS  0U
#define EXAMPLE_PHY_RESOURCE &g_phy_resource
#define EXAMPLE_CLOCK_FREQ   CLOCK_GetFreq(kCLOCK_EnetIpgClk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
