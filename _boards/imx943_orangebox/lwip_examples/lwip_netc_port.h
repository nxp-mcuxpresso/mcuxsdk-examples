/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _LWIP_NETC_PORT_H_
#define _LWIP_NETC_PORT_H_

#include "fsl_netc.h"
#include "fsl_netc_soc.h"

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

/*
 * Example supports 2 ports to select
 *
 * ENETC0
 * - NETC_PSI:       kNETC_ENETC0PSI0
 * ENETC1
 * - NETC_PSI:       kNETC_ENETC1PSI0
 */
#define NETC_PSI         kNETC_ENETC0PSI0

#define NETC_MII_MODE    kNETC_RgmiiMode
#define NETC_MII_SPEED   kNETC_MiiSpeed1000M
#define NETC_MSGINTR     MSGINTR2
#if __CORTEX_M == 7
#define NETC_MSGINTR_IRQ IRQSTEERM7_CH6_IRQn
#else
#define NETC_MSGINTR_IRQ MSGINTR2_IRQn
#endif

#endif

