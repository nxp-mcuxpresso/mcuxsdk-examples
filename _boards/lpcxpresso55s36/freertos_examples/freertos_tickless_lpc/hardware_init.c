/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include <stdbool.h>
#include "FreeRTOS.h"
#include "fsl_power.h"
#if configUSE_TICKLESS_IDLE == 2
#include "fsl_ostimer.h"
#include "app.h"
#endif
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* enable clock for IOCON */
    CLOCK_EnableClock(kCLOCK_Iocon);

    /* enable clock for GPIO*/
    CLOCK_EnableClock(kCLOCK_Gpio0);

    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 0u, false);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom0Clk, 1u, true);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

#if configUSE_TICKLESS_IDLE == 2

    /* Clock setting for OS timer. choose xtal32k as the source clock of OS timer. */
    POWER_DisablePD(kPDRUNCFG_PD_XTAL32K);
    CLOCK_AttachClk(kXTAL32K_to_OSC32K);
    CLOCK_AttachClk(kOSC32K_to_OSTIMER);

#endif

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

#if configUSE_TICKLESS_IDLE == 2

/*!
 * @brief Fuction of OS timer.
 *
 * This function to return OS timer base address
 */

OSTIMER_Type *vPortGetOstimerBase(void)
{
    return TICKLESS_OSTIMER_BASE_PTR;
}

/*!
 * @brief Fuction of OS timer.
 *
 * This function to return OS timer interrupt number
 */

IRQn_Type vPortGetOstimerIrqn(void)
{
    return TICKLESS_OSTIMER_IRQn;
}
#endif
/*${function:end}*/
