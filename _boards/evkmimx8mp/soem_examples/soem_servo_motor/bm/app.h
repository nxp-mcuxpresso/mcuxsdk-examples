/*
 * Copyright 2022-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

#include "fsl_phyrtl8211f.h"
#include "fsl_gpt.h"

#include "ethercattype.h"
#include "nicdrv.h"
#include "ethercatbase.h"
#include "ethercatmain.h"
#include "ethercatdc.h"
#include "ethercatcoe.h"
#include "ethercatfoe.h"
#include "ethercatconfig.h"
#include "ethercatprint.h"
#include "enet/soem_enet.h"
#include "enet/enet.h"
#include "soem_port.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#define EXAMPLE_ENET_NUM    1U
#define EXAMPLE_NETC_HAS_NO_SWITCH 1U

#define EXAMPLE_ENET         ENET1
#define EXAMPLE_PHY_ADDRESS  0x01U
#define EXAMPLE_CLOCK_FREQ   CLOCK_GetFreq(kCLOCK_EnetIpgClk)
#define EXAMPLE_PHY_INTERFACE_RGMII

#define CLOCK_GRANULARITY_NS 40UL
#define CLOCK_GRANULARITY_FRE (1000000000UL/CLOCK_GRANULARITY_NS)
#define CLOCK_INCREASE_PER_SEC 1000000000UL 
/*${macro:end}*/

#ifndef PHY_AUTONEGO_TIMEOUT_COUNT
#define PHY_AUTONEGO_TIMEOUT_COUNT (800000U)
#endif

#ifndef PHY_STABILITY_DELAY_US
#define PHY_STABILITY_DELAY_US (500000U)
#endif

#define SOEM_PORT_NAME "enet1"
#define ENET_RXBD_NUM          (1)
#define ENET_TXBD_NUM          (1)

#define ENET_RXBUFF_SIZE       (ENET_FRAME_MAX_FRAMELEN)
#define ENET_TXBUFF_SIZE       (ENET_FRAME_MAX_FRAMELEN)

/*! @brief GPT timer will be used to calculate the system time and delay */
#define OSAL_TIMER_IRQ_ID     GPT1_IRQn
#define OSAL_TIMER            GPT1
#define OSAL_TIMER_IRQHandler GPT1_IRQHandler
#define OSAL_TIMER_CLK_FREQ                                                                \
    (CLOCK_GetPllFreq(kCLOCK_SystemPll1Ctrl) / (CLOCK_GetRootPreDivider(kCLOCK_RootGpt1)) / \
     (CLOCK_GetRootPostDivider(kCLOCK_RootGpt1)) / 2) /* SYSTEM PLL1 DIV2 */
     
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
uint64_t gettime(void);
void nsleep_to (uint64_t nsec_target);
void osal_gettime(struct timeval *current_time);
void osal_timer_init(uint32_t priority);
int if_port_init(void);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
