/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_I2C_MASTER_BASE                  (LPI2C3_BASE)
#define LPI2C_MASTER_CLOCK_FREQUENCY             (CLOCK_GetIpFreq(kCLOCK_Lpi2c3))
#define LPI2CMASTER_TRANSMIT_EDMA_REQUEST_SOURCE kDmaRequestMux1LPI2C3Tx
#define LPI2CMASTER_RECEIVE_EDMA_REQUEST_SOURCE  kDmaRequestMux1LPI2C3Rx
#define EXAMPLE_LPI2C_MASTER_DMA_MUX             (DMAMUX1)
#define EXAMPLE_LPI2C_MASTER_DMA                 (DMA1)
#define LPI2C_TRANSMIT_DMA_CHANNEL               0U
#define LPI2C_RECEIVE_DMA_CHANNEL                1U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
