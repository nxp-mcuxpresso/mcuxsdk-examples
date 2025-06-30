/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_netc_endpoint.h"
#include "fsl_netc_switch.h"
#include "fsl_netc_mdio.h"
#include "fsl_phyyt8521.h"
#include "fsl_msgintr.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_SWT_PORT0 0x01U
#define EXAMPLE_SWT_PORT2 0x03U

#define EXAMPLE_SWT_PORT0_PHY_ADDR 0x05U
#define EXAMPLE_SWT_PORT2_PHY_ADDR 0x04U
#define EXAMPLE_NETC_FREQ          CLOCK_GetRootClockFreq(kCLOCK_Root_Netc)

/* @TEST_ANCHOR */

/* Ethernet configuration. */

#define configMAC_ADDR                     \
    {                                      \
        0x00, 0x00, 0xfa, 0xfa, 0xdd, 0x05 \
    }

#define EXAMPLE_PHY_ADDRESS  EXAMPLE_SWT_PORT0_PHY_ADDR
#define EXAMPLE_PHY_OPS      &phyyt8521_ops
#define EXAMPLE_PHY_RESOURCE &g_phy_yt8521_resource
#define EXAMPLE_CLOCK_FREQ   EXAMPLE_NETC_FREQ

#define EXAMPLE_PHY_INT_PORT BOARD_INITNETPINS_ETH0_INT_B_PERIPHERAL
#define EXAMPLE_PHY_INT_PIN  BOARD_INITNETPINS_ETH0_INT_B_CHANNEL
/*${macro:end}*/

static phy_yt8521_resource_t g_phy_yt8521_resource;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
