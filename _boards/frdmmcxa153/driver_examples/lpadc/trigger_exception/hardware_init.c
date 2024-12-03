/*
 * Copyright 2023 NXP
 * All rights reserved.
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

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_SetClockDiv(kCLOCK_DivADC0, 1U);
    CLOCK_AttachClk(kFRO12M_to_ADC0);

    /* Map Input pin to ADC trigger1 input. */
    INPUTMUX_Init(INPUTMUX0);
    INPUTMUX_AttachSignal(INPUTMUX0, 1UL, kINPUTMUX_Gpio3PinEventTrig0ToAdc0Trigger);

    CLOCK_EnableClock(kCLOCK_GateGPIO3);

    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput,
        0,
    };
    GPIO_SetPinInterruptConfig(BOARD_SW2_GPIO, BOARD_SW2_GPIO_PIN, kGPIO_ActiveLowTriggerOutputEnable);
    EnableIRQ(BOARD_SW2_IRQ);
    GPIO_PinInit(BOARD_SW2_GPIO, BOARD_SW2_GPIO_PIN, &sw_config);
}

void BOARD_SW2_IRQ_HANDLER(void)
{
    GPIO_GpioClearInterruptFlags(BOARD_SW2_GPIO, 1U << BOARD_SW2_GPIO_PIN);
}

/*${function:end}*/
