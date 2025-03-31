/*
 * Copyright 2021-2022, 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_enet.h"
#include "fsl_gpio.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/
phy_lan8741_resource_t g_phy_resource;
/*${variable:end}*/

/*${function:start}*/
static void MDIO_Init(void)
{
    CLOCK_EnableClock(s_enetClock[ENET_GetInstance(EXAMPLE_ENET)]);
    ENET_SetSMI(EXAMPLE_ENET, EXAMPLE_CLOCK_FREQ, false);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(EXAMPLE_ENET, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(EXAMPLE_ENET, phyAddr, regAddr, pData);
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitENETPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Disable MPU */
    SYSMPU->CESR &= ~SYSMPU_CESR_VLD(1);

    /* Reset PHY */
    GPIO_PinWrite(BOARD_INITENETPINS_PTC3_PIN42_GPIO, BOARD_INITENETPINS_PTC3_PIN42_PIN, 0U);
    SDK_DelayAtLeastUs(25000, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    GPIO_PinWrite(BOARD_INITENETPINS_PTC3_PIN42_GPIO, BOARD_INITENETPINS_PTC3_PIN42_PIN, 1U);

    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;

    MDIO_Init();
}

/*${function:end}*/
