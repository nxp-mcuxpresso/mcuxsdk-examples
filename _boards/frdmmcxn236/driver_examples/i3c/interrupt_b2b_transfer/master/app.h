/*
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_MASTER             I3C1
#define EXAMPLE_I2C_BAUDRATE       100000
#define EXAMPLE_I3C_OD_BAUDRATE    312500
#define EXAMPLE_I3C_PP_BAUDRATE    625000
#define I3C_MASTER_CLOCK_FREQUENCY CLOCK_GetI3cClkFreq(1U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
