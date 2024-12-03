/*
 * Copyright 2023 NXP
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
#define I3C_SLAVE_CLOCK_ROOT       kCLOCK_Root_I3c1
#define I3C_SLAVE_CLOCK_GATE       kCLOCK_I3c1
#define I3C_SLAVE_CLOCK_FREQUENCY  CLOCK_GetIpFreq(I3C_SLAVE_CLOCK_ROOT)
#define I3C_TIME_OUT_INDEX         210000000
#define I3C_MASTER_SLAVE_ADDR_7BIT 0x1EU
#define I3C_DATA_LENGTH            34U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
