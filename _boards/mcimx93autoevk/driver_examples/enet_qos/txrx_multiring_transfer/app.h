/*
 * Copyright 2023 NXP
 *
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
#define EXAMPLE_ENET_QOS_BASE      ENET_QOS
#define EXAMPLE_PHY_ADDR           (0x01U)
#define EXAMPLE_PHY_OPS            &phyrtl8211f_ops
#define EXAMPLE_PHY_RESOURCE       &g_phy_resource
#define ENET_QOS_SYSTEM_CLOCK_ROOT kCLOCK_Root_WakeupAxi
#define CORE_CLK_FREQ              CLOCK_GetIpFreq(ENET_QOS_SYSTEM_CLOCK_ROOT)
#define ENET_QOS_PTP_CLOCK_ROOT    kCLOCK_Root_EnetTimer2
#define ENET_PTP_REF_CLK           CLOCK_GetIpFreq(ENET_QOS_PTP_CLOCK_ROOT)
#define ENET_QOS_CLOCK_ROOT        kCLOCK_Root_Enet
#define ENET_QOS_CLOCK_GATE        kCLOCK_Enet_Qos
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
