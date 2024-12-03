/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_I2C_MASTER                       Driver_I2C0
#define EXAMPLE_I2C_SLAVE                        Driver_I2C1
#define EXAMPLE_I2C_MASTER_BASE                  (LPI2C0_BASE)
#define EXAMPLE_I2C_SLAVE_BASE                   (LPI2C1_BASE)
#define LPI2C_MASTER_CLOCK_FREQUENCY             (CLOCK_GetIpFreq(kCLOCK_Lpi2c0))
#define LPI2C_SLAVE_CLOCK_FREQUENCY              (CLOCK_GetIpFreq(kCLOCK_Lpi2c1))
#define LPI2CMASTER_TRANSMIT_EDMA_REQUEST_SOURCE kDmaRequestMux0LPI2C0Tx
#define LPI2CMASTER_RECEIVE_EDMA_REQUEST_SOURCE  kDmaRequestMux0LPI2C0Rx
#define EXAMPLE_LPI2C_DMAMUX_BASEADDR            DMAMUX
#define EXAMPLE_LPI2C_DMA_BASEADDR               DMA0

#define I2C_SLAVE_IRQ LPI2C1_IRQn
#define DMA0_IRQn     DMA0_04_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
