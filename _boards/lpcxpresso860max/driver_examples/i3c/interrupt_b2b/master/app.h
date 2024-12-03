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
#define EXAMPLE_MASTER          I3C0
#define EXAMPLE_I2C_BAUDRATE    100000
#define EXAMPLE_I3C_OD_BAUDRATE 400000
/* PP baudrate should be set equal to I3C FCLK dividing even number. */
#define EXAMPLE_I3C_PP_BAUDRATE    1875000
#define I3C_MASTER_CLOCK_FREQUENCY CLOCK_GetI3cClkFreq()
#define I3C_MASTER_SLAVE_ADDR_7BIT 0x1E
#define I3C_DATA_LENGTH            33
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
