/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#include "fsl_msmc.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_SMC                    MSMC0
#define BOARD_ACCEL_I2C_BASEADDR    LPI2C3
#define DEMO_ACCEL_I2C_CLOCK_NAME   kCLOCK_Lpi2c3
#define DEMO_ACCEL_I2C_CLOCK_SOURCE kCLOCK_IpSrcSircAsync

#define DEMO_LPI2C_RX_DMA_REQ_SOURCE kDmaRequestMux0LPI2C3Rx
#define DEMO_LPI2C_TX_DMA_REQ_SOURCE kDmaRequestMux0LPI2C3Tx

#define DEMO_LPI2C_CLOCK_FREQUNCY CLOCK_GetIpFreq(DEMO_ACCEL_I2C_CLOCK_NAME)
#define DEMO_LPIT_CLOCK_SOURCE    kCLOCK_IpSrcSircAsync
#define DEMO_LPIT_CLOCK_FREQUNCY  CLOCK_GetIpFreq(kCLOCK_Lpit0)

#define DMAMUX0 DMA_CH_MUX0
/* Accelerometer I2C address*/
#define ACCEL_I2C_ADDR 0x1EU
/* Accelerometer Device identification register read value*/
#define ACCEL_WHO_AM_I 0xC7U

/*on evkmcimx7ulp board, the reset is conected to 1.8v ,active high*/
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
