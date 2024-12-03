/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void I2C8_InitPins();
extern void I2C8_DeinitPins();
extern void I2C9_InitPins();
extern void I2C9_DeinitPins();

/*Driver name mapping*/
/* User needs to provide the implementation of I2CX_GetFreq/I2CX_InitPins/I2CX_DeinitPins for the enabled I2C instance.
 */
#define RTE_I2C8            1
#define RTE_I2C8_PIN_INIT   I2C8_InitPins
#define RTE_I2C8_PIN_DEINIT I2C8_DeinitPins
#define RTE_I2C8_DMA_EN     0
#define RTE_I2C9            1
#define RTE_I2C9_PIN_INIT   I2C9_InitPins
#define RTE_I2C9_PIN_DEINIT I2C9_DeinitPins
#define RTE_I2C9_DMA_EN     0

/*I2C configuration*/
#define RTE_I2C8_Master_DMA_BASE DMA0
#define RTE_I2C8_Master_DMA_CH   17

#define RTE_I2C9_Master_DMA_BASE DMA0
#define RTE_I2C9_Master_DMA_CH   19
#endif /* _RTE_DEVICE_H */
