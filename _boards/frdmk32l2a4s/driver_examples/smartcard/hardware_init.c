/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_smartcard_emvsim.h"
#include "fsl_intmux.h"
#include "fsl_port.h"
#if defined(USING_PHY_TDA8035)
#include "fsl_smartcard_phy_tda8035.h"
#endif
#include <stdbool.h>
#include "clock_config.h"
/*${header:end}*/

extern smartcard_context_t *g_smartcardContext;

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();

    /* Initialize pin interrupt */
    PORT_SetPinInterruptConfig(PORTE, 2u, kPORT_InterruptEitherEdge);
    /* Clear interrupt status flags */
    PORT_ClearPinsInterruptFlags(PORTE, 1u << 2u);

    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    INTMUX_Init(INTMUX0);

    /* Enable EMVSIM intmux source on INTMUX channel 0 */
    INTMUX_EnableInterrupt(INTMUX0, 0, EMVSIM0_IRQn);

    /* Set clock source for EVMSIM */
    CLOCK_SetIpSrc(kCLOCK_Emvsim0, kCLOCK_IpSrcFircAsync);
}

/* IRQ handler for emvsim */
void EMVSIM0_IRQHandler(void)
{
    SMARTCARD_EMVSIM_IRQHandler(BOARD_SMARTCARD_MODULE, g_smartcardContext);
}
#if defined(USING_PHY_TDA8035)
/* IRQ handler for card presence detection */
void PORTE_IRQHandler(void)
{
    /* Clear interrupt status flags */
    PORT_ClearPinsInterruptFlags(PORTE, 1u << 2u);
    SMARTCARD_PHY_TDA8035_IRQHandler(BOARD_SMARTCARD_MODULE, g_smartcardContext);
}
#endif
/*${function:end}*/
