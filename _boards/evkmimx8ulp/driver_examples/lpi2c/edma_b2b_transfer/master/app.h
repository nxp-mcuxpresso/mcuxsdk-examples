/*
 * Copyright 2021 NXP
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
#define EXAMPLE_I2C_MASTER_BASE      (LPI2C0_BASE)
#define LPI2C_MASTER_CLOCK_FREQUENCY CLOCK_GetIpFreq(kCLOCK_Lpi2c0)

#define DEMO_LPI2C_TRANSMIT_EDMA_CHANNEL kDmaRequestMux0LPI2C0Tx
#define DEMO_LPI2C_RECEIVE_EDMA_CHANNEL  kDmaRequestMux0LPI2C0Rx
#define EXAMPLE_LPI2C_MASTER_DMA         (DMA0)
#define LPI2C_TRANSMIT_DMA_CHANNEL       0U
#define LPI2C_RECEIVE_DMA_CHANNEL        1U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
