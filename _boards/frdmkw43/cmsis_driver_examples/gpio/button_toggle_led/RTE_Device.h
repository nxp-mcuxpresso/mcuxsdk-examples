/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

/* Driver name mapping. */
/* User needs to provide the implementation of GPIO set up pins for the enabled GPI instance. */
#define RTE_GPIO_PORTD        1

#define RTE_GPIOD_IRQ_HANDLER GPIOD_INT0_IRQHandler
#define RTE_GPIOD_IRQ         GPIOD_INT0_IRQn

#endif /* _RTE_DEVICE_H */
