/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_phyrtl8211f.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
extern phy_rtl8211f_resource_t g_phy_resource;
#define EXAMPLE_ENET         ENET
#define EXAMPLE_PHY_ADDRESS  (0x02U)
#define EXAMPLE_PHY_OPS      &phyrtl8211f_ops
#define EXAMPLE_PHY_RESOURCE &g_phy_resource

/* ENET clock frequency. */
#define EXAMPLE_CLOCK_ROOT kCLOCK_Root_WakeupAxi
#define EXAMPLE_CLOCK_FREQ CLOCK_GetIpFreq(EXAMPLE_CLOCK_ROOT)
#define ENETREF_CLOCK_ROOT kCLOCK_Root_EnetRef
#define ENET_CLOCK_GATE    kCLOCK_Enet1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
