/* Copyright 2023 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _SE_BOARD_CONFIG_H
#define _SE_BOARD_CONFIG_H

#include "board.h"

/*
 * Platform based definitions for Enable pin
 * Define GPIO port for enable pin
 * Define Pin number on GPIO port for enable pin
 */

#define SE05X_ENA_HOST_PORT BOARD_SW2_GPIO_PORT
#define SE05X_ENA_HOST_PIN  4U

/*
 * Where applicable, Configure the PINs on the Host
 *
 */
void se05x_host_configure(void);

/*
 * Where applicable, put SE in low power/standby mode
 *
 * Pre-Requistie: @ref se05x_host_configure has been called
 */
void se05x_host_powerdown(void);

/*
 * Where applicable, put SE in powered/active mode
 *
 * Pre-Requistie: @ref se05x_host_configure has been called
 */
void se05x_host_powerup(void);

#endif // _SE_BOARD_CONFIG_H
