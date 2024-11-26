/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "board.h"
#include "fsl_netc_endpoint.h"
#include "fsl_netc_mdio.h"
#include "fsl_phyrtl8211f.h"
#include "fsl_phyrtl8201.h"
#include "fsl_msgintr.h"
#include "fsl_rgpio.h"
#include "netc_ep/soem_netc_ep.h"
/*${header:end}*/

#define EP0_PORT      0U
#define EP1_PORT      1U
#define SOEM_PORT 		EP0_PORT

struct netc_ep_if_port if_port;

#define BOARD_LED_RGPIO          GPIO2
#define BOARD_LED_RGPIO_PIN      13U

#define MSGINTR       MSGINTR2

#define KNETC_EP_CONFIG_SI    kNETC_ENETC2PSI0

#define KNETC_HW_MII_MODE     kNETC_MiiMode

#if BOARD_IMX943_TYPE == BOARD_IMX943_EMULATOR
#define EP_NUM        1U

#define EP_SI                      \
    {                                      \
        kNETC_ENETC2PSI0 \
    }

#define EP_PHY_ADDR \
    {                       \
        0x1U          \
    }
#elif BOARD_IMX943_TYPE == BOARD_IMX943_EVK
#define EP_NUM        2U

#define EP_SI                      \
    {                                      \
        kNETC_ENETC0PSI0, kNETC_ENETC2PSI0 \
    }
#define EP_PHY_ADDR \
    {                       \
        0x1U, 0x8U          \
    }
#endif

#define EP_RING_NUM          3U
#define EP_RXBD_NUM          8U
#define EP_TXBD_NUM          8U
#define EP_BD_ALIGN          128U
#define EP_BUFF_SIZE_ALIGN   64U
#define EP_RXBUFF_SIZE       1518U
#define EP_TXBUFF_SIZE       1518U
#define EP_RXBUFF_SIZE_ALIGN SDK_SIZEALIGN(EP_RXBUFF_SIZE, EP_BUFF_SIZE_ALIGN)
#define EP_TXBUFF_SIZE_ALIGN SDK_SIZEALIGN(EP_TXBUFF_SIZE, EP_BUFF_SIZE_ALIGN)

#define EP_TXFRAME_NUM 20U
#define PHY_PAGE_SELECT_REG 0x1FU /*!< The PHY page select register. */
#define EP0_PHY_ADDR       0x00U
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
#define OSAL_TIMER_CLK_FREQ   HAL_ClockGetRate(hal_clock_gpt1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
status_t NETC_EP_MDIO_Init(void);
status_t NETC_EP_PHY_Init(void);
status_t NETC_EP_PHY_GetLinkStatus(phy_handle_t *handler, bool *link);
/*${prototype:end}*/

#endif /* _APP_H_ */