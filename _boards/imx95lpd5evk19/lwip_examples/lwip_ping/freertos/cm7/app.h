/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "hal_clock.h"
#include "fsl_phyaqr113c.h"
#include "fsl_phyrtl8211f.h"
#include "lwip_netc_port.h"
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

#define configMAC_ADDR                     \
    {                                      \
        0x00, 0x00, 0xfa, 0xfa, 0xdd, 0x05 \
    }

#define EXAMPLE_PHY_ADDRESS  ((NETC_PSI == kNETC_ENETC0PSI0) ? 0x1U : 0x8U)
#define EXAMPLE_PHY_OPS      ((NETC_PSI == kNETC_ENETC0PSI0) ? \
        &g_app_phy_rtl8211f_ops : &g_app_phy_aqr113c_ops)
#define EXAMPLE_PHY_RESOURCE ((NETC_PSI == kNETC_ENETC0PSI0) ? \
        (void *)&g_phy_rtl8211f_resource : (void *)&g_phy_aqr113c_resource)
#define EXAMPLE_CLOCK_FREQ   HAL_ClockGetIpFreq(hal_clock_enet)

/*${macro:end}*/

extern phy_aqr113c_resource_t g_phy_aqr113c_resource;
extern const phy_operations_t g_app_phy_aqr113c_ops;

extern phy_rtl8211f_resource_t g_phy_rtl8211f_resource;
extern const phy_operations_t g_app_phy_rtl8211f_ops;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
