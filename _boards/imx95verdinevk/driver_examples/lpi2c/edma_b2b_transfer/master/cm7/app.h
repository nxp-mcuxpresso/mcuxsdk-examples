/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_I2C_MASTER_BASE      (LPI2C2)
#define LPI2C_MASTER_CLOCK_ROOT      hal_clock_lpi2c2
#define LPI2C_MASTER_CLOCK_FREQUENCY HAL_ClockGetIpFreq(LPI2C_MASTER_CLOCK_ROOT)
#define EXAMPLE_LPI2C_MASTER_DMA     (DMA3)
#define LPI2C_TRANSMIT_DMA_CHANNEL  14
#define LPI2C_RECEIVE_DMA_CHANNEL   15
#define DEMO_LPI2C_TRANSMIT_EDMA_CHANNEL   Dma3RequestMuxLPSPI2Tx
#define DEMO_LPI2C_RECEIVE_EDMA_CHANNEL    Dma3RequestMuxLPSPI2Rx
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
