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
#define EXAMPLE_SPIFI        SPIFI0
#define PAGE_SIZE            (256)
#define SECTOR_SIZE          (4096)
#define EXAMPLE_SPI_BAUDRATE (32000000)
#define kFRO_HF_to_SPIFI_CLK kFRO_HF_to_SPIFI
#define SPIFI0_IRQn          SPIFI_IRQn
/* Use MX25R6435FM2IL0 Flash */
#define FLASH_MX25R
/* Use fast_read replace quad mode */
#define FAST_READ
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
