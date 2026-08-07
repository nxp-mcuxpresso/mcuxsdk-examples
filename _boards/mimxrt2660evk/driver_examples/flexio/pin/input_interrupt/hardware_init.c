/*
 * Copyright 2024, 2026 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_clock.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitFLEXIO_PININPUTPins();
    BOARD_InitLEDsPins();

    CLOCK_EnableClock(kCLOCK_MAIN_hsp_flexio1);

    /* GPIO configuration on HSP__GPIO_1 pin 1 */
    gpio_pin_config_t gpio_hsp_gpio_1_pin1_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 0U,
    };

    /* Initialize GPIO on HSP__GPIO_1 pin 1 */
    GPIO_PinInit(HSP__GPIO_1, 1U, &gpio_hsp_gpio_1_pin1_config);

    /* PIO3_1 is configured as HSP_GPIO1_GPIO1 */
    /* Input Buffer Enable: Enables */
    IOMUXC_SetPin_Mux_Config(IOMUXC_PIO3_1_HSP_GPIO1_GPIO1, 0x80U);

    CLOCK_SetRootClockDiv(kCLOCK_Root_MAIN_flexio1_fclk, 10U);

    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput,
        0,
    };

    GPIO_PinInit(BOARD_GPIO_OUTPUT_PORT, BOARD_GPIO_OUTPUT_PORT_PIN, &led_config);

    CLOCK_EnableClock(kCLOCK_MAIN_hsp_flexio0);
    SystemCoreClockUpdate();
}
/*${function:end}*/
