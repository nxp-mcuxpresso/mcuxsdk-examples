/*
 * Copyright 2020, 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_INIT_H_
#define _BOARD_INIT_H_

#ifdef MCMGR_USED
#include "mcmgr.h"
#endif /* MCMGR_USED */

#ifdef ENABLE_ETHERNET_PHY
#if BOARD_NETWORK_USE_100M_ENET_PORT
#include "fsl_phyrtl8201.h"
#else
#include "fsl_phyrtl8211f.h"
#endif
#endif /* ENABLE_ETHERNET_PHY */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifdef RPMSG_USED
#define RPMSG_LITE_LINK_ID (RL_PLATFORM_IMXRT1170_M7_M4_LINK_ID)
#endif /* RPMSG_USED */

#ifdef BOOT_SECONDARY_CORE
/* Address of memory, from which the secondary core will boot */
#ifndef CORE1_BOOT_ADDRESS
#define CORE1_BOOT_ADDRESS 0x20200000
#endif

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t Image$$CORE1_REGION$$Base;
extern uint32_t Image$$CORE1_REGION$$Length;
#define CORE1_IMAGE_START &Image$$CORE1_REGION$$Base
#elif defined(__ICCARM__)
extern unsigned char core1_image_start[];
#define CORE1_IMAGE_START core1_image_start
#elif (defined(__GNUC__)) && (!defined(__MCUXPRESSO))
extern const char core1_image_start[];
extern const char *core1_image_end;
extern int core1_image_size;
#define CORE1_IMAGE_START ((void *)core1_image_start)
#define CORE1_IMAGE_SIZE  ((void *)core1_image_size)
#endif
#endif /* BOOT_SECONDARY_CORE */

#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void);
#endif /* CORE1_IMAGE_COPY_TO_RAM */

#ifdef ENABLE_ETHERNET_PHY
#if BOARD_NETWORK_USE_100M_ENET_PORT
extern phy_rtl8201_resource_t g_phy_resource;
#define EXAMPLE_ENET ENET
/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS
/* PHY operations. */
#define EXAMPLE_PHY_OPS &phyrtl8201_ops
/* ENET instance select. */
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#else
extern phy_rtl8211f_resource_t g_phy_resource;
#define EXAMPLE_ENET          ENET_1G
/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS   BOARD_ENET1_PHY_ADDRESS
/* PHY operations. */
#define EXAMPLE_PHY_OPS       &phyrtl8211f_ops
/* ENET instance select. */
#define EXAMPLE_NETIF_INIT_FN ethernetif1_init
#endif

/* PHY resource. */
#define EXAMPLE_PHY_RESOURCE &g_phy_resource

/* ENET clock frequency. */
#define EXAMPLE_CLOCK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_Bus)
#endif /* ENABLE_ETHERNET_PHY */

void BOARD_Init();

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_INIT_H_ */
