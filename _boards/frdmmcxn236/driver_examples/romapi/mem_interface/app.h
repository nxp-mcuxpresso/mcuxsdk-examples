/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "pin_mux.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define PFLASH_BASE_ADDRESS (0x0U)
/* @brief Flash size in bytes */
#define PFLASH_TOTAL_SIZE_BYTES (0x100000U)
/* @brief Flash sector size in bytes */
#define PFLASH_SECTOR_SIZE_BYTES (0x2000U)
/* @brief Flash page size in bytes */
#define PFLASH_PAGE_SIZE_BYTES (128U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
