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
#define FLASH_PAGE_SIZE         256
#define NOR_FLASH_START_ADDRESS FSL_FEATURE_SPIFI_START_ADDR
#define EXAMPLE_DISABLE_FLASH_CHIP_ERASE 1 /* Don't erase full flash, it needs long time. */
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
