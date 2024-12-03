/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
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
#define DEMO_IAP_FLASH_SECTOR (1)
#define DEMO_IAP_FLASH_NUMBER_OF_WAIT_STATES \
    ((SYSCON->FLASHCFG & SYSCON_FLASHCFG_FLASHTIM_MASK) >> SYSCON_FLASHCFG_FLASHTIM_SHIFT)
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_CORE0_H_ */
