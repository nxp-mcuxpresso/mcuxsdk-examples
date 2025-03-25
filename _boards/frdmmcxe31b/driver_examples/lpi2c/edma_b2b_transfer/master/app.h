/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define EXAMPLE_I2C_MASTER_BASE                   (LPI2C_1_BASE)
#define LPI2C_MASTER_CLOCK_FREQUENCY              CLOCK_GetFreq(kCLOCK_Lpi2c1Clk)
#define EXAMPLE_LPI2C_MASTER_TX_DMA_MUX           DMAMUX_1
#define EXAMPLE_LPI2C_MASTER_RX_DMA_MUX           DMAMUX_1
#define EXAMPLE_LPI2C_MASTER_DMA                  EDMA
#define LPI2CMASTER_TRANSMIT_EDMA_REQUEST_SOURCE  51U
#define LPI2CMASTER_RECEIVE_EDMA_REQUEST_SOURCE   50U
#define LPI2C_TRANSMIT_DMA_CHANNEL                16U
#define LPI2C_RECEIVE_DMA_CHANNEL                 17U
#define LPI2C_TRANSMIT_DMAMUX_CHANNEL             0U
#define LPI2C_RECEIVE_DMAMUX_CHANNEL              1U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
