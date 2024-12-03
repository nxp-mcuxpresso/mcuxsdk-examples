/*
 * Copyright 2020 NXP
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
#define EXAMPLE_I2C_MASTER_BASEADDR I2C0
#define EXAMPLE_I2C_SLAVE_BASEADDR  I2C1
#define I2C_MASTER_CLK_FREQ         CLOCK_GetIpClkSrcFreq(kCLOCK_I2C0)
#define I2C_SLAVE_CLK_FREQ          CLOCK_GetIpClkSrcFreq(kCLOCK_I2C1)
#define I2C_MASTER_IRQ              I2C0_IRQn
#define I2C_SLAVE_IRQ               I2C1_IRQn
#define I2C_MASTER_IRQHandler       ivINT_I2C0
#define I2C_SLAVE_IRQHandler        ivINT_I2C1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
