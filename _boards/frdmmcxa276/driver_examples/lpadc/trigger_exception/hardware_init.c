/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "board.h"
#include <stdbool.h>
#include "fsl_inputmux.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    RESET_PeripheralReset(kADC0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kINPUTMUX0_RST_SHIFT_RSTn);

    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBUTTONsPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_SetClockDiv(kCLOCK_DivADC, 1U);
    CLOCK_AttachClk(kFRO_LF_DIV_to_ADC);

    /* Map Input pin to ADC trigger1 input. */
    INPUTMUX_Init(INPUTMUX0);
    INPUTMUX_AttachSignal(INPUTMUX0, 1UL, kINPUTMUX_Gpio1PinEventTrig0ToAdc0Trigger);

    GPIO_SetPinInterruptConfig(BOARD_SW2_GPIO, BOARD_SW2_GPIO_PIN, kGPIO_ActiveLowTriggerOutputEnable);
    EnableIRQ(BOARD_SW2_IRQ);
}

void BOARD_SW2_IRQ_HANDLER(void)
{
    GPIO_GpioClearInterruptFlags(BOARD_SW2_GPIO, 1U << BOARD_SW2_GPIO_PIN);
}

/*${function:end}*/
