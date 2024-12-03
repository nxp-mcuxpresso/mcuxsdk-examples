/*
 * Copyright 2023 NXP
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
#define DEMO_IAP_FLASH_SECTOR                (30)
#define DEMO_IAP_FLASH_NUMBER_OF_WAIT_STATES (2)
#define DEMO_IAP_FLASH_ACCESS_TIME           kFlash_IAP_TwoSystemClockTime
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_CORE0_H_ */
