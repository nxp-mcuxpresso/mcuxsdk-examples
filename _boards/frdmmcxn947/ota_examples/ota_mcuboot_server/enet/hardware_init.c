/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_enet.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"
#include "fsl_phylan8741.h"

#define EXAMPLE_ENET_BASE ENET0
/*${header:end}*/

/*${variable:start}*/
phy_lan8741_resource_t g_phy_resource;
/*${variable:end}*/

/*${function:start}*/

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(EXAMPLE_ENET_BASE, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(EXAMPLE_ENET_BASE, phyAddr, regAddr, pData);
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    CLOCK_EnableClock(kCLOCK_InputMux);
    /* Attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_EnableClock(kCLOCK_LPUart4);

    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    CLOCK_EnableClock(kCLOCK_Trng);

    /* Select RMII type PHY */
    SYSCON0->ENET_PHY_INTF_SEL = SYSCON_ENET_PHY_INTF_SEL_PHY_SEL(1);
#if 0
    /* ENET is clocked from PLL0 (150 MHz) */
    CLOCK_AttachClk(kPLL0_to_ENETRMII);
    CLOCK_SetClkDiv(kCLOCK_DivEnetrmiiClk, 3);
#else
    /* ENET is clocked from TXCLK PIN
        R274 0-Ohm (originally DNP) needs to be added to the board
    */
    CLOCK_AttachClk(kNONE_to_ENETRMII);        /* No internal clock */
    CLOCK_SetClkDiv(kCLOCK_DivEnetrmiiClk, 0); /* Halt */
#endif
    //    CLOCK_AttachClk(kPLL0_to_ENETPTPREF);
    /* Enable ENET bus clocks */
    CLOCK_EnableClock(kCLOCK_Enet);
    /* Set MII speed - must be done after ENET is clocked */
    ENET_SetSMI(EXAMPLE_ENET_BASE, CLOCK_GetPll0OutFreq());
    /* Reset ENET */
    SYSCON0->PRESETCTRL2 = SYSCON_PRESETCTRL2_ENET_RST_MASK;
    SYSCON0->PRESETCTRL2 &= ~SYSCON_PRESETCTRL2_ENET_RST_MASK;

    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;
}
/*${function:end}*/
