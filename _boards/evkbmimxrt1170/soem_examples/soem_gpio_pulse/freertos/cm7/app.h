/*
 * Copyright 2022-2024, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

#include "fsl_phyrtl8201.h"
#include "fsl_gpt.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* Ethernet configuration. */
extern phy_rtl8201_resource_t g_phy_resource;
#define EXAMPLE_ENET         ENET
#define EXAMPLE_PHY_ADDRESS  0x03U
#define EXAMPLE_PHY_OPS      &phyrtl8201_ops
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
#define OSAL_TIMER_CLK_FREQ   CLOCK_GetFreq(kCLOCK_OscRc48MDiv2)

#endif /* _APP_H_ */
