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
#define EXAMPLE_SPI_MASTER          SPI1
#define EXAMPLE_SPI_MASTER_IRQ      FLEXCOMM1_IRQn
#define EXAMPLE_SPI_MASTER_CLK_SRC  kCLOCK_Flexcomm1
#define EXAMPLE_SPI_MASTER_CLK_FREQ CLOCK_GetFlexCommClkFreq(1U)
#define EXAMPLE_SPI_SLAVE           SPI2
#define EXAMPLE_SPI_SLAVE_IRQ       FLEXCOMM2_IRQn
#define EXAMPLE_SPI_SSEL            1
#define SPI_MASTER_IRQHandler       FLEXCOMM1_IRQHandler
#define SPI_SLAVE_IRQHandler        FLEXCOMM2_IRQHandler
#define EXAMPLE_SPI_SPOL            SPI_CFG_SPOL1(0)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
