/*
 * Copyright 2026 NXP
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

/* MSGINTR */
#define EXAMPLE_MSGINTR MSGINTR1
#define EXAMPLE_MSGINTR_IRQN MSGINTR1_IRQn

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
/*${prototype:end}*/

#endif /* _APP_H_ */
