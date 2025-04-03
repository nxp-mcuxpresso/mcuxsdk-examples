/*
 * Copyright 2021-2022 NXP
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
/*${macro:end}*/

/*${variable:start}*/
/* PHY operation. */
#ifdef EXAMPLE_PHY_USE_PORT_MDIO
static netc_mdio_handle_t s_mdio_handle[5];
#else
static netc_mdio_handle_t s_emdio_handle;
#endif
static phy_rtl8211f_resource_t s_phy_resource[5];
static phy_handle_t s_phy_handle[5];
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    if (NVIC_GetEnableIRQ(GPIO1_0_IRQn))
    {
        NVIC_DisableIRQ(GPIO1_0_IRQn);
    }
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* RMII mode, link0 -> switch port0, link4 -> EP */
    BLK_CTRL_WAKEUPMIX->NETC_LINK_CFG[0] = BLK_CTRL_WAKEUPMIX_NETC_LINK_CFG_MII_PROT(1);
    BLK_CTRL_WAKEUPMIX->NETC_LINK_CFG[4] = BLK_CTRL_WAKEUPMIX_NETC_LINK_CFG_MII_PROT(1);

    /* RGMII mode, link1 -> switch port1. */
    BLK_CTRL_WAKEUPMIX->NETC_LINK_CFG[1] = BLK_CTRL_WAKEUPMIX_NETC_LINK_CFG_MII_PROT(2);

    /* Output reference clock for RMII */
    BLK_CTRL_WAKEUPMIX->NETC_PORT_MISC_CFG |= BLK_CTRL_WAKEUPMIX_NETC_PORT_MISC_CFG_PORT0_RMII_REF_CLK_DIR_MASK |
                                              BLK_CTRL_WAKEUPMIX_NETC_PORT_MISC_CFG_PORT4_RMII_REF_CLK_DIR_MASK;

    /* Set PHY address in IERB to use MAC port MDIO, otherwise the access will be blocked */
    NETC_PRIV->NETCRR &= ~NETC_PRIV_NETCRR_LOCK_MASK;
    while ((NETC_PRIV->NETCRR & NETC_PRIV_NETCRR_LOCK_MASK) != 0U)
    {
    }

#ifdef EXAMPLE_PHY_USE_PORT_MDIO
    NETC_IERB->L0BCR = NETC_IERB_L0BCR_MDIO_PHYAD_PRTAD(EXAMPLE_SWT_PORT0_PHY_ADDR);
    NETC_IERB->L1BCR = NETC_IERB_L0BCR_MDIO_PHYAD_PRTAD(EXAMPLE_SWT_PORT1_PHY_ADDR);
    NETC_IERB->L4BCR = NETC_IERB_L0BCR_MDIO_PHYAD_PRTAD(EXAMPLE_EP0_PHY_ADDR);
#endif

    /* Set the access attribute, otherwise MSIX access will be blocked. */
    NETC_IERB->ARRAY_NUM_RC[0].RCMSIAMQR &= ~(7U << 27);
    NETC_IERB->ARRAY_NUM_RC[0].RCMSIAMQR |= (1U << 27);

    /* Lock the IERB. */
    NETC_PRIV->NETCRR |= NETC_PRIV_NETCRR_LOCK_MASK;
    while ((NETC_PRIV->NETCSR & NETC_PRIV_NETCSR_STATE_MASK) != 0U)
    {
    }
}

status_t APP_MDIO_Init(void)
{
    status_t result = kStatus_Success;

    netc_mdio_config_t mdioConfig = {
        .isPreambleDisable = false,
        .isNegativeDriven  = false,
        .srcClockHz        = EXAMPLE_NETC_FREQ,
    };

#ifdef EXAMPLE_PHY_USE_PORT_MDIO
    /* Usually should call EP_Init/SWT_Init then init port MDIO, here just an quick enablement example. */
    NETC_F2_PCI_HDR_TYPE0->PCI_CFH_CMD |=
        (ENETC_PCI_TYPE0_PCI_CFH_CMD_MEM_ACCESS_MASK | ENETC_PCI_TYPE0_PCI_CFH_CMD_BUS_MASTER_EN_MASK);
    NETC_F3_PCI_HDR_TYPE0->PCI_CFH_CMD |=
        (ENETC_PCI_TYPE0_PCI_CFH_CMD_MEM_ACCESS_MASK | ENETC_PCI_TYPE0_PCI_CFH_CMD_BUS_MASTER_EN_MASK);

    for (int i = 0U; i < 5U; i++)
    {
        mdioConfig.mdio.port = (netc_hw_eth_port_idx_t)((uint32_t)kNETC_ENETC0EthPort + i);
        result               = NETC_MDIOInit(&s_mdio_handle[i], &mdioConfig);
        if (result != kStatus_Success)
        {
            return result;
        }
    }
#else
    mdioConfig.mdio.type = kNETC_EMdio;
    result               = NETC_MDIOInit(&s_emdio_handle, &mdioConfig);
#endif

    return result;
}

