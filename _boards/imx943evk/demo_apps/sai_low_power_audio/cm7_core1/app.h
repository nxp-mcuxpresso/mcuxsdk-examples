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
/* SAI and I2C instance and clock */
#define DEMO_SAI              SAI1

/* Get frequency of sai3 clock */
#define SAI_CLOCK_ROOT          kCLOCK_Sai1
#define DEMO_SAI_CLK_FREQ       CLOCK_GetRate(SAI_CLOCK_ROOT)

/* Get frequency of lpi2c clock */
#define LPI2C_MASTER_CLOCK_ROOT kCLOCK_Lpi2c3
#define DEMO_I2C_CLK_FREQ       CLOCK_GetRate(LPI2C_MASTER_CLOCK_ROOT)
#define BOARD_MASTER_CLOCK_CONFIG()
#define BOARD_SAI_RXCONFIG(config, mode)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
