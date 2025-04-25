/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_iocon.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
#include "fsl_component_led.h"
/*${header:end}*/

/*${variable:start}*/
led_config_t g_ledMonochrome[LED_TYPE_MONOCHROME_COUNT] = {
    {
        .type = kLED_TypeMonochrome,
        .ledMonochrome =
            {
                .monochromePin =
                    {
                        .dimmingEnable = 0,
                        .gpio =
                            {
#if (defined(LED_USE_CONFIGURE_STRUCTURE) && (LED_USE_CONFIGURE_STRUCTURE > 0U))
                                .direction = kHAL_GpioDirectionOut,
#endif
                                .level = 1,
                                .port  = BOARD_LED_GPIO_PORT,
                                .pin   = BOARD_LED_GPIO_PIN,
                            },
                    },
            },
    },
};
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    POWER_Init();
    GPIO_PortInit(GPIO, 0);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM0);
    CLOCK_EnableClock(kCLOCK_FlexComm0);
    CLOCK_EnableClock(kCLOCK_InputMux);
    CLOCK_EnableClock(kCLOCK_Dma0);
    CLOCK_AttachClk(kOSC32K_to_CTIMER2);
    CLOCK_AttachClk(kOSC32K_to_CTIMER1);

    BOARD_InitPins();
    BOARD_BootClockXTAL32M();
    BOARD_InitDebugConsole2();
    BOARD_Configure32kOsc();
}
/*${function:end}*/
