/*
 * Copyright 2019 NXP
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
#define EXAMPLE_LPI2C_MASTER_BASEADDR LPI2C1
#define EXAMPLE_LPI2C_SLAVE_BASEADDR  LPI2C2
#define LPI2C_MASTER_IRQ              LPI2C1_IRQn
#define LPI2C_SLAVE_IRQ               LPI2C2_IRQn
#define LPI2C_MASTER_IRQHandler       LPI2C1_IRQHandler
#define LPI2C_SLAVE_IRQHandler        LPI2C2_IRQHandler
#define LPI2C_MASTER_CLOCK_FREQUENCY  (CLOCK_GetIpFreq(kCLOCK_Lpi2c1))
#define LPI2C_SLAVE_CLOCK_FREQUENCY   (CLOCK_GetIpFreq(kCLOCK_Lpi2c2))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
