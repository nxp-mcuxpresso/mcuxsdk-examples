/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_inputmux.h"
#include "fsl_pint.h"
#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/
extern volatile bool g_userPress;

void pint_intr_callback(pint_pin_int_t pintr, pint_status_t* pmatch_status)
{
    g_userPress = true;
}

void BOARD_InitKey(void)
{
    /* Connect trigger sources to PINT */
    INPUTMUX_Init(INPUTMUX);
    INPUTMUX_AttachSignal(INPUTMUX, kPINT_PinInt0, kINPUTMUX_GpioPort0Pin18ToPintsel);
    /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */
    INPUTMUX_Deinit(INPUTMUX);

    /* Initialize PINT */
    PINT_Init(PINT);

    /* Setup Pin Interrupt 0 for falling edge */
    PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableFallEdge);
    PINT_SetCallback(PINT, pint_intr_callback);

    /* Enable callbacks for PINT */
    PINT_EnableCallback(PINT);
}

void BOARD_InitHardware(void)
{
    POWER_Init();
    CLOCK_EnableClock(kCLOCK_Iocon);
    CLOCK_EnableClock(kCLOCK_Gpio0);
    GPIO_PortInit(GPIO, 0);

    BOARD_InitPins();
    BOARD_InitBootClocks();

    CLOCK_AttachClk(kTRACE_DIV_to_TRACE);
    CLOCK_SetClkDiv(kCLOCK_DivArmTrClkDiv, 1U, false);         /*!< Set TRACECLKDIV divider to value 1 */

    BOARD_InitKey();
}

void BOARD_InitDebugConsoleSWO(unsigned int port, unsigned int baudrate)
{
    DbgConsole_Init(port, baudrate, kSerialPort_Swo, SystemCoreClock);
}
/*${function:end}*/
