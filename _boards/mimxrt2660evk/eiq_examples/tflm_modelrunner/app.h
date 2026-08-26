/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

/* XSPI1 -- APS256XXN 256Mb (32 MB) PSRAM on MIMXRT2660-EVK */
#define EXAMPLE_XSPI           MAIN__XSPI_1
#define EXAMPLE_XSPI_AMBA_BASE 0x88000000U
#define EXAMPLE_XSPI_CLOCK     kCLOCK_Xspi1

/* APS256XXN device size: 256 Mb / 8 = 32 MB */
#define DRAM_SIZE              0x2000000U

/* Tensor arena sizes for the modelrunner.
 * With TENSORARENA_DATA=1 the arena is a static global in .tensordata
 * (PSRAM non-cacheable region, nominally 16 MB available).
 * 10 MB leaves headroom after the heap carve-out. */
#define KTENSOR_ARENA_SIZE_MEM   (1024 * 1024 * 10)
#define KTENSOR_ARENA_SIZE_FLASH (1024 * 1024 * 10)

/* Phase 2 (HTTP): ENET / PHY macros -- active when MODELRUNNER_HTTP=1.
 * The YT8531 on this board is register-compatible with the YT8521 SDK driver.
 * Merge from _boards/mimxrt2660evk/lwip_examples/common/enet/app.h. */
#if defined(MODELRUNNER_HTTP) && MODELRUNNER_HTTP
#include "fsl_phyyt8521.h"

extern phy_yt8521_resource_t g_phy_resource;

#define EXAMPLE_ENET            COMM__ENET
#define EXAMPLE_PHY_ADDRESS     0x00U
#define EXAMPLE_PHY_OPS         &phyyt8521_ops
#define EXAMPLE_PHY_RESOURCE    &g_phy_resource
#define EXAMPLE_PHY_INTERFACE_RGMII 1
#define EXAMPLE_CLOCK_FREQ      CLOCK_GetRootClockFreq(kCLOCK_Root_COMM_comm_clk)
#define EXAMPLE_NETIF_INIT_FN   ethernetif0_init

/* IP address configuration (static fallback -- board uses DHCP at runtime). */
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

/*! @brief Stack size for the temporary lwIP init thread. */
#define INIT_THREAD_STACKSIZE  1024
/*! @brief Priority of the temporary lwIP init thread. */
#define INIT_THREAD_PRIO       DEFAULT_THREAD_PRIO

#endif /* MODELRUNNER_HTTP */

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
int64_t os_clock_now(void);
void BOARD_InitHardware(void);
void cleanCache_by_Addr(uint32_t addr, uint32_t size);
/*${prototype:end}*/

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _APP_H_ */
