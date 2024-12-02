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
#define EXAMPLE_I2C_MASTER_BASE      (LPI2C6_BASE)
#define LPI2C_MASTER_CLOCK_ROOT      hal_clock_lpi2c6
#define LPI2C_MASTER_CLOCK_FREQUENCY HAL_ClockGetIpFreq(LPI2C_MASTER_CLOCK_ROOT)
#define EXAMPLE_LPI2C_MASTER_DMA     (EDMA5_2)
#define LPI2C_TRANSMIT_DMA_CHANNEL  0
#define LPI2C_RECEIVE_DMA_CHANNEL   1
#define DEMO_LPI2C_TRANSMIT_EDMA_CHANNEL   (Dma5RequestMuxLPI2C6Tx & 0xFF)
#define DEMO_LPI2C_RECEIVE_EDMA_CHANNEL    (Dma5RequestMuxLPI2C6Rx & 0xFF)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
