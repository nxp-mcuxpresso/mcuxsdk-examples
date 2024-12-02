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
#define APP_FLASH_SPI (LPSPI6)
#define LPSPI_MASTER_CLK_FREQ CLOCK_GetLPFlexCommClkFreq(6u)
/* The flash can use up to 133MHz clock, chose the one as fast as possible. */
#define LPSPI_BAUDRATE (30 * 1000 * 1000)

#define APP_LPSPI_CS_GPIO GPIO3
#define APP_LPSPI_CS_PIN  23

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void SPI_Flash_PullCS(uint8_t level);
/*${prototype:end}*/

#endif
