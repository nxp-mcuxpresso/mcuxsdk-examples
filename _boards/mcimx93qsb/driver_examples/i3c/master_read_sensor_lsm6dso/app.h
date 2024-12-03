/*
 * Copyright 2022 NXP
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
#define EXAMPLE_I2C_BAUDRATE       400000
#define I3C_MASTER_CLOCK_ROOT      kCLOCK_Root_I3c1
#define I3C_MASTER_CLOCK_GATE      kCLOCK_I3c1
#define I3C_MASTER_CLOCK_FREQUENCY CLOCK_GetIpFreq(I3C_MASTER_CLOCK_ROOT)
#define I3C_MASTER_SLAVE_ADDR_7BIT 0x6A
#define WAIT_TIME                  1000
#define I3C_DATA_LENGTH            1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
