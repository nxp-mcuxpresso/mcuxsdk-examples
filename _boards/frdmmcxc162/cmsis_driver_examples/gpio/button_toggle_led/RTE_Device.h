/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

/* Driver name mapping. */
/* User needs to provide the implementation of GPIO set up pins for the enabled GPI instance. */
#define RTE_GPIO_PORT2 1
#define RTE_GPIO_PORT3 3

#define RTE_GPIO3_IRQ_HANDLER GPIO3_IRQHandler
#define RTE_GPIO3_IRQ         GPIO3_IRQn

#endif /* _RTE_DEVICE_H */
