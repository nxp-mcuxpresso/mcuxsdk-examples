/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* Flash address where the secondary core (M7_1) image is programmed and runs
 * in place. The secondary core is released by writing this address to the
 * MC_ME PRTN0_CORE1 interface; the image is not copied to RAM. */
#define CORE1_BOOT_ADDRESS 0x00600000
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
/*******************************************************************************
 * EOF
 ******************************************************************************/
