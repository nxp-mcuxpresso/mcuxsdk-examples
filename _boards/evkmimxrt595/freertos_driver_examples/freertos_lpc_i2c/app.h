/*.
 * Copyright 2016-2019 NXP
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
#define EXAMPLE_I2C_MASTER_IRQ FLEXCOMM11_IRQn
#define EXAMPLE_I2C_SLAVE_IRQ  FLEXCOMM12_IRQn

#define EXAMPLE_I2C_MASTER_BASE (I2C11_BASE)
#define EXAMPLE_I2C_SLAVE_BASE  (I2C12_BASE)

#define I2C_MASTER_CLOCK_FREQUENCY CLOCK_GetFlexcommClkFreq(11)
#define I2C_SLAVE_CLOCK_FREQUENCY  CLOCK_GetFlexcommClkFreq(12)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
