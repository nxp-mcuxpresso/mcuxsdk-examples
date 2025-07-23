/*
 * Copyright 2025 NXP
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

/* Switch port used */
#define EXAMPLE_PORT_NUM 2U
#define EXAMPLE_SWT_USED_PORT_BITMAP 0xbU
#define EXAMPLE_SWT_PORT0 0x00U
#define EXAMPLE_SWT_PORT1 0x01U
#define EXAMPLE_SWT_SI    kNETC_ENETC3PSI0

#define EXAMPLE_SWT_PORT_MODE kNETC_SgmiiMode
#define EXAMPLE_SWT_PORT_SPEED kNETC_MiiSpeed1000M
#define EXAMPLE_SWT_PORT_DUPLEX kNETC_MiiFullDuplex

/*
 * To use 2.5G SGMII of switch port0 and port1 and on-board PHY
 * instead of 100M MII, add below definition in app.h.
 *
 * #define EXAMPLE_USE_PHY_GPY215 1U
 */

#define EXAMPLE_USE_PHY_GPY215 1U

/* PHY: SWT_PORT0, SWT_PORT1, SWT_PORT2 */
#if defined(EXAMPLE_USE_PHY_GPY215)
#define EXAMPLE_PHY_ADDR \
    {                       \
        0xFU, 0x10U        \
    }
#else
#define EXAMPLE_PHY_ADDR \
    {                       \
        0x2U, 0x3U        \
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

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
status_t APP_MDIO_Init(void);
status_t APP_PHY_Init(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
