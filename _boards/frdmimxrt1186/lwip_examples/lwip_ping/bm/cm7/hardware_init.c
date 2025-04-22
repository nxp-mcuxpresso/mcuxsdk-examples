/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${macro:start}*/
#define PHY_PAGE_SELECT_REG 0x1FU /*!< The PHY page select register. */
/*!< PHY reset pins. */
#define EXAMPLE_SWT_PORT0_PHY_RESET_PIN RGPIO1, 15
/*${macro:end}*/

/*${variable:start}*/

/* PHY operation. */
static netc_mdio_handle_t s_emdio_handle;
static phy_handle_t g_phy_yt8521;
/*${variable:end}*/

/*${function:start}*/

static status_t APP_EMDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return NETC_MDIOWrite(&s_emdio_handle, phyAddr, regAddr, data);
}

static status_t APP_EMDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return NETC_MDIORead(&s_emdio_handle, phyAddr, regAddr, pData);
}

static status_t APP_PHY_SetPort(uint32_t port, phy_config_t *phyConfig)
{
    status_t result = kStatus_Success;

    g_phy_yt8521_resource.write = APP_EMDIOWrite;
    g_phy_yt8521_resource.read  = APP_EMDIORead;

    result = PHY_Init(&g_phy_yt8521, phyConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    return PHY_EnableLoopback(&g_phy_yt8521, kPHY_LocalLoop, phyConfig->speed, true);
}

status_t APP_PHY_Init(void)
{
    status_t result            = kStatus_Success;
    phy_config_t phyyt8521Config = {
        .autoNeg   = false,
        .speed     = kPHY_Speed1000M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phyyt8521_ops,
    };

    RGPIO_PinWrite(EXAMPLE_SWT_PORT0_PHY_RESET_PIN, 0);
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));

    RGPIO_PinWrite(EXAMPLE_SWT_PORT0_PHY_RESET_PIN, 1);
    SDK_DelayAtLeastUs(150000, CLOCK_GetFreq(kCLOCK_CpuClk));

    /* Initialize PHY for switch port0. */
    phyyt8521Config.resource = &g_phy_yt8521_resource;
    phyyt8521Config.phyAddr  = BOARD_SWT_PORT0_PHY_ADDR;
    result                   = APP_PHY_SetPort(EXAMPLE_SWT_PORT0, &phyyt8521Config);
    if (result != kStatus_Success)
    {
        return result;
    }

    return result;
}

static void APP_MDIO_Init(void)
{
    status_t result = kStatus_Success;

    netc_mdio_config_t mdioConfig = {
        .mdio =
            {
                .type = kNETC_EMdio,
            },
        .isPreambleDisable = false,
        .isNegativeDriven  = false,
        .srcClockHz        = EXAMPLE_NETC_FREQ,
    };

    mdioConfig.mdio.port = (netc_hw_eth_port_idx_t)kNETC_ENETC0EthPort;
    result               = NETC_MDIOInit(&s_emdio_handle, &mdioConfig);
    while (result != kStatus_Success)
    {
        // failed
    }
}

status_t APP_PHY_GetLinkStatus(uint32_t port, bool *link)
{
    return PHY_GetLinkStatus(&g_phy_yt8521, link);
}

status_t APP_PHY_GetLinkModeSpeedDuplex(uint32_t port,
                                        netc_hw_mii_mode_t *mode,
                                        netc_hw_mii_speed_t *speed,
                                        netc_hw_mii_duplex_t *duplex)
{
    switch (port)
    {
        case EXAMPLE_SWT_PORT0:
            *mode = kNETC_RgmiiMode;
            break;
        case EXAMPLE_SWT_PORT2:
            *mode = kNETC_RgmiiMode;
            break;
        default:
            assert(false);
            break;
    }

    return PHY_GetLinkSpeedDuplex(&g_phy_yt8521, (phy_speed_t *)speed, (phy_duplex_t *)duplex);
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitNETPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* RGMII mode */
    BLK_CTRL_WAKEUPMIX->NETC_LINK_CFG[0] = BLK_CTRL_WAKEUPMIX_NETC_LINK_CFG_MII_PROT(2);

    /* Unlock the IERB. It will warm reset whole NETC. */
    NETC_PRIV->NETCRR &= ~NETC_PRIV_NETCRR_LOCK_MASK;
    while ((NETC_PRIV->NETCRR & NETC_PRIV_NETCRR_LOCK_MASK) != 0U)
    {
    }

    /* Set PHY address in IERB to use MAC port MDIO, otherwise the access will be blocked. */
    NETC_IERB->L0BCR = NETC_IERB_L0BCR_MDIO_PHYAD_PRTAD(EXAMPLE_SWT_PORT0_PHY_ADDR);

    /* Set the access attribute, otherwise MSIX access will be blocked. */
    NETC_IERB->ARRAY_NUM_RC[0].RCMSIAMQR &= ~(7U << 27);
    NETC_IERB->ARRAY_NUM_RC[0].RCMSIAMQR |= (1U << 27);

    /* Lock the IERB. */
    NETC_PRIV->NETCRR |= NETC_PRIV_NETCRR_LOCK_MASK;
    while ((NETC_PRIV->NETCSR & NETC_PRIV_NETCSR_STATE_MASK) != 0U)
    {
    }

    APP_MDIO_Init();
    APP_PHY_Init();
    status_t APP_PHY_GetLinkStatus(uint32_t port, bool *link);
    status_t APP_PHY_GetLinkModeSpeedDuplex(uint32_t port,
                                        netc_hw_mii_mode_t *mode,
                                        netc_hw_mii_speed_t *speed,
                                        netc_hw_mii_duplex_t *duplex);
}
/*${function:end}*/
