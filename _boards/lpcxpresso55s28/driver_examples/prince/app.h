/*
 * Copyright 2018 NXP
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
#define APP_ENCRYPTION_START_ADDR (0x40000)
#define APP_ENCRYPTION_LENGTH     (0x2000)

#define APP_PUF_CORE_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
/* Worst-case time in ms to fully discharge PUF SRAM */
#define APP_PUF_DISCHARGE_TIME 400
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
