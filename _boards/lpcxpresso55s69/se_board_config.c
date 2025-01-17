/* Copyright 2023 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "se_board_config.h"
#include "se05x_apis.h"
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "sm_timer.h"
#include "sm_types.h"

/*
 * Configure the PINs on the Host
 *
 */

void se05x_host_configure()
{
    gpio_pin_config_t reset_pin_cfg = {kGPIO_DigitalOutput, SE_RESET_LOGIC};
    GPIO_PinInit(GPIO, (uint32_t)SE05X_ENA_HOST_PORT, SE05X_ENA_HOST_PIN, &reset_pin_cfg);
    return;
}

/*
 * Where applicable, put SE in low power/standby mode
 *
 * Pre-Requistie: @ref se05x_host_configure has been called
 */

void se05x_host_powerdown()
{
    GPIO_PinWrite(GPIO, (uint32_t)SE05X_ENA_HOST_PORT, SE05X_ENA_HOST_PIN, !SE_RESET_LOGIC);
    return;
}

/*
 * Where applicable, put SE in powered/active mode
 *
 * Pre-Requistie: @ref se05x_host_configure has been called
 */

void se05x_host_powerup()
{
    GPIO_PinWrite(GPIO, (uint32_t)SE05X_ENA_HOST_PORT, SE05X_ENA_HOST_PIN, SE_RESET_LOGIC);
    return;
}
