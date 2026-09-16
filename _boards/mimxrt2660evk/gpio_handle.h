/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef GPIO_HANDLE_H_
#define GPIO_HANDLE_H_

#include "fsl_gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#ifndef BOARD_USE_GPIO0_HANDLE
#define BOARD_USE_GPIO0_HANDLE 0
#endif
#ifndef BOARD_USE_GPIO1_HANDLE
#define BOARD_USE_GPIO1_HANDLE 0
#endif
#ifndef BOARD_USE_GPIO2_HANDLE
#define BOARD_USE_GPIO2_HANDLE 0
#endif
#ifndef BOARD_USE_GPIO3_HANDLE
#define BOARD_USE_GPIO3_HANDLE 0
#endif
#ifndef BOARD_USE_GPIO4_HANDLE
#define BOARD_USE_GPIO4_HANDLE 0
#endif
#ifndef BOARD_USE_GPIO5_HANDLE
#define BOARD_USE_GPIO5_HANDLE 0
#endif
#ifndef BOARD_USE_GPIO6_HANDLE
#define BOARD_USE_GPIO6_HANDLE 0
#endif

#define BOARD_GPIO0      HSP__GPIO_0
#define BOARD_GPIO0_IRQ  HSP_GPIO0_CH0_IRQn
#define BOARD_GPIO1      HSP__GPIO_1
#define BOARD_GPIO1_IRQ  HSP_GPIO1_CH0_IRQn
#define BOARD_GPIO2      HSP__GPIO_2
#define BOARD_GPIO2_IRQ  HSP_GPIO2_CH0_IRQn
#define BOARD_GPIO3      HSP__GPIO_3
#define BOARD_GPIO3_IRQ  HSP_GPIO3_CH0_IRQn
#define BOARD_GPIO4      HSP__GPIO_4
#define BOARD_GPIO4_IRQ  HSP_GPIO4_CH0_IRQn
#define BOARD_GPIO5      VBAT__GPIO
#define BOARD_GPIO5_IRQ  VBAT_GPIO_CH0_IRQn
#define BOARD_GPIO6      WAKE__GPIO
#define BOARD_GPIO6_IRQ  WAKE_GPIO_CH0_IRQn

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if BOARD_USE_GPIO0_HANDLE
extern gpio_handle_t g_gpio0Handle; /* HSP__GPIO_0 */
#endif
#if BOARD_USE_GPIO1_HANDLE
extern gpio_handle_t g_gpio1Handle; /* HSP__GPIO_1 */
#endif
#if BOARD_USE_GPIO2_HANDLE
extern gpio_handle_t g_gpio2Handle; /* HSP__GPIO_2 */
#endif
#if BOARD_USE_GPIO3_HANDLE
extern gpio_handle_t g_gpio3Handle; /* HSP__GPIO_3 */
#endif
#if BOARD_USE_GPIO4_HANDLE
extern gpio_handle_t g_gpio4Handle; /* HSP__GPIO_4 */
#endif
#if BOARD_USE_GPIO5_HANDLE
extern gpio_handle_t g_gpio5Handle; /* VBAT__GPIO  */
#endif
#if BOARD_USE_GPIO6_HANDLE
extern gpio_handle_t g_gpio6Handle; /* WAKE__GPIO  */
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void BOARD_CreateGpioHandle(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _GPIO_HANDLE_H_ */
