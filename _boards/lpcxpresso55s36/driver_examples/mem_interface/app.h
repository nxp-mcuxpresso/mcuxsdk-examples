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
#define PFLASH_BASE_ADDRESS (0x0U)
/* @brief Flash size in bytes */
#define PFLASH_TOTAL_SIZE_BYTES (0x3DC00U)
/* @brief Flash sector size in bytes */
#define PFLASH_SECTOR_SIZE_BYTES (0x8000U)
/* @brief Flash page size in bytes */
#define PFLASH_PAGE_SIZE_BYTES (512U)

/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
