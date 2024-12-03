/*
 * Copyright 2017 NXP
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
/* I2C source clock */
#define I2C_SLAVE_CLK_SRC          kCLOCK_BusClk
#define I2C_SLAVE_CLK_FREQ         CLOCK_GetFreq(kCLOCK_BusClk)
#define EXAMPLE_I2C_SLAVE_BASEADDR I2C0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
