/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_CORE0_H_
#define _APP_CORE0_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define FlexSpiInstance           0U
#define EXAMPLE_FLEXSPI_AMBA_BASE FlexSPI0_AMBA_BASE
#define FLASH_SIZE                0x02000000UL /* 32MBytes */
#define FLASH_PAGE_SIZE           256UL        /* 256Bytes */
#define FLASH_SECTOR_SIZE         0x1000UL     /* 4KBytes */
#define FLASH_BLOCK_SIZE          0x10000UL    /* 64KBytes */
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_CORE0_H_ */
