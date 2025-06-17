/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_enet_qos.h"
#include "fsl_phy.h"
#include "fsl_siul2.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/
phy_lan8741_resource_t g_phy_resource;
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitModuleClock(void)
{
    CLOCK_SetEmacRmiiTxClkFreq(50000000U);

    CLOCK_AttachClk(kEMAC_RMII_TX_CLK_to_EMAC_TX);
    CLOCK_AttachClk(kEMAC_RMII_TX_CLK_to_EMAC_RX);
    CLOCK_AttachClk(kEMAC_RMII_TX_CLK_to_EMAC_TS);

    CLOCK_SetClkDiv(kCLOCK_DivEmacRxClk, 2U);
    CLOCK_SetClkDiv(kCLOCK_DivEmacTxClk, 2U);
    CLOCK_SetClkDiv(kCLOCK_DivEmacTsClk, 2U);

    ENET_QOS_EnableClock(true);
}

void ENET_QOS_SetSYSControl(enet_qos_mii_mode_t miiMode)
{
    if (miiMode == kENET_QOS_RmiiMode)
    {
        DCM_GPR->DCMRWF1 = (DCM_GPR->DCMRWF1 & DCM_GPR_DCMRWF1_RMII_MII_SEL_MASK) | DCM_GPR_DCMRWF1_RMII_MII_SEL(1U);
    }
    else
    {
        DCM_GPR->DCMRWF1 = (DCM_GPR->DCMRWF1 & DCM_GPR_DCMRWF1_RMII_MII_SEL_MASK) | DCM_GPR_DCMRWF1_RMII_MII_SEL(0U);
    }
}

void ENET_QOS_EnableClock(bool enable)
{
    if (enable)
    {
        CLOCK_EnableClock(kCLOCK_Emac);
    }
    else
    {
        CLOCK_DisableClock(kCLOCK_Emac);
    }
}

static void MDIO_Init(void)
{
    ENET_QOS_SetSMI(EXAMPLE_ENET_QOS_BASE, CORE_CLK_FREQ);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_QOS_MDIOWrite(EXAMPLE_ENET_QOS_BASE, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_QOS_MDIORead(EXAMPLE_ENET_QOS_BASE, phyAddr, regAddr, pData);
}

void BOARD_InitHardware(void)
{
    /* Hardware Initialization. */
    BOARD_ConfigMPU();

    BOARD_InitBootPins();
    BOARD_InitEMACPins();
    BOARD_InitBootClocks();

    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();

    /* Reset PHY */
    SIUL2_PortPinWrite(BOARD_INITEMACPINS_ENET_PHY_RST_SIUL2_BASE, BOARD_INITEMACPINS_ENET_PHY_RST_GPIO, 3U, 0U);
    SDK_DelayAtLeastUs(25000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    SIUL2_PortPinWrite(BOARD_INITEMACPINS_ENET_PHY_RST_SIUL2_BASE, BOARD_INITEMACPINS_ENET_PHY_RST_GPIO, 3U, 1U);

    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;

    MDIO_Init();
}
/*${function:end}*/
