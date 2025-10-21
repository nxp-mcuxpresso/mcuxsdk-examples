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

#include "FreeRTOS.h"
#include "task.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern phy_rtl8211f_resource_t g_phy_resource;
#define EXAMPLE_ENET_NUM    1U
#define EXAMPLE_NETC_HAS_NO_SWITCH 1U

#define SOEM_PORT_NAME       "enet1"
#define EXAMPLE_ENET         ENET1
#define EXAMPLE_PHY_ADDRESS  0x01U
#define EXAMPLE_PHY_OPS      &phyrtl8211f_ops
#define EXAMPLE_PHY_RESOURCE &g_phy_resource
#define EXAMPLE_CLOCK_FREQ   CLOCK_GetFreq(kCLOCK_EnetIpgClk)
#define EXAMPLE_PHY_INTERFACE_RGMII

/*${macro:end}*/

#ifndef PHY_AUTONEGO_TIMEOUT_COUNT
#define PHY_AUTONEGO_TIMEOUT_COUNT (300000)
#endif

#ifndef PHY_STABILITY_DELAY_US
#define PHY_STABILITY_DELAY_US (0)
#endif

#define ENET_RXBD_NUM          (4)
#define ENET_TXBD_NUM          (4)

#define ENET_RXBUFF_SIZE       (ENET_FRAME_MAX_FRAMELEN)
#define ENET_TXBUFF_SIZE       (ENET_FRAME_MAX_FRAMELEN)

/*! @brief GPT timer will be used to calculate the system time and delay */
#define OSAL_TIMER_IRQ_ID     GPT1_IRQn
#define OSAL_TIMER            GPT1
#define OSAL_TIMER_IRQHandler GPT1_IRQHandler
#define OSAL_TIMER_CLK_FREQ		(CLOCK_GetPllFreq(kCLOCK_SystemPll1Ctrl) / (CLOCK_GetRootPreDivider(kCLOCK_RootGpt1)) / \
     (CLOCK_GetRootPostDivider(kCLOCK_RootGpt1)) / 2) /* SYSTEM PLL1 DIV2 */
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
