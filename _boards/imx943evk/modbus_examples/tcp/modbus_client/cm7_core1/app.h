/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_clock.h"
#include "fsl_phyrtl8211f.h"
#include "lwip_netc_port.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_LED_RGPIO                 GPIO2
#define BOARD_LED_RGPIO_PIN1            31U
#define BOARD_LED_RGPIO_PIN2            31U

#define TCP_PORT_DEFAULT    502 /* TCP listening port. */
#define MAX_BUFFER_SIZE     ( 256 + 7 ) /* Must hold a complete Modbus TCP frame. */

/* ----------------------- MBAP Header --------------------------------------*/
#define HEADER_UID_POS          6
#define HEADER_LEN_POS          4
#define HEADER_FUNC_POS         7

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
        0x00, 0x00, 0xfb, 0xea, 0xaa, 0x02 \
    }
#endif

#define EXAMPLE_PHY_ADDRESS  ((NETC_PSI == kNETC_ENETC0PSI0) ? 0x5U : \
				((NETC_PSI == kNETC_ENETC1PSI0) ? 0x6U : 0x7U))
#define EXAMPLE_PHY_OPS      &g_app_phy_rtl8211f_ops
#define EXAMPLE_PHY_RESOURCE (void *)&g_phy_rtl8211f_resource
#define EXAMPLE_CLOCK_FREQ   CLOCK_GetRate(kCLOCK_Enet)
/*${macro:end}*/

extern phy_rtl8211f_resource_t g_phy_rtl8211f_resource;
extern const phy_operations_t g_app_phy_rtl8211f_ops;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
