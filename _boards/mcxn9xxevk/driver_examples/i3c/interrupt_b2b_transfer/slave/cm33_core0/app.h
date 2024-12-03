/*
 * Copyright 2022-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_SLAVE              I3C1
#define I3C_SLAVE_CLOCK_FREQUENCY  CLOCK_GetFreq(kCLOCK_Clk1M)
#define I3C_MASTER_SLAVE_ADDR_7BIT 0x1EU
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
