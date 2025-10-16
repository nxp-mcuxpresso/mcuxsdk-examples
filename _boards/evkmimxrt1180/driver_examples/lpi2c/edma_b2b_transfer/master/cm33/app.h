/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_I2C_MASTER_BASE (LPI2C2_BASE)

/* Get frequency of lpi2c clock */
#define LPI2C_CLOCK_FREQUENCY CLOCK_GetRootClockFreq(kCLOCK_Root_Lpi2c0102)

#define LPI2C_MASTER_CLOCK_FREQUENCY LPI2C_CLOCK_FREQUENCY

#define EXAMPLE_LPI2C_MASTER_DMA (DMA3)

#define DEMO_LPI2C_TRANSMIT_EDMA_CHANNEL kDma3RequestMuxLPI2C2Tx
#define DEMO_LPI2C_RECEIVE_EDMA_CHANNEL  kDma3RequestMuxLPI2C2Rx

#define LPI2C_TRANSMIT_DMA_CHANNEL 0U
#define LPI2C_RECEIVE_DMA_CHANNEL  1U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
