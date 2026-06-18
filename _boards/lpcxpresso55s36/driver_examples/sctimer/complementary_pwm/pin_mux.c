/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_iocon.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
}

/* SCT0 outputs used by the two dead-time complementary pairs (selected through IOCON FUNC4):
 *   pair 1 high: SCT0_OUT0 = PIO1_4  (J10-5)
 *   pair 1 low:  SCT0_OUT1 = PIO1_8  (J10-7)
 *   pair 2 high: SCT0_OUT3 = PIO1_10 (J92-9)
 *   pair 2 low:  SCT0_OUT4 = PIO1_17 (J10-13)
 * Debug console (FLEXCOMM0) uses PIO0_29 / PIO0_30 (FUNC1).
 */
static void BOARD_ConfigSctOut(uint32_t port, uint32_t pin)
{
    const uint32_t config = (IOCON_PIO_FUNC4 | IOCON_PIO_MODE_INACT | IOCON_PIO_SLEW_STANDARD |
                             IOCON_PIO_INV_DI | IOCON_PIO_DIGITAL_EN | IOCON_PIO_OPENDRAIN_DI);
    IOCON_PinMuxSet(IOCON, (uint8_t)port, (uint8_t)pin, config);
}

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M33 */
void BOARD_InitPins(void)
{
    /* Enables the clock for the I/O controller. */
    CLOCK_EnableClock(kCLOCK_Iocon);

    /* Debug console: FC0 RXD on PIO0_29, TXD on PIO0_30 (FUNC1). */
    const uint32_t fc0_config = (IOCON_PIO_FUNC1 | IOCON_PIO_MODE_INACT | IOCON_PIO_SLEW_STANDARD |
                                 IOCON_PIO_INV_DI | IOCON_PIO_DIGITAL_EN | IOCON_PIO_OPENDRAIN_DI);
    IOCON_PinMuxSet(IOCON, 0U, 29U, fc0_config);
    IOCON_PinMuxSet(IOCON, 0U, 30U, fc0_config);

    /* SCT0 outputs for the two complementary pairs. */
    BOARD_ConfigSctOut(1U, 4U);  /* SCT0_OUT0 - pair 1 high */
    BOARD_ConfigSctOut(1U, 8U);  /* SCT0_OUT1 - pair 1 low  */
    BOARD_ConfigSctOut(1U, 10U); /* SCT0_OUT3 - pair 2 high */
    BOARD_ConfigSctOut(1U, 17U); /* SCT0_OUT4 - pair 2 low  */
}
