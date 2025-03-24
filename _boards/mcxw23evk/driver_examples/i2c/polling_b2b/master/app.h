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
/** I2C master base */
#define EXAMPLE_I2C_MASTER_BASE ((I2C_Type *)I2C1_BASE)
/** I2C master clock frequency */
#define I2C_MASTER_CLOCK_FREQUENCY (12000000)
#define WAIT_TIME                  10U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
