/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

/* Driver name mapping. */
/* User needs to provide the implementation of GPIO set up pins for the enabled GPI instance. */
#define RTE_GPIO_PORTB 1

#define RTE_GPIOB_IRQ_HANDLER PORTB_IRQHandler
#define RTE_GPIOB_IRQ         PORTB_IRQn

#endif /* _RTE_DEVICE_H */
