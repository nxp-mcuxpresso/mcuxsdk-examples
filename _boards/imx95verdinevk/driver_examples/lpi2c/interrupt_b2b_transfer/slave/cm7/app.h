/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "hal_clock.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_I2C_SLAVE_BASE      (LPI2C2)
#define LPI2C_SLAVE_CLOCK_ROOT      hal_clock_lpi2c2
#define LPI2C_SLAVE_CLOCK_FREQUENCY HAL_ClockGetIpFreq(LPI2C_SLAVE_CLOCK_ROOT)
#define EXAMPLE_LPI2C_NO_TIMEOUT
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
