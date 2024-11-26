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
#include "fsl_netc_switch.h"
#include "fsl_netc_mdio.h"
#include "fsl_phyrtl8211f.h"
#include "fsl_phyrtl8201.h"
#include "fsl_msgintr.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

/*
 * +--------------------------------------------------------------------------------+
 * | enetc/switch    |   mac   | eth   |port    | link    | MII protocol            |
 * +--------------------------------------------------------------------------------+
 * | switch(enetc3)  |   mac0  | eth0  |port0   | link0   | RGMII/MII/RMII          |
 * +--------------------------------------------------------------------------------+
 * | switch(enetc3)  |   mac1  | eth1  |port1   | link1   | RGMII/MII/RMII          |
 * +--------------------------------------------------------------------------------+
 * | switch(enetc3)  |   mac2  | eth2  |port2   | link2   | RGMII/RMII/RevMII       |
 * +--------------------------------------------------------------------------------+
 * | enetc0          |   mac3  | eth2  |port3   | link3   | RGMII/RMII/RevMII       |
 * +--------------------------------------------------------------------------------+
 * | enetc1          |   mac4  | eth3  |port4   | link4   | RGMII/RMII/RevMII       |
 * +--------------------------------------------------------------------------------+
 * | enetc2          |   mac5  | eth4  |port5   | link5   | RGMII/RMII/RevMII       |
 * +--------------------------------------------------------------------------------+
 *
 */
/* Ethernet port identifier. */
#define EXAMPLE_PORT_NUM 6
#define EXAMPLE_PORTS { kNETC_ENETC2EthPort, kNETC_ENETC1EthPort, kNETC_ENETC0EthPort, kNETC_SWITCH0EthPort0, kNETC_SWITCH0EthPort1, kNETC_SWITCH0EthPort2}

/* endpoint or switch port */
#define EXAMPLE_EP0_PORT      0x00U
#define EXAMPLE_EP1_PORT      0x01U
#define EXAMPLE_EP2_PORT      0x02U
#define EXAMPLE_SWT_PORT0 0x03U
#define EXAMPLE_SWT_PORT1 0x04U
#define EXAMPLE_SWT_PORT2 0x05U

#if BOARD_IMX943_TYPE == BOARD_IMX943_EMULATOR
#define EXAMPLE_EP_NUM        1U

#define EXAMPLE_EP_SI                      \
    {                                      \
        kNETC_ENETC2PSI0 \
    }

#define EXAMPLE_EP_PHY_ADDR \
    {                       \
        BOARD_EP0_PHY_ADDR          \
    }

#elif BOARD_IMX943_TYPE == BOARD_IMX943_EVK
#define EXAMPLE_EP_NUM        2U

#define EXAMPLE_EP_SI                      \
    {                                      \
        kNETC_ENETC0PSI0, kNETC_ENETC2PSI0 \
    }
#define EXAMPLE_EP_PHY_ADDR \
    {                       \
        0x1U, 0x8U          \
    }

#endif

#define EXAMPLE_SWT_SI kNETC_ENETC3PSI0

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

#define EXAMPLE_SWT_MAX_PORT_NUM   3U
#if !defined(EXAMPLE_SWT_USED_PORT_BITMAP)
#define EXAMPLE_SWT_USED_PORT_BITMAP 0x4U /*! Enabled Switch port bit map, bit n represents port n. Only enabled switch port2(0x4 = 0b 0100 - the second bit)  */
#endif
/*${macro:end}*/

extern phy_handle_t g_phy_rtl8211;

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