#ifdef EXAMPLE_PHY_USE_PORT_MDIO
static status_t APP_PMDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    status_t result = kStatus_Success;
    netc_mdio_handle_t *mdioHandle;

    switch (phyAddr)
    {
        case EXAMPLE_EP0_PHY_ADDR:
            mdioHandle = &s_mdio_handle[0];
            break;
        case EXAMPLE_SWT_PORT0_PHY_ADDR:
            mdioHandle = &s_mdio_handle[1];
            break;
        case EXAMPLE_SWT_PORT1_PHY_ADDR:
            mdioHandle = &s_mdio_handle[2];
            break;
        case EXAMPLE_SWT_PORT2_PHY_ADDR:
            mdioHandle = &s_mdio_handle[3];
            break;
        case EXAMPLE_SWT_PORT3_PHY_ADDR:
            mdioHandle = &s_mdio_handle[4];
            break;
        default:
            result = kStatus_InvalidArgument;
            break;
    }

    if (result != kStatus_Success)
    {
        return result;
    }

    return NETC_MDIOWrite(mdioHandle, phyAddr, regAddr, data);
}

static status_t APP_PMDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    status_t result = kStatus_Success;
    netc_mdio_handle_t *mdioHandle;

    switch (phyAddr)
    {
        case EXAMPLE_EP0_PHY_ADDR:
            mdioHandle = &s_mdio_handle[0];
            break;
        case EXAMPLE_SWT_PORT0_PHY_ADDR:
            mdioHandle = &s_mdio_handle[1];
            break;
        case EXAMPLE_SWT_PORT1_PHY_ADDR:
            mdioHandle = &s_mdio_handle[2];
            break;
        case EXAMPLE_SWT_PORT2_PHY_ADDR:
            mdioHandle = &s_mdio_handle[3];
            break;
        case EXAMPLE_SWT_PORT3_PHY_ADDR:
            mdioHandle = &s_mdio_handle[4];
            break;
        default:
            result = kStatus_InvalidArgument;
            break;
    }

    if (result != kStatus_Success)
    {
        return result;
    }

    return NETC_MDIORead(mdioHandle, phyAddr, regAddr, pData);
}
#else
static status_t APP_EMDIOWrite(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return NETC_MDIOWrite(&s_emdio_handle, phyAddr, regAddr, data);
}

static status_t APP_EMDIORead(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return NETC_MDIORead(&s_emdio_handle, phyAddr, regAddr, pData);
}
#endif

static status_t APP_Phy8201SetUp(phy_handle_t *handle)
{
    status_t result;
    uint16_t data;

    result = PHY_Write(handle, PHY_PAGE_SELECT_REG, 7);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = PHY_Read(handle, 16, &data);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* CRS/DV pin is RXDV signal. */
    data |= (1U << 2);
    result = PHY_Write(handle, 16, data);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = PHY_Write(handle, PHY_PAGE_SELECT_REG, 0);

    return result;
}

