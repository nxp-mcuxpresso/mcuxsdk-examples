/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_clock.h"
#include "fsl_phy.h"
#include "fsl_netc.h"
#include "fsl_netc_soc.h"
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

/*
 * ENETC3 dummy definitions
 */
#define EXAMPLE_PHY_ADDRESS  0x0U
#define EXAMPLE_PHY_OPS      &g_app_phy_dummy_ops
#define EXAMPLE_PHY_RESOURCE NULL
#define EXAMPLE_CLOCK_FREQ   CLOCK_GetRate(kCLOCK_Enet)

/* Switch port used */
#define EXAMPLE_SWT_PORT_NUM 2U
#define EXAMPLE_SWT_USED_PORT_BITMAP 0xbU
#define EXAMPLE_SWT_PORT0 0x00U
#define EXAMPLE_SWT_PORT1 0x01U
#define EXAMPLE_SWT_PORT_MODE kNETC_SgmiiMode
#define EXAMPLE_SWT_PORT_SPEED kNETC_MiiSpeed1000M
#define EXAMPLE_SWT_PORT_DUPLEX kNETC_MiiFullDuplex
#define EXAMPLE_SWT_PHY_ADDR \
    {                        \
        0xFU, 0x10U          \
    }

/*${macro:end}*/

extern const phy_operations_t g_app_phy_dummy_ops;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
status_t APP_NETC_PreinitVsi(netc_enetc_hw_t *hw, netc_hw_si_idx_t si);
/*${prototype:end}*/

#endif /* _APP_H_ */
