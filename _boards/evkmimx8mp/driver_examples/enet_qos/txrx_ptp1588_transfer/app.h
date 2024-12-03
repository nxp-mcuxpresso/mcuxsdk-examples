/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_phyrtl8211f.h"
#include "fsl_enet_qos.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
extern phy_rtl8211f_resource_t g_phy_resource;
#define EXAMPLE_PHY_RESOURCE       &g_phy_resource
#define EXAMPLE_PHY_OPS            &phyrtl8211f_ops
#define EXAMPLE_ENET_QOS_BASE ENET_QOS
#define EXAMPLE_PHY_ADDR      0x01U
#define EXAMPLE_PHY_LOOPBACK_ENABLE 1U
/* ENET clock frequency. */
#define CORE_CLK_FREQ    250000000U
#define ENET_PTP_REF_CLK 100000000U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_UpdateENETModuleClock(enet_qos_mii_speed_t miiSpeed);
/*${prototype:end}*/

#endif /* _APP_H_ */
