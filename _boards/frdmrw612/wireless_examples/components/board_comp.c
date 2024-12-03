/*!
 * Copyright 2024 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * \file board_comp.c
 * \brief Collection of helper to init and configure HW resources for the application such as buttons, serial interfaces
 * , IOs, ...
 *
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include "pin_mux.h"
#include "board_comp.h"

#if defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0)
#include "fsl_component_button.h"
#endif /* defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0) */

#if defined(gAppLedCnt_c) && (gAppLedCnt_c > 0)
#include "fsl_component_led.h"
#endif /* defined(gAppLedCnt_c) && (gAppLedCnt_c > 0) */

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */

#if defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0)

static const button_config_t button0Config = {
    .gpio =
        {
            .pinStateDefault = BOARD_BUTTON0_GPIO_PIN_DEFAULT_STATE,
            .port            = BOARD_BUTTON0_GPIO_PORT_INSTANCE,
            .pin             = BOARD_BUTTON0_GPIO_PIN,
        },
};

#endif

#if defined(gAppLedCnt_c) && (gAppLedCnt_c > 0)

static const led_config_t g_RgbLedConfig = {
    .type = kLED_TypeRgb,
    .ledRgb =
        {
            .redPin =
                {
                    .dimmingEnable = 0,
                    .gpio =
                        {
#if (defined(LED_USE_CONFIGURE_STRUCTURE) && (LED_USE_CONFIGURE_STRUCTURE > 0U))
                            .direction = kHAL_GpioDirectionOut,
#endif
                            .level = BOARD_RGB_RED_GPIO_PIN_DEFAULT_STATE,
                            .port  = BOARD_RGB_RED_GPIO_PORT_INSTANCE,
                            .pin   = BOARD_RGB_RED_GPIO_PIN,
                        },
                },
            .greenPin =
                {
                    .dimmingEnable = 0,
                    .gpio =
                        {
#if (defined(LED_USE_CONFIGURE_STRUCTURE) && (LED_USE_CONFIGURE_STRUCTURE > 0U))
                            .direction = kHAL_GpioDirectionOut,
#endif
                            .level = BOARD_RGB_GREEN_GPIO_PIN_DEFAULT_STATE,
                            .port  = BOARD_RGB_GREEN_GPIO_PORT_INSTANCE,
                            .pin   = BOARD_RGB_GREEN_GPIO_PIN,
                        },
                },
            .bluePin =
                {
                    .dimmingEnable = 0,
                    .gpio =
                        {
#if (defined(LED_USE_CONFIGURE_STRUCTURE) && (LED_USE_CONFIGURE_STRUCTURE > 0U))
                            .direction = kHAL_GpioDirectionOut,
#endif
                            .level = BOARD_RGB_BLUE_GPIO_PIN_DEFAULT_STATE,
                            .port  = BOARD_RGB_BLUE_GPIO_PORT_INSTANCE,
                            .pin   = BOARD_RGB_BLUE_GPIO_PIN,
                        },
                },
        },
};

#endif /* defined(gAppLedCnt_c) && (gAppLedCnt_c > 0) */

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

#if defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0)

void BOARD_InitButton0(button_handle_t buttonHandle)
{
    button_status_t ret;
    button_config_t button_config = button0Config;

    /* Init Pin mux */
    BOARD_InitPinButton0();

    /* Init button module and Gpio module */
    ret = BUTTON_Init(buttonHandle, &button_config);
    assert(ret == kStatus_BUTTON_Success);
    (void)ret;
}

#endif /* defined(gAppButtonCnt_c) && (gAppButtonCnt_c > 0) */

#if defined(gAppLedCnt_c) && (gAppLedCnt_c > 0)

void BOARD_InitRgbLed(led_handle_t ledHandle)
{
    led_status_t ret;

    BOARD_InitPinLEDRGB();

    ret = LED_Init(ledHandle, &g_RgbLedConfig);
    assert(kStatus_LED_Success == ret);
    (void)ret;
}

#endif /* defined(gAppLedCnt_c) && (gAppLedCnt_c > 0) */