/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _LWIP_NETC_PORT_H_
#define _LWIP_NETC_PORT_H_

#include "fsl_netc.h"
#include "fsl_netc_soc.h"

/*
 * Example supports two ports to select
 *
 * ENETC0
 * - NETC_PSI:       kNETC_ENETC0PSI0
 * - NETC_MII_MODE:  kNETC_RgmiiMode
 * - NETC_MII_SPEED: kNETC_MiiSpeed1000M
 *
 * ENETC1
 * - NETC_PSI:       kNETC_ENETC1PSI0
 * - NETC_MII_MODE:  kNETC_RgmiiMode
 * - NETC_MII_SPEED: kNETC_MiiSpeed1000M
 */
#define NETC_PSI         kNETC_ENETC0PSI0
#define NETC_MII_MODE    kNETC_RgmiiMode
#define NETC_MII_SPEED   kNETC_MiiSpeed1000M
#define NETC_MSGINTR     MSGINTR2
#define NETC_MSGINTR_IRQ IRQSTEER_0_IRQn

#endif

