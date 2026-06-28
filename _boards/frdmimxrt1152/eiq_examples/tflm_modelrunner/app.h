/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define KTENSOR_ARENA_SIZE_MEM 1024 * 1024 * 20
#define KTENSOR_ARENA_SIZE_FLASH 1024 * 1024 * 40

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _APP_H_ */
