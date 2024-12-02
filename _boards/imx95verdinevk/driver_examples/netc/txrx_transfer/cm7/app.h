/*
 * Copyright 2022-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_netc_endpoint.h"
#include "fsl_netc_mdio.h"
#include "fsl_netc_phy_wrapper.h"
#include "fsl_phyrtl8211f.h"
#include "fsl_phyaqr113c.h"
#include "fsl_msgintr.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

/* Ethernet port identifier. */
#define EXAMPLE_EP_NUM        2U
#define EXAMPLE_EP0_PORT      0U
#define EXAMPLE_EP1_PORT      1U

#define EXAMPLE_EP_SI                      \
    {                                      \
        kNETC_ENETC0PSI0, kNETC_ENETC2PSI0 \
    }
#define EXAMPLE_EP_PHY_ADDR \
    {                       \
        0x1U, 0x8U          \
    }
#define EXAMPLE_MSGINTR       MSGINTR2

/* Buffer desciptor configuration. */
#define EXAMPLE_EP_RING_NUM          3U
#define EXAMPLE_EP_RXBD_NUM          8U
#define EXAMPLE_EP_TXBD_NUM          8U
#define EXAMPLE_EP_BD_ALIGN          128U
#define EXAMPLE_EP_BUFF_SIZE_ALIGN   64U
#define EXAMPLE_EP_RXBUFF_SIZE       1518U
#define EXAMPLE_EP_RXBUFF_SIZE_ALIGN SDK_SIZEALIGN(EXAMPLE_EP_RXBUFF_SIZE, EXAMPLE_EP_BUFF_SIZE_ALIGN)
#define EXAMPLE_EP_TEST_FRAME_SIZE   1000U
#define EXAMPLE_EP_TXFRAME_NUM 20U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
status_t APP_MDIO_Init(void);
status_t APP_PHY_Init(void);
status_t APP_PHY_GetLinkStatus(uint32_t port, bool *link);
status_t APP_PHY_GetLinkModeSpeedDuplex(uint32_t port, netc_hw_mii_mode_t *mode, netc_hw_mii_speed_t *speed, netc_hw_mii_duplex_t *duplex);
/*${prototype:end}*/

#endif /* _APP_H_ */
