/*
 * Copyright 2021 NXP
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
#define EXAMPLE_LPI2C_MASTER_BASEADDR LPI2C0
#define EXAMPLE_LPI2C_SLAVE_BASEADDR  LPI2C1
#define LPI2C_MASTER_IRQ              LPI2C0_IRQn
#define LPI2C_SLAVE_IRQ               LPI2C1_IRQn
#define LPI2C_MASTER_IRQHandler       LPI2C0_IRQHandler
#define LPI2C_SLAVE_IRQHandler        LPI2C1_IRQHandler
#define LPI2C_MASTER_CLOCK_FREQUENCY  (CLOCK_GetIpFreq(kCLOCK_Lpi2c0))
#define LPI2C_SLAVE_CLOCK_FREQUENCY   (CLOCK_GetIpFreq(kCLOCK_Lpi2c1))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
