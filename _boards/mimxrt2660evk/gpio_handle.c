/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "gpio_handle.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if BOARD_USE_GPIO0_HANDLE
gpio_handle_t g_gpio0Handle; /* HSP__GPIO_0 */
#endif
#if BOARD_USE_GPIO1_HANDLE
gpio_handle_t g_gpio1Handle; /* HSP__GPIO_1 */
#endif
#if BOARD_USE_GPIO2_HANDLE
gpio_handle_t g_gpio2Handle; /* HSP__GPIO_2 */
#endif
#if BOARD_USE_GPIO3_HANDLE
gpio_handle_t g_gpio3Handle; /* HSP__GPIO_3 */
#endif
#if BOARD_USE_GPIO4_HANDLE
gpio_handle_t g_gpio4Handle; /* HSP__GPIO_4 */
#endif
#if BOARD_USE_GPIO5_HANDLE
gpio_handle_t g_gpio5Handle; /* VBAT__GPIO  */
#endif
#if BOARD_USE_GPIO6_HANDLE
gpio_handle_t g_gpio6Handle; /* WAKE__GPIO  */
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_CreateGpioHandle(void)
{
#if BOARD_USE_GPIO0_HANDLE
    GPIO_PortInit(BOARD_GPIO0);
    GPIO_CreateHandle(BOARD_GPIO0, &g_gpio0Handle);
    EnableIRQ(BOARD_GPIO0_IRQ);
#endif

#if BOARD_USE_GPIO1_HANDLE
    GPIO_PortInit(BOARD_GPIO1);
    GPIO_CreateHandle(BOARD_GPIO1, &g_gpio1Handle);
    EnableIRQ(BOARD_GPIO1_IRQ);
#endif

#if BOARD_USE_GPIO2_HANDLE
    GPIO_PortInit(BOARD_GPIO2);
    GPIO_CreateHandle(BOARD_GPIO2, &g_gpio2Handle);
    EnableIRQ(BOARD_GPIO2_IRQ);
#endif

#if BOARD_USE_GPIO3_HANDLE
    GPIO_PortInit(BOARD_GPIO3);
    GPIO_CreateHandle(BOARD_GPIO3, &g_gpio3Handle);
    EnableIRQ(BOARD_GPIO3_IRQ);
#endif

#if BOARD_USE_GPIO4_HANDLE
    GPIO_PortInit(BOARD_GPIO4);
    GPIO_CreateHandle(BOARD_GPIO4, &g_gpio4Handle);
    EnableIRQ(BOARD_GPIO4_IRQ);
#endif

#if BOARD_USE_GPIO5_HANDLE
    GPIO_PortInit(BOARD_GPIO5);
    GPIO_CreateHandle(BOARD_GPIO5, &g_gpio5Handle);
    EnableIRQ(BOARD_GPIO5_IRQ);
#endif

#if BOARD_USE_GPIO6_HANDLE
    GPIO_PortInit(BOARD_GPIO6);
    GPIO_CreateHandle(BOARD_GPIO6, &g_gpio6Handle);
    EnableIRQ(BOARD_GPIO6_IRQ);
#endif
}
