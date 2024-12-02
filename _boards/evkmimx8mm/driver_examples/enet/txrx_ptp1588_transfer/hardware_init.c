/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_enet.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/
#if EXAMPLE_ENET_PHY_PHYAR8031
phy_ar8031_resource_t g_phy_resource;
#else
phy_rtl8211f_resource_t g_phy_resource;
#endif
/*${variable:end}*/

/*${function:start}*/
static void MDIO_Init(void)
{
    (void)CLOCK_EnableClock(s_enetClock[ENET_GetInstance(EXAMPLE_ENET)]);
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
    /* Board specific RDC settings */
    BOARD_RdcInit();

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitMemory();

    CLOCK_SetRootDivider(kCLOCK_RootEnetAxi, 1U, 1U);
    CLOCK_SetRootMux(kCLOCK_RootEnetAxi, kCLOCK_EnetAxiRootmuxSysPll2Div4); /* SYSTEM PLL2 divided by 4: 125Mhz */

    CLOCK_SetRootDivider(kCLOCK_RootEnetTimer, 1U, 1U);
    CLOCK_SetRootMux(kCLOCK_RootEnetTimer, kCLOCK_EnetTimerRootmuxSysPll2Div10); /* SYSTEM PLL2 divided by 10: 100Mhz */

    CLOCK_SetRootDivider(kCLOCK_RootEnetRef, 1U, 1U);
    CLOCK_SetRootMux(kCLOCK_RootEnetRef, kCLOCK_EnetRefRootmuxSysPll2Div8); /* SYSTEM PLL2 divided by 8: 125Mhz */

    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
    GPIO_PinInit(GPIO4, 22U, &gpio_config);
    GPIO_WritePinOutput(GPIO4, 22U, 0);
    SDK_DelayAtLeastUs(10000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    GPIO_WritePinOutput(GPIO4, 22U, 1);
    SDK_DelayAtLeastUs(30000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

    EnableIRQ(ENET1_MAC0_Rx_Tx_Done1_IRQn);
    EnableIRQ(ENET1_MAC0_Rx_Tx_Done2_IRQn);

    MDIO_Init();
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;
}
/*${function:end}*/
