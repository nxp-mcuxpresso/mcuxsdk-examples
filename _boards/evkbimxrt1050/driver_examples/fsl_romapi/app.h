/*
 * Copyright 2020-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_romapi.h"
/*${header:start}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define FlexSpiInstance           0U
#define EXAMPLE_FLEXSPI_AMBA_BASE FlexSPI_AMBA_BASE
#define FLASH_SIZE                0x4000000UL /* 64MBytes */
#define FLASH_PAGE_SIZE           512UL       /* 512Bytes */
#define FLASH_SECTOR_SIZE         0x40000UL   /* 256KBytes */
#define FLASH_BLOCK_SIZE          0x40000UL   /* 256KBytes */
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
#if !(defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1))
void BOARD_SetupFlexSpiClock(void);
#endif
/* Get FlexSPI NOR Configuration Block */
void FLEXSPI_NorFlash_GetConfig(flexspi_nor_config_t *config);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
