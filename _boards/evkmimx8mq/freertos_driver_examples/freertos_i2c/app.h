/*
 * Copyright 2017 NXP
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
#define EXAMPLE_I2C_MASTER_BASE I2C2
#define EXAMPLE_I2C_MASTER_IRQN I2C2_IRQn
#define EXAMPLE_I2C_MASTER_CLK_FREQ                                                        \
    CLOCK_GetPllFreq(kCLOCK_SystemPll1Ctrl) / (CLOCK_GetRootPreDivider(kCLOCK_RootI2c2)) / \
        (CLOCK_GetRootPostDivider(kCLOCK_RootI2c2)) / 5 /* SYSTEM PLL1 DIV5 */
#define EXAMPLE_I2C_SLAVE_BASE I2C2
#define EXAMPLE_I2C_SLAVE_IRQN I2C2_IRQn
#define EXAMPLE_I2C_SLAVE_CLK_FREQ                                                         \
    CLOCK_GetPllFreq(kCLOCK_SystemPll1Ctrl) / (CLOCK_GetRootPreDivider(kCLOCK_RootI2c2)) / \
        (CLOCK_GetRootPostDivider(kCLOCK_RootI2c2)) / 5 /* SYSTEM PLL1 DIV5 */
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
