/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_smc.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_SMC                 SMC
#define BOARD_ACCEL_I2C_BASEADDR LPI2C0

#define DEMO_ACCEL_FXLS8974         1U
#define DEMO_ACCEL_I2C_CLOCK_NAME   kCLOCK_Lpi2c0
#define DEMO_ACCEL_I2C_CLOCK_SOURCE kCLOCK_IpSrcSircAsync

#define DEMO_LPI2C_RX_DMA_REQ_SOURCE kDmaRequestMux0LPI2C0Rx
#define DEMO_LPI2C_TX_DMA_REQ_SOURCE kDmaRequestMux0LPI2C0Tx

#define DEMO_LPI2C_CLOCK_FREQUNCY CLOCK_GetIpFreq(DEMO_ACCEL_I2C_CLOCK_NAME)
#define DEMO_LPIT_CLOCK_SOURCE    kCLOCK_IpSrcSircAsync
#define DEMO_LPIT_CLOCK_FREQUNCY  CLOCK_GetIpFreq(kCLOCK_Lpit0)

#define DMAMUX0 DMAMUX
/* Accelerometer I2C address*/
#define ACCEL_I2C_ADDR 0x18U
/* Accelerometer Device identification register read value*/
#define ACCEL_WHO_AM_I 0x86U

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
