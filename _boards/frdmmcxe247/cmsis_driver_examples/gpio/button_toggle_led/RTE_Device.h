/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

/* Driver name mapping. */
/* User needs to provide the implementation of GPIO set up pins for the enabled GPI instance. */
#define RTE_GPIO_PORTC 1

#define RTE_GPIOC_IRQ_HANDLER PORTC_IRQHandler
#define RTE_GPIOC_IRQ         PORTC_IRQn

#endif /* _RTE_DEVICE_H */
