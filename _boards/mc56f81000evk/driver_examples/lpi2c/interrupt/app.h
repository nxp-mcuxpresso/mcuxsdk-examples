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
#define EXAMPLE_LPI2C_MASTER_BASEADDR LPI2C0
#define EXAMPLE_LPI2C_SLAVE_BASEADDR  LPI2C1

#define LPI2C_MASTER_CLOCK_FREQUENCY CLOCK_GetIpClkSrcFreq(kCLOCK_LPI2C0)
#define LPI2C_SLAVE_CLOCK_FREQUENCY  CLOCK_GetIpClkSrcFreq(kCLOCK_LPI2C1)

#define LPI2C_MASTER_IRQHandler ivINT_LPI2C0
#define LPI2C_SLAVE_IRQHandler  ivINT_LPI2C1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
