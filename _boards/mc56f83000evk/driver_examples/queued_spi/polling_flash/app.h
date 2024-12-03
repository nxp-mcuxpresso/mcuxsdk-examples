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
#define SECTOR_ADDR    0
#define SECTOR_SIZE    4096
#define FLASH_SPI_SSEL 0

#define EXAMPLE_QSPI_MASTER_BASEADDR QSPI1
#define EXAMPLE_QSPI_CLK_FREQ        CLOCK_GetIpClkSrcFreq(kCLOCK_QSPI1)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
