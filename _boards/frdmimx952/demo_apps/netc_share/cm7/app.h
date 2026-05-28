/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_clock.h"
#include "fsl_phyrtl8211f.h"
#include "fsl_netc_soc.h"
#include "netc_hw/fsl_netc_hw.h"
#include "app_srtm.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

/* @TEST_ANCHOR */

/* IP address configuration. */
#ifndef configIP_ADDR0
#define configIP_ADDR0 192
#endif
#ifndef configIP_ADDR1
#define configIP_ADDR1 168
#endif
#ifndef configIP_ADDR2
#define configIP_ADDR2 0
#endif
#ifndef configIP_ADDR3
#define configIP_ADDR3 102
#endif

/* Netmask configuration. */
#ifndef configNET_MASK0
#define configNET_MASK0 255
#endif
#ifndef configNET_MASK1
#define configNET_MASK1 255
#endif
#ifndef configNET_MASK2
#define configNET_MASK2 255
#endif
#ifndef configNET_MASK3
#define configNET_MASK3 0
#endif

/* Gateway address configuration. */
#ifndef configGW_ADDR0
#define configGW_ADDR0 192
#endif
#ifndef configGW_ADDR1
#define configGW_ADDR1 168
#endif
#ifndef configGW_ADDR2
#define configGW_ADDR2 0
#endif
#ifndef configGW_ADDR3
#define configGW_ADDR3 100
#endif

/* Ethernet configuration. */
#ifndef configMAC_ADDR
#define configMAC_ADDR                     \
    {                                      \
        0x00, 0x00, 0xfa, 0xfa, 0xdd, 0x05 \
    }
#endif
#define EXAMPLE_PHY_ADDRESS  0x1U
#define EXAMPLE_PHY_OPS      &g_app_phy_rtl8211f_ops
#define EXAMPLE_PHY_RESOURCE &g_phy_rtl8211f_resource
#define EXAMPLE_CLOCK_FREQ   CLOCK_GetRate(kCLOCK_enet)
/*${macro:end}*/

extern phy_rtl8211f_resource_t g_phy_rtl8211f_resource;
extern const phy_operations_t g_app_phy_rtl8211f_ops;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
status_t APP_PHY_Init(phy_handle_t *phy_handle, const phy_config_t *config);
status_t APP_NETC_PreinitVsi(netc_enetc_hw_t *hw, netc_hw_si_idx_t si);
/*${prototype:end}*/

#endif /* _APP_H_ */