status_t APP_PHY_Init(void)
{
    status_t result            = kStatus_Success;
    phy_config_t phy8211Config = {
        .autoNeg   = false,
        .speed     = kPHY_Speed100M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phyrtl8211f_ops,
    };
    phy_config_t phy8201Config = {
        .autoNeg   = false,
        .speed     = kPHY_Speed100M,
        .duplex    = kPHY_FullDuplex,
        .enableEEE = false,
        .ops       = &phyrtl8201_ops,
    };
    rgpio_pin_config_t pinConfig = {.pinDirection = kRGPIO_DigitalOutput, .outputLogic = 0};

    /* Reset PHY8201 for ETH4(EP), ETH0(Switch port0). Power on 150ms, reset 10ms, wait 150ms. */
    /* Reset PHY8211 for ETH1(Switch port1). Reset 10ms, wait 72ms. */
    RGPIO_PinInit(RGPIO4, 13, &pinConfig);
    RGPIO_PinInit(RGPIO4, 25, &pinConfig);
    RGPIO_PinInit(RGPIO6, 13, &pinConfig);
    SDK_DelayAtLeastUs(10000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    RGPIO_PinWrite(RGPIO4, 13, 1);
    RGPIO_PinWrite(RGPIO4, 25, 1);
    RGPIO_PinWrite(RGPIO6, 13, 1);
    SDK_DelayAtLeastUs(150000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    /* Initialize PHY for EP. */
#ifdef EXAMPLE_PHY_USE_PORT_MDIO
    s_phy_resource[0].write = APP_PMDIOWrite;
    s_phy_resource[0].read  = APP_PMDIORead;
#else
    s_phy_resource[0].write = APP_EMDIOWrite;
    s_phy_resource[0].read  = APP_EMDIORead;
#endif
    phy8201Config.resource = &s_phy_resource[0];
    phy8201Config.phyAddr  = EXAMPLE_EP0_PHY_ADDR;
    result                 = PHY_Init(&s_phy_handle[0], &phy8201Config);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = APP_Phy8201SetUp(&s_phy_handle[0]);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Initialize PHY for switch port0. */
#ifdef EXAMPLE_PHY_USE_PORT_MDIO
    s_phy_resource[1].write = APP_PMDIOWrite;
    s_phy_resource[1].read  = APP_PMDIORead;
#else
    s_phy_resource[1].write = APP_EMDIOWrite;
    s_phy_resource[1].read  = APP_EMDIORead;
#endif
    phy8201Config.resource = &s_phy_resource[1];
    phy8201Config.phyAddr  = EXAMPLE_SWT_PORT0_PHY_ADDR;
    result                 = PHY_Init(&s_phy_handle[1], &phy8201Config);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = APP_Phy8201SetUp(&s_phy_handle[1]);
    if (result != kStatus_Success)
    {
        return result;
    }

    /* Initialize PHY for switch port1. */
#ifdef EXAMPLE_PHY_USE_PORT_MDIO
    s_phy_resource[2].write = APP_PMDIOWrite;
    s_phy_resource[2].read  = APP_PMDIORead;
#else
    s_phy_resource[2].write = APP_EMDIOWrite;
    s_phy_resource[2].read  = APP_EMDIORead;
#endif
    phy8211Config.resource = &s_phy_resource[2];
    phy8211Config.phyAddr  = EXAMPLE_SWT_PORT1_PHY_ADDR;
    do
    {
        result = PHY_Init(&s_phy_handle[2], &phy8211Config);
        if (result == kStatus_Success)
        {
            break;
        }
        RGPIO_PinInit(RGPIO6, 13, &pinConfig);
        SDK_DelayAtLeastUs(10000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
        RGPIO_PinWrite(RGPIO6, 13, 1);
        SDK_DelayAtLeastUs(150000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    } while (true);

    return result;
}

status_t APP_PHY_WriteReadDuplex(phy_handle_t *handle)
{
    status_t result = kStatus_Success;
    uint16_t data   = 0;

    result = PHY_Write(handle, PHY_BASICCONTROL_REG, 0);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = PHY_Write(handle, PHY_BASICCONTROL_REG, PHY_BCTL_DUPLEX_MASK);
    if (result != kStatus_Success)
    {
        return result;
    }
    result = PHY_Read(handle, PHY_BASICCONTROL_REG, &data);
    if (result != kStatus_Success)
    {
        return result;
    }
    if ((data & PHY_BCTL_DUPLEX_MASK) == 0U)
    {
        return kStatus_Fail;
    }
    return result;
}

status_t APP_PHY_WriteReadTest(void)
{
    status_t result = kStatus_Success;

    for (int i = 0U; i < 3U; i++)
    {
        result = APP_PHY_WriteReadDuplex(&s_phy_handle[i]);
        if (result != kStatus_Success)
        {
            return result;
        }
    }
    return result;
}

status_t APP_GetPhyStatus(uint8_t port, app_phy_status_t *phyStatus)
{
    status_t result = kStatus_Success;

    if (EXAMPLE_MAC_INTERNAL_LOOP)
    {
        phyStatus->speed  = EXAMPLE_PHY_DEFAULT_SPEED;
        phyStatus->duplex = EXAMPLE_PHY_DEFAULT_DUPLEX;
        phyStatus->link   = true;
    }
    else
    {
        result = PHY_GetLinkStatus(&s_phy_handle[port], &phyStatus->link);
        if (result != kStatus_Success)
        {
            phyStatus->link = false;
        }

        if (EXAMPLE_PHY_AUTO_NEGO)
        {
            result = PHY_GetLinkSpeedDuplex(&s_phy_handle[port], &phyStatus->speed, &phyStatus->duplex);
        }
        else
        {
            phyStatus->speed  = EXAMPLE_PHY_DEFAULT_SPEED;
            phyStatus->duplex = EXAMPLE_PHY_DEFAULT_DUPLEX;
        }
    }

    switch (port)
    {
        case EXAMPLE_EP_PHY_PORT:
            phyStatus->miiMode = kNETC_RmiiMode;
            break;
        case EXAMPLE_SWT_PHY_PORT0:
            phyStatus->miiMode = kNETC_RmiiMode;
            break;
        case EXAMPLE_SWT_PHY_PORT1:
            phyStatus->miiMode = kNETC_RgmiiMode;
            break;
        default:
            result = kStatus_InvalidArgument;
            break;
    }

    return result;
}
/*${function:end}*/
