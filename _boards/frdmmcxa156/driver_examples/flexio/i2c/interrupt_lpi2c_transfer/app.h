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
#define BOARD_FLEXIO_BASE FLEXIO0

#define I2C_BAUDRATE 400000

#define FLEXIO_CLOCK_FREQUENCY CLOCK_GetFlexioClkFreq()

#define FLEXIO_I2C_SDA_PIN 0U
#define FLEXIO_I2C_SCL_PIN 1U

#define BOARD_LPI2C_SLAVE_BASE LPI2C3
#define BOARD_LPI2C_SLAVE_IRQn LPI2C3_IRQn

#define LPI2C_CLOCK_FREQUENCY CLOCK_GetLpi2cClkFreq(3U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
