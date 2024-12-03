/*
 * Copyright 2020 NXP
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
#define EXAMPLE_SPI_MASTER            SPI8
#define EXAMPLE_SPI_MASTER_IRQ        LSPI_HS_IRQn
#define EXAMPLE_SPI_MASTER_CLK_SRC    kCLOCK_Hs_Lspi
#define EXAMPLE_SPI_MASTER_CLK_FREQ   CLOCK_GetHsLspiClkFreq()
#define EXAMPLE_SPI_SSEL              1
#define EXAMPLE_MASTER_SPI_SPOL       kSPI_SpolActiveAllLow
#define BUFFER_SIZE                   (2)
#define DEMO_PINT_PIN_INT1_SRC        kINPUTMUX_GpioPort1Pin18ToPintsel
#define EXAMPLE_HSPI_CS_LOW_GPIO_GPIO BOARD_INITPINS_SPI_CS_GPIO_GPIO
#define EXAMPLE_HSPI_CS_LOW_GPIO_PORT BOARD_INITPINS_SPI_CS_GPIO_PORT
#define EXAMPLE_HSPI_CS_LOW_GPIO_PIN  BOARD_INITPINS_SPI_CS_GPIO_PIN
#define EXAMPLE_BOARD_NAME            "LPCXpresso55s16"
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
