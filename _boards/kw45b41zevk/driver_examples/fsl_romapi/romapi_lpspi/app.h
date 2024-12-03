/*
 * Copyright 2021 NXP
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
#define LPSPI_BAUD_RATE         4000000
#define LPSPI_FLASH_BASE        0x0
#define LPSPI_FLASH_PAGE_SIZE   256
#define LPSPI_FLASH_SECTOR_SIZE 4096
#define LPSPI_FLASH_TOTAL_SIZE  0x200000
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
