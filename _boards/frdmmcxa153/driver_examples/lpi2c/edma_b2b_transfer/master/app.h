/*
 * Copyright 2023 NXP
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

#define EXAMPLE_I2C_MASTER_BASE          (LPI2C0)
#define LPI2C_MASTER_CLOCK_FREQUENCY     CLOCK_GetLpi2cClkFreq()
#define EXAMPLE_LPI2C_MASTER_DMA         DMA0
#define LPI2C_TRANSMIT_DMA_CHANNEL       2U
#define LPI2C_RECEIVE_DMA_CHANNEL        3U
#define DEMO_LPI2C_TRANSMIT_EDMA_CHANNEL kDma0RequestLPI2C0Tx
#define DEMO_LPI2C_RECEIVE_EDMA_CHANNEL  kDma0RequestLPI2C0Rx
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
