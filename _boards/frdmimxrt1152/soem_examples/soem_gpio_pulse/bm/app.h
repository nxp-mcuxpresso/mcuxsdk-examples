/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

#include "board.h"
#include "fsl_phyyt8521.h"
#include "fsl_gpt.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* Ethernet configuration. */
extern phy_yt8521_resource_t g_phy_resource;
#define EXAMPLE_PHY_INTERFACE_RGMII 1
#define EXAMPLE_ENET         ENET_1G
#define EXAMPLE_PHY_ADDRESS  BOARD_ENET1_PHY_ADDRESS
#define EXAMPLE_PHY_OPS      &phyyt8521_ops
#define EXAMPLE_PHY_RESOURCE &g_phy_resource
#define EXAMPLE_CLOCK_FREQ   CLOCK_GetRootClockFreq(kCLOCK_Root_Bus)
/*${macro:end}*/

/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#ifndef PHY_AUTONEGO_TIMEOUT_COUNT
#define PHY_AUTONEGO_TIMEOUT_COUNT (100000)
#endif

#ifndef PHY_STABILITY_DELAY_US
#define PHY_STABILITY_DELAY_US (0U)
#endif

/*! @brief GPT timer will be used to calculate the system time and delay */
#define OSAL_TIMER_IRQ_ID     GPT2_IRQn
#define OSAL_TIMER            GPT2
#define OSAL_TIMER_IRQHandler GPT2_IRQHandler

/* Get source clock for GPT driver (GPT prescaler = 0) */
#define OSAL_TIMER_CLK_FREQ   CLOCK_GetFreq(kCLOCK_OscRc48MDiv2)

#endif /* _APP_H_ */
