/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define EXAMPLE_SPIFI         SPIFI0
#define PAGE_SIZE             (256)
#define SECTOR_SIZE           (4096)
#define EXAMPLE_SPI_BAUDRATE  (96000000)
#define EXAMPLE_DMA           DMA0
#define EXAMPLE_SPIFI_CHANNEL 18
#define FLASH_W25Q
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
