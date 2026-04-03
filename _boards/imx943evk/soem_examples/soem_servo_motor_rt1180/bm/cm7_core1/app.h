/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "board.h"
#include "fsl_gpt.h"
#include "fsl_netc_endpoint.h"
#include "fsl_netc_switch.h"
#include "fsl_netc_mdio.h"
#include "fsl_phyrtl8211f.h"
#include "fsl_phyrtl8201.h"
#include "fsl_msgintr.h"
#include "fsl_rgpio.h"

#include "netc_ep/soem_netc_ep.h"
#include "netc_ep/netc_ep.h"
#include "soem_port.h"

#include "ethercattype.h"
#include "nicdrv.h"
#include "ethercatbase.h"
#include "ethercatmain.h"
#include "ethercatdc.h"
#include "ethercatcoe.h"
#include "ethercatfoe.h"
#include "ethercatconfig.h"
#include "ethercatprint.h"
/*${header:end}*/

#define CYCLE_SHIFT_NS  440000  // 440us
#define DC_FILTER_CNT   64

#define SOEM_PORT_NAME "J26"
#define MASTER_SLAVE_SYNC 1U

#define CLOCK_GRANULARITY_NS 50UL
#define CLOCK_GRANULARITY_FRE (1000000000UL/CLOCK_GRANULARITY_NS)
#define CLOCK_INCREASE_PER_SEC 1000000000UL 

#define EXAMPLE_EP_NUM        2U
#define EXAMPLE_NETC_HAS_NO_SWITCH 1U
#define EP0_PORT      0U
#define EP1_PORT      1U
#define SOEM_PORT 		EP0_PORT

/* Switch port */
#define EXAMPLE_SWT_MAX_PORT_NUM   3U
#define EXAMPLE_SWT_USED_PORT 0x2U
#define EXAMPLE_SWT_USED_PORT_BITMAP (1U << EXAMPLE_SWT_USED_PORT)
#define EXAMPLE_SWT_PSEUDO_PORT 0x3U
#define EXAMPLE_SWT_PORT0 0x02U
#define EXAMPLE_SWT_PORT1 0x03U
#define EXAMPLE_SWT_PORT2 0x04U

#define EP0_PHY_ADDR       0x6U

#define BOARD_LED_RGPIO          GPIO2
#define BOARD_LED_RGPIO_PIN      31U

#define MSGINTR       MSGINTR2

#define KNETC_EP_CONFIG_SI    kNETC_ENETC1PSI0

#define KNETC_HW_MII_MODE     kNETC_RgmiiMode

#define EP_NUM        3U

#define EP_RING_NUM          3U
#define EP_RXBD_NUM          8U
#define EP_TXBD_NUM          128U
#define EP_BD_ALIGN          128U
#define EP_BUFF_SIZE_ALIGN   64U
#define EP_RXBUFF_SIZE       1518U
#define EP_TXBUFF_SIZE       1518U
#define EP_RXBUFF_SIZE_ALIGN SDK_SIZEALIGN(EP_RXBUFF_SIZE, EP_BUFF_SIZE_ALIGN)
#define EP_TXBUFF_SIZE_ALIGN SDK_SIZEALIGN(EP_TXBUFF_SIZE, EP_BUFF_SIZE_ALIGN)

#define EP_TXFRAME_NUM 20U
#define PHY_PAGE_SELECT_REG 0x1FU /*!< The PHY page select register. */
#define TX_INTR_MSG_DATA  1U
#define RX_INTR_MSG_DATA  2U
#define TX_MSIX_ENTRY_IDX 0U
#define RX_MSIX_ENTRY_IDX 1U

#ifndef PHY_STABILITY_DELAY_US
#define PHY_STABILITY_DELAY_US (500000U)
#endif

/*! @brief GPT timer will be used to calculate the system time and delay */
#define OSAL_TIMER_IRQ_ID     GPT1_IRQn
#define OSAL_TIMER            GPT1
#define OSAL_TIMER_IRQHandler GPT1_IRQHandler
#define OSAL_TIMER_CLK_FREQ   CLOCK_GetRate(kCLOCK_Gpt1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void osal_timer_init(uint32_t priority);
uint64_t system_time64_ns(void);
void nsleep_to (uint64_t nsec_target);
void osal_gettime(struct timeval *current_time);
int if_port_init(void);
void update_master_clock(void);
status_t BOARD_InitHardware(void);
status_t NETC_EP_MDIO_Init(void);
status_t NETC_EP_PHY_Init(void);
status_t NETC_EP_PHY_GetLinkStatus(phy_handle_t *handler, bool *link);
/*${prototype:end}*/

#endif /* _APP_H_ */