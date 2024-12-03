/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_I2C_MASTER_BASE      LPI2C1
#define LPI2C_MASTER_CLOCK           kCLOCK_Lpi2c1
#define LPI2C_MASTER_CLOCK_FREQUENCY (CLOCK_GetIpFreq(kCLOCK_Lpi2c1))
#define LPI2C_MASTER_IRQ             LPI2C1_IRQn
#define WAIT_TIME                    10U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
