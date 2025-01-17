/*
 * Copyright 2021 NXP
 * All rights reserved.
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
#define EXAMPLE_I2C_MASTER_BASE LPI2C1_BASE
#define EXAMPLE_I2C_SLAVE_BASE  LPI2C3_BASE

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define LPI2C_CLOCK_FREQUENCY ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

#define LPI2C_MASTER_CLOCK_FREQUENCY LPI2C_CLOCK_FREQUENCY
#define LPI2C_SLAVE_CLOCK_FREQUENCY  LPI2C_CLOCK_FREQUENCY

#define EXAMPLE_LPI2C_MASTER_DMA_MUX (DMAMUX)
#define EXAMPLE_LPI2C_MASTER_DMA     (DMA0)

#define LPI2CMASTER_TRANSMIT_EDMA_REQUEST_SOURCE kDmaRequestMuxLPI2C1
#define LPI2CMASTER_RECEIVE_EDMA_REQUEST_SOURCE  kDmaRequestMuxLPI2C1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
