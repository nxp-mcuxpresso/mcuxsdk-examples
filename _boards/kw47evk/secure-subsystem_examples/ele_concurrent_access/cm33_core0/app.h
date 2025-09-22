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

/* Address where the image for core1 starts */
#define CORE1_BOOT_ADDRESS 0x48800000

#define CORE0_DOMAIN_ID 0      /* SoC-defined domain ID for Core 0 */
#define APP_SEMA42      SEMA42
#define APP_SEMA42_GATE 0      /* This is the same between both cores */

/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);

/*${prototype:end}*/

#endif /* _APP_H_ */
