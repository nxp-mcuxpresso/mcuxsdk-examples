/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* I2C source clock */
#define EXAMPLE_I2C_MASTER_BASE     (I2C0_BASE)
#define EXAMPLE_I2C_MASTER_BASEADDR (I2C0)
#define EXAMPLE_I2C_MASTER_IRQN     (I2C0_I2C1_IRQn)
#define EXAMPLE_I2C_MASTER_CLK_SRC  (I2C0_CLK_SRC)
#define EXAMPLE_I2C_MASTER_CLK_FREQ CLOCK_GetFreq((I2C0_CLK_SRC))

#define EXAMPLE_I2C_SLAVE_BASE     (I2C1_BASE)
#define EXAMPLE_I2C_SLAVE_BASEADDR (I2C1)
#define EXAMPLE_I2C_SLAVE_IRQN     (I2C0_I2C1_IRQn)
#define EXAMPLE_I2C_SLAVE_CLK_SRC  (I2C0_CLK_SRC)
#define EXAMPLE_I2C_SLAVE_CLK_FREQ CLOCK_GetFreq((I2C0_CLK_SRC))

#define SINGLE_BOARD   0
#define BOARD_TO_BOARD 1

#define EXAMPLE_CONNECT_I2C BOARD_TO_BOARD
#if (EXAMPLE_CONNECT_I2C == BOARD_TO_BOARD)
#define isMASTER         0
#define isSLAVE          1
#define I2C_MASTER_SLAVE isMASTER
#endif
#if (EXAMPLE_CONNECT_I2C == BOARD_TO_BOARD)
#define EXAMPLE_I2C_DEALY_COUNT 1000u
#endif
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
