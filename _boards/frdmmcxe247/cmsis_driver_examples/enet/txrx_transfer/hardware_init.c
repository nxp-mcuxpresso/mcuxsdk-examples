/*
 * Copyright 2021-2022, 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_enet_phy_cmsis.h"
#include "fsl_enet_cmsis.h"
#include "fsl_phylan8741.h"
#include "fsl_gpio.h"
#include "fsl_enet.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/
cmsis_enet_mac_resource_t ENET0_Resource;
cmsis_enet_phy_resource_t ENETPHY0_Resource;
static phy_lan8741_resource_t g_phy_resource;
/*${variable:end}*/

/*${function:start}*/
uint32_t ENET_GetFreq(void)
{
    return CLOCK_GetCoreSysClkFreq();
}

static void MDIO_Init(void)
{
    (void)CLOCK_EnableClock(s_enetClock[ENET_GetInstance(ENET)]);
    ENET_SetSMI(ENET, ENET_GetFreq(), false);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(ENET, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(ENET, phyAddr, regAddr, pData);
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Disable MPU */
    SYSMPU->CESR &= ~SYSMPU_CESR_VLD(1);

    /* Reset PHY */
    GPIO_PinWrite(BOARD_INITPINS_PHY_RESET_GPIO, BOARD_INITPINS_PHY_RESET_PIN, 0U);
    SDK_DelayAtLeastUs(25000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    GPIO_PinWrite(BOARD_INITPINS_PHY_RESET_GPIO, BOARD_INITPINS_PHY_RESET_PIN, 1U);

    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;

    ENET0_Resource.base           = ENET;
    ENET0_Resource.GetFreq        = ENET_GetFreq;
    ENETPHY0_Resource.phyAddr     = RTE_ENET_PHY_ADDRESS;
    ENETPHY0_Resource.ops         = &phylan8741_ops;
    ENETPHY0_Resource.opsResource = &g_phy_resource;

    MDIO_Init();
}
/*${function:end}*/
