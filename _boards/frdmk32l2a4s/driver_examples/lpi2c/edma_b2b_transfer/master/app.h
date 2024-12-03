/*
 * Copyright 2019 NXP
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
#define EXAMPLE_I2C_MASTER_BASE      (LPI2C1_BASE)
#define LPI2C_MASTER_CLOCK_FREQUENCY CLOCK_GetIpFreq(kCLOCK_Lpi2c1)

#define LPI2CMASTER_TRANSMIT_EDMA_REQUEST_SOURCE kDmaRequestMux0LPI2C1Tx
#define LPI2CMASTER_RECEIVE_EDMA_REQUEST_SOURCE  kDmaRequestMux0LPI2C1Rx
#define EXAMPLE_LPI2C_MASTER_DMA_MUX             (DMAMUX0)
#define EXAMPLE_LPI2C_MASTER_DMA                 (DMA0)
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
