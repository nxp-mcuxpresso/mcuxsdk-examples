/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_I2C_MASTER LPI2C2
#define EXAMPLE_I2C_SLAVE  LPI2C3

#define EXAMPLE_LPUART1 LPUART2
#define EXAMPLE_LPUART2 LPUART3

#define EXAMPLE_LPFLEXCOMM_INSTANCE1 2U
#define EXAMPLE_LPFLEXCOMM_INSTANCE2 3U

/* Get frequency of lpi2c clock */
#define EXAMPLE_CLOCK_FREQUENCY CLOCK_GetLPFlexCommClkFreq(2u)

#define EXAMPLE_LPI2C_MASTER_CLOCK_FREQUENCY EXAMPLE_CLOCK_FREQUENCY
#define EXAMPLE_LPI2C_SLAVE_CLOCK_FREQUENCY  EXAMPLE_CLOCK_FREQUENCY
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
