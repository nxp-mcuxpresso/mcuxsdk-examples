/*
 * Copyright 2023 NXP
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
#define EXAMPLE_LPI2C_MASTER_BASEADDR LPI2C2
#define EXAMPLE_LPI2C_SLAVE_BASEADDR  LPI2C1

/* Get frequency of lpi2c clock */
#define LPI2C_CLOCK_FREQUENCY CLOCK_GetLPFlexCommClkFreq(1u)

#define LPI2C_MASTER_CLOCK_FREQUENCY LPI2C_CLOCK_FREQUENCY
#define LPI2C_SLAVE_CLOCK_FREQUENCY  LPI2C_CLOCK_FREQUENCY

#define LPI2C_MASTER_IRQ LP_FLEXCOMM2_IRQn
#define LPI2C_SLAVE_IRQ  LP_FLEXCOMM1_IRQn

#define LPI2C_MASTER_IRQHandler LP_FLEXCOMM2_IRQHandler
#define LPI2C_SLAVE_IRQHandler  LP_FLEXCOMM1_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
