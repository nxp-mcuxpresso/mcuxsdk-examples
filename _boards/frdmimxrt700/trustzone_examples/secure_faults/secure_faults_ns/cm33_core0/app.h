/*
 * Copyright 2026 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_SEC_ADDRESS 0x30000000

/* 0x20200000 - 0x20207FFF is configured Non-Secure SRAM in SAU.
   0x20200000 - 0x20207FFF is configured secure in AHB_SECURE_CTRL. */
#define DEMO_NONSEC_ADDRESS 0x20200000
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
