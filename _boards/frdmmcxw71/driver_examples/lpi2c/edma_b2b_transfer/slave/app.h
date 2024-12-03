/*
 * Copyright 2024 NXP
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
#if defined(BOARD_EBB)
#define EXAMPLE_I2C_SLAVE_BASE      LPI2C0
#define LPI2C_SLAVE_CLOCK           kCLOCK_Lpi2c0
#define LPI2C_SLAVE_CLOCK_FREQUENCY (CLOCK_GetIpFreq(kCLOCK_Lpi2c0))
#else
#define EXAMPLE_I2C_SLAVE_BASE      LPI2C1
#define LPI2C_SLAVE_CLOCK           kCLOCK_Lpi2c1
#define LPI2C_SLAVE_CLOCK_FREQUENCY (CLOCK_GetIpFreq(kCLOCK_Lpi2c1))
#endif
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
