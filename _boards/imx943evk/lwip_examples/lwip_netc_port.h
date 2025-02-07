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
 * +--------------------------------------------------------------------------------+
 * | enetc/switch    |   mac   | eth   |port    | link    | MII protocol            |
 * +--------------------------------------------------------------------------------+
 * | switch(enetc3)  |   mac0  | eth0  |port0   | link0   | SGMII/RGMII/MII/RMII    |
 * +--------------------------------------------------------------------------------+
 * | switch(enetc3)  |   mac1  | eth1  |port1   | link1   | SGMII/RGMII/MII/RMII    |
 * +--------------------------------------------------------------------------------+
 * | switch(enetc3)  |   mac2  | eth2  |port2   | link2   | RGMII/RMII/RevMII       |
 * +--------------------------------------------------------------------------------+
 * | enetc0          |   mac3  | eth2  |port3   | link3   | RGMII/RMII/RevMII       |
 * +--------------------------------------------------------------------------------+
 * | enetc1          |   mac4  | eth3  |port4   | link4   | RGMII/RMII/RevMII       |
 * +--------------------------------------------------------------------------------+
 * | enetc2          |   mac5  | eth4  |port5   | link5   | RGMII/RMII/RevMII       |
 * +--------------------------------------------------------------------------------+
 *
 */
#define MAC_PORT0 (0)
#define MAC_PORT1 (1)
#define MAC_PORT2 (2)
#define MAC_PORT3 (3)
#define MAC_PORT4 (4)
#define MAC_PORT5 (5)
//#define SELECT_MAC_PORT MAC_PORT0
//#define SELECT_MAC_PORT MAC_PORT1
//#define SELECT_MAC_PORT MAC_PORT2
//#define SELECT_MAC_PORT MAC_PORT3
//#define SELECT_MAC_PORT MAC_PORT4
#define SELECT_MAC_PORT MAC_PORT5

#if SELECT_MAC_PORT == MAC_PORT0
#define NETC_PSI         kNETC_ENETC3PSI0
#define NETC_MII_MODE    kNETC_SgmiiMode
#define NETC_MII_SPEED   kNETC_MiiSpeed2500M
#define NETC_MSGINTR     MSGINTR2
#define NETC_MSGINTR_IRQ MSGINTR2_IRQn
#elif SELECT_MAC_PORT == MAC_PORT1
#define NETC_PSI         kNETC_ENETC3PSI0
#define NETC_MII_MODE    kNETC_SgmiiMode
#define NETC_MII_SPEED   kNETC_MiiSpeed2500M
#define NETC_MSGINTR     MSGINTR2
#define NETC_MSGINTR_IRQ MSGINTR2_IRQn
#elif SELECT_MAC_PORT == MAC_PORT2
#define NETC_PSI         kNETC_ENETC3PSI0
#define NETC_MII_MODE    kNETC_RgmiiMode
#define NETC_MII_SPEED   kNETC_MiiSpeed1000M
#define NETC_MSGINTR     MSGINTR2
#define NETC_MSGINTR_IRQ MSGINTR2_IRQn
#elif SELECT_MAC_PORT == MAC_PORT3
#define NETC_PSI         kNETC_ENETC0PSI0
#define NETC_MII_MODE    kNETC_RgmiiMode
#define NETC_MII_SPEED   kNETC_MiiSpeed1000M
#define NETC_MSGINTR     MSGINTR2
#define NETC_MSGINTR_IRQ MSGINTR2_IRQn
#elif SELECT_MAC_PORT == MAC_PORT4
#define NETC_PSI         kNETC_ENETC1PSI0
#define NETC_MII_MODE    kNETC_RgmiiMode
#define NETC_MII_SPEED   kNETC_MiiSpeed1000M
#define NETC_MSGINTR     MSGINTR2
#define NETC_MSGINTR_IRQ MSGINTR2_IRQn
#elif SELECT_MAC_PORT == MAC_PORT5
#define NETC_PSI         kNETC_ENETC2PSI0
#define NETC_MII_MODE    kNETC_RgmiiMode
#define NETC_MII_SPEED   kNETC_MiiSpeed1000M
#define NETC_MSGINTR     MSGINTR2
#define NETC_MSGINTR_IRQ MSGINTR2_IRQn
#endif

#endif

