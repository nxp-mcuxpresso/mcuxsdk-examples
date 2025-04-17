/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "board.h"
#include "fsl_netc_endpoint.h"
#include "fsl_netc_switch.h"
#include "fsl_netc_tag.h"
#include "fsl_netc_mdio.h"
#include "fsl_phyrtl8211f.h"
#include "fsl_phydp8384x.h"
#include "fsl_phygpy215.h"
#include "fsl_msgintr.h"
#include "fsl_netc_phy_wrapper.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/*
 * Network interfaces mapping
 *
 * +--------------------------------------------------------------------------+
 * | MAC               | Instance | ETH  | index       | MII protocol         |
 * +--------------------------------------------------------------------------+
 * | switch mac0       | sw port0 | eth0 | port0/link0 | SGMII/RGMII/MII/RMII |
 * +--------------------------------------------------------------------------+
 * | switch mac1       | sw port1 | eth1 | port1/link1 | SGMII/RGMII/MII/RMII |
 * +--------------------------------------------------------------------------+
 * | switch mac2       | sw port2 | eth2 | port2/link2 | RGMII/RMII/RevMII    |
 * +--------------------------------------------------------------------------+
 * | switch pseudo mac | sw port3 |      |             |                      |
 * +--------------------------------------------------------------------------+
 * | enetc mac3        | enetc0   | eth2 | port3/link3 | RGMII/RMII/RevMII    |
 * +--------------------------------------------------------------------------+
 * | enetc mac4        | enetc1   | eth3 | port4/link4 | RGMII/RMII/RevMII    |
 * +--------------------------------------------------------------------------+
 * | enetc mac5        | enetc2   | eth4 | port5/link5 | RGMII/RMII/RevMII    |
 * +--------------------------------------------------------------------------+
 * | enetc pseudo mac  | enetc3   |      |             |                      |
 * +--------------------------------------------------------------------------+
 */

/* Port used */
#define EXAMPLE_EP0_PORT  0x00U
#define EXAMPLE_EP1_PORT  0x01U
#define EXAMPLE_SWT_PORT0 0x02U
#define EXAMPLE_SWT_PORT1 0x03U
#define EXAMPLE_SWT_PORT2 0x04U

/* End-point port */
#define EXAMPLE_EP_NUM 2U

#define EXAMPLE_EP_SI                      \
    {                                      \
        kNETC_ENETC1PSI0, kNETC_ENETC2PSI0 \
    }

/* Switch port */
#define EXAMPLE_SWT_MAX_PORT_NUM     3U
#define EXAMPLE_SWT_USED_PORT_BITMAP 0x7U
#define EXAMPLE_SWT_PSEUDO_PORT      0x3U
#define EXAMPLE_SWT_SI               kNETC_ENETC3PSI0

/*
 * To use 2.5G SGMII of switch port0 and port1 and on-board PHY
 * instead of 100M MII, add below definition in app.h.
 *
 * #define EXAMPLE_USE_PHY_GPY215 1U
 */

/* PHY: EP0, EP1, SWT_PORT0, SWT_PORT1, SWT_PORT2 */
#if defined(EXAMPLE_USE_PHY_GPY215)
#define EXAMPLE_PHY_ADDR \
    {                       \
        0x6U, 0x7U, 0xFU, 0x10U, 0x5U        \
    }
#else
#define EXAMPLE_PHY_ADDR \
    {                       \
        0x6U, 0x7U, 0x2U, 0x3U, 0x5U        \
    }
#endif

/* MSGINTR */
#define EXAMPLE_MSGINTR MSGINTR2

/* Buffer desciptor configuration. */
#define EXAMPLE_EP_RING_NUM          3U
#define EXAMPLE_EP_RXBD_NUM          8U
#define EXAMPLE_EP_TXBD_NUM          8U
#define EXAMPLE_EP_BD_ALIGN          128U
#define EXAMPLE_EP_BUFF_SIZE_ALIGN   64U
#define EXAMPLE_EP_RXBUFF_SIZE       1518U
#define EXAMPLE_EP_RXBUFF_SIZE_ALIGN SDK_SIZEALIGN(EXAMPLE_EP_RXBUFF_SIZE, EXAMPLE_EP_BUFF_SIZE_ALIGN)
#define EXAMPLE_EP_TEST_FRAME_SIZE   1000U

#define EXAMPLE_EP_TXFRAME_NUM 20U
/*${macro:end}*/

extern phy_handle_t g_phy_rtl8211;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
status_t APP_MDIO_Init(void);
status_t APP_PHY_Init(void);
status_t APP_PHY_GetLinkStatus(uint32_t port, bool *link);
status_t APP_PHY_GetLinkModeSpeedDuplex(uint32_t port, netc_hw_mii_mode_t *mode, netc_hw_mii_speed_t *speed, netc_hw_mii_duplex_t *duplex);
/*${prototype:end}*/

#endif /* _APP_H_ */
