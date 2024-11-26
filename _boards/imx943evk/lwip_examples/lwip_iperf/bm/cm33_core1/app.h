/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "hal_clock.h"
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
#ifndef configMAC_ADDR
#define configMAC_ADDR                     \
    {                                      \
        0x00, 0x00, 0xfa, 0xfa, 0xdd, 0x05 \
    }
#endif

/*
 * +--------------------------------------------------------------------------------+-------------------+-----------------------+
 * | enetc/switch    |   mac   | eth   |port    | link    | MII protocol            |  phy in imx943evk | phy addr in imx943evk |
 * +--------------------------------------------------------------------------------+-------------------+-----------------------+
 * | switch(enetc3)  |   mac0  | eth0  |port0   | link0   | SGMII/RGMII/MII/RMII    |     DP838491      | 0x6                   |
 * +--------------------------------------------------------------------------------+-------------------+-----------------------+
 * | switch(enetc3)  |   mac1  | eth1  |port1   | link1   | SGMII/RGMII/MII/RMII    |     DP838491      | 0x7                   |
 * +--------------------------------------------------------------------------------+-------------------+-----------------------+
 * | switch(enetc3)  |   mac2  | eth2  |port2   | link2   | RGMII/RMII/RevMII       |     RTL8211FDI    | 0x5                   |
 * +--------------------------------------------------------------------------------+-------------------------------------------+
 * | enetc0          |   mac3  | eth2  |port3   | link3   | RGMII/RMII/RevMII       |     RTL8211FDI    | 0x5                   |
 * +--------------------------------------------------------------------------------+-------------------------------------------+
 * | enetc1          |   mac4  | eth3  |port4   | link4   | RGMII/RMII/RevMII       |     RTL8211FDI    | 0x6                   |
 * +--------------------------------------------------------------------------------+-------------------------------------------+
 * | enetc2          |   mac5  | eth4  |port5   | link5   | RGMII/RMII/RevMII       |     RTL8211FDI    | 0x7                   |
 * +--------------------------------------------------------------------------------+-------------------------------------------+
 *
 */


#if SELECT_MAC_PORT == MAC_PORT0
#define EXAMPLE_PHY_ADDRESS  0x6
//#define EXAMPLE_PHY_OPS (&g_app_phy_dp838491_ops)
#define EXAMPLE_PHY_OPS (&g_app_phy_rtl8211f_ops)
#define EXAMPLE_PHY_RESOURCE ((void *)&g_phy_dp838491_resource)
#define NETC_SWITCH_MAC_PORT (kNETC_SWITCH0EthPort0)
#elif SELECT_MAC_PORT == MAC_PORT1
#define EXAMPLE_PHY_ADDRESS  0x7
//#define EXAMPLE_PHY_OPS (&g_app_phy_dp838491_ops)
#define EXAMPLE_PHY_OPS (&g_app_phy_rtl8211f_ops)
#define NETC_SWITCH_MAC_PORT (kNETC_SWITCH0EthPort1)
#define EXAMPLE_PHY_RESOURCE ((void *)&g_phy_dp838491_resource)
#elif SELECT_MAC_PORT == MAC_PORT2
#define EXAMPLE_PHY_ADDRESS  0x5
#define EXAMPLE_PHY_OPS (&g_app_phy_rtl8211f_ops)
#define NETC_SWITCH_MAC_PORT (kNETC_SWITCH0EthPort2)
#define EXAMPLE_PHY_RESOURCE ((void *)&g_phy_rtl8211f_resource)
#elif SELECT_MAC_PORT == MAC_PORT3
#define EXAMPLE_PHY_ADDRESS  0x5
#define EXAMPLE_PHY_OPS (&g_app_phy_rtl8211f_ops)
#define EXAMPLE_PHY_RESOURCE ((void *)&g_phy_rtl8211f_resource)
#elif SELECT_MAC_PORT == MAC_PORT4
#define EXAMPLE_PHY_ADDRESS  0x6
#define EXAMPLE_PHY_OPS (&g_app_phy_rtl8211f_ops)
#define EXAMPLE_PHY_RESOURCE ((void *)&g_phy_rtl8211f_resource)
#elif SELECT_MAC_PORT == MAC_PORT5
#define EXAMPLE_PHY_ADDRESS  0x7
#define EXAMPLE_PHY_OPS (&g_app_phy_rtl8211f_ops)
#define EXAMPLE_PHY_RESOURCE ((void *)&g_phy_rtl8211f_resource)
#endif

#define EXAMPLE_CLOCK_FREQ   HAL_ClockGetRate(hal_clock_enet)

/*${macro:end}*/

#if 0
extern phy_dp838491_resource_t g_phy_dp838491_resource;
extern const phy_operations_t g_app_phy_dp838491_ops;
#endif

extern phy_rtl8211f_resource_t g_phy_rtl8211f_resource;
extern const phy_operations_t g_app_phy_rtl8211f_ops;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
