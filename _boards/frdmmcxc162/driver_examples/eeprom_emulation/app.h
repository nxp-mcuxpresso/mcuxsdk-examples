/*
 * Copyright 2026 NXP
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
#define FLASH_SECTOR_SIZE FSL_FEATURE_FLASH_PFLASH_SECTOR_SIZE
#define FLASH_WRITE_SIZE  FSL_FEATURE_FLASH_PFLASH_PHRASE_SIZE
#define FLASH_BASE        FSL_FEATURE_FLASH_PFLASH_START_ADDRESS
#define FLASH_SIZE        (FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE * FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
