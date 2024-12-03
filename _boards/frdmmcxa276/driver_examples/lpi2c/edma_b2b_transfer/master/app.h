/*
 * Copyright 2024 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define EXAMPLE_I2C_MASTER_BASE          (LPI2C2)
#define LPI2C_MASTER_CLOCK_FREQUENCY     CLOCK_GetLpi2cClkFreq(2u)
#define EXAMPLE_LPI2C_MASTER_DMA         DMA0
#define LPI2C_TRANSMIT_DMA_CHANNEL       2U
#define LPI2C_RECEIVE_DMA_CHANNEL        3U
#define DEMO_LPI2C_TRANSMIT_EDMA_CHANNEL kDma0RequestLPI2C2Tx
#define DEMO_LPI2C_RECEIVE_EDMA_CHANNEL  kDma0RequestLPI2C2Rx
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
