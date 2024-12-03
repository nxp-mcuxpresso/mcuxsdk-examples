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
#define EXAMPLE_I2C_MASTER_BASE      (LPI2C1_BASE)
#define LPI2C_MASTER_CLOCK_ROOT      kCLOCK_Root_Lpi2c1
#define LPI2C_MASTER_CLOCK_GATE      kCLOCK_Lpi2c1
#define LPI2C_MASTER_CLOCK_FREQUENCY CLOCK_GetIpFreq(LPI2C_MASTER_CLOCK_ROOT)

#define EXAMPLE_LPI2C_MASTER_DMA         (DMA3)
#define LPI2C_TRANSMIT_DMA_CHANNEL       kDma3RequestMuxLPI2C1Tx
#define LPI2C_RECEIVE_DMA_CHANNEL        kDma3RequestMuxLPI2C1Rx
#define DEMO_LPI2C_TRANSMIT_EDMA_CHANNEL (-1)
#define DEMO_LPI2C_RECEIVE_EDMA_CHANNEL  (-1)
#define EXAMPLE_DMA_CLOCK_ROOT           kCLOCK_Root_WakeupAxi
#define EXAMPLE_DMA_CLOCK_GATE           kCLOCK_Edma1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
