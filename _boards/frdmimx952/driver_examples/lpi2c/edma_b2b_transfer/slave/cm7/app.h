/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_I2C_SLAVE_BASE      (LPI2C6_BASE)
#define LPI2C_SLAVE_CLOCK_ROOT      kCLOCK_lpi2c6
#define LPI2C_SLAVE_CLOCK_FREQUENCY CLOCK_GetRate(LPI2C_SLAVE_CLOCK_ROOT)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
