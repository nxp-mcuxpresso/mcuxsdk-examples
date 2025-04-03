/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#include "fsl_netc_endpoint.h"
#include "fsl_netc_switch.h"
#include "fsl_netc_timer.h"
#include "fsl_netc_mdio.h"
#include "fsl_phyrtl8211f.h"
#include "fsl_phyrtl8201.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* Fixed definition */
#define EXAMPLE_EP_PHY_PORT   0U
#define EXAMPLE_SWT_PHY_PORT0 1U
#define EXAMPLE_SWT_PHY_PORT1 2U
#define EXAMPLE_SWT_PHY_PORT2 3U
#define EXAMPLE_SWT_PHY_PORT3 4U

#define EXAMPLE_EP0_PHY_ADDR       0x03U
#define EXAMPLE_SWT_PORT0_PHY_ADDR 0x02U
#define EXAMPLE_SWT_PORT1_PHY_ADDR 0x05U
#define EXAMPLE_SWT_PORT2_PHY_ADDR 0x04U
#define EXAMPLE_SWT_PORT3_PHY_ADDR 0x08U
#define EXAMPLE_NETC_FREQ          CLOCK_GetRootClockFreq(kCLOCK_Root_Netc)
#define EXAMPLE_NETC_TIMER_FREQ    CLOCK_GetRootClockFreq(kCLOCK_Root_Tmr_1588)

#define EXAMPLE_TIMER_ALARM1_TEST_SUPPORT false
#define EXAMPLE_TIMER_ALARM2_TEST_SUPPORT false

#define EXAMPLE_TIMER_FIPER1_TEST_SUPPORT true
#define EXAMPLE_TIMER_FIPER2_TEST_SUPPORT false
#define EXAMPLE_TIMER_FIPER3_TEST_SUPPORT false

/* User-defined definition */
#define EXAMPLE_PHY_AUTO_NEGO      false
#define EXAMPLE_PHY_DEFAULT_SPEED  kPHY_Speed100M
#define EXAMPLE_PHY_DEFAULT_DUPLEX kPHY_FullDuplex
#define EXAMPLE_MAC_INTERNAL_LOOP  false

#define EXAMPLE_EP_RING_NUM          4U
#define EXAMPLE_EP_RXBD_NUM          8U
#define EXAMPLE_EP_TXBD_NUM          8U
#define EXAMPLE_EP_BD_ALIGN          128U
#define EXAMPLE_EP_BUFF_SIZE_ALIGN   64U
#define EXAMPLE_EP_RXBUFF_SIZE       1518U
#define EXAMPLE_EP_RXBUFF_SIZE_ALIGN SDK_SIZEALIGN(EXAMPLE_EP_RXBUFF_SIZE, EXAMPLE_EP_BUFF_SIZE_ALIGN)
#define EXAMPLE_EP_TEST_FRAME_SIZE   1000U
#define EXAMPLE_EP_TXFRAME_NUM       20U
#define EXAMPLE_EP_POOL_RXBUFF_NUM   (EXAMPLE_EP_RXBD_NUM * 2U)

#define EXAMPLE_SWT_RING_NUM          3U
#define EXAMPLE_SWT_RXBD_NUM          8U
#define EXAMPLE_SWT_TXBD_NUM          8U
#define EXAMPLE_SWT_BD_ALIGN          128U
#define EXAMPLE_SWT_BUFF_SIZE_ALIGN   64U
#define EXAMPLE_SWT_RXBUFF_SIZE       1518U
#define EXAMPLE_SWT_RXBUFF_SIZE_ALIGN SDK_SIZEALIGN(EXAMPLE_SWT_RXBUFF_SIZE, EXAMPLE_SWT_BUFF_SIZE_ALIGN)
#define EXAMPLE_SWT_TEST_FRAME_SIZE   1000U
#define EXAMPLE_SWT_TXFRAME_NUM       20U
#define EXAMPLE_SWT_TEST_PORT_NUM     2U
#define EXAMPLE_SWT_TEST_PORT_ARRY \
    {                              \
        0, 1                       \
    }
#define EXAMPLE_SWT_TEST_PORT_BITMAP (kNETC_SWTPort1Bit | kNETC_SWTPort0Bit)
/*${macro:end}*/

extern phy_handle_t g_phy_rtl8211;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
typedef struct _app_phy_status
{
    bool link;
    phy_speed_t speed;
    phy_duplex_t duplex;
    netc_hw_mii_mode_t miiMode;
} app_phy_status_t;

void BOARD_InitHardware(void);
status_t APP_MDIO_Init(void);
status_t APP_PHY_Init(void);
status_t APP_PHY_WriteReadTest(void);
status_t APP_GetPhyStatus(uint8_t port, app_phy_status_t *phyStatus);
/*${prototype:end}*/

#endif /* _APP_H_ */
