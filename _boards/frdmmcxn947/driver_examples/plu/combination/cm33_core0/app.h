/*
 * Copyright 2022 NXP
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

/* PLU module */
#define DEMO_PLU_BASE              PLU0
#define DEMO_PLU_LUT_IN_SRC_0      kPLU_LUT_IN_SRC_PLU_IN_2
#define DEMO_PLU_LUT_IN_SRC_1      kPLU_LUT_IN_SRC_PLU_IN_3
#define DEMO_PLU_LUT_IN_SRC_2      kPLU_LUT_IN_SRC_PLU_IN_5
#define DEMO_PLU_LUT_OUT_SRC_0     kPLU_OUT_SRC_LUT_0
#define DEMO_PLU_LUT_OUT_SRC_1     kPLU_OUT_SRC_LUT_1
#define DEMO_PLU_LUT_OUT_SRC_2     kPLU_OUT_SRC_LUT_2
#define DEMO_PLU_LUT_0_TRUTH_TABLE 0x000004D /* 0b01001101 */
#define DEMO_PLU_LUT_1_TRUTH_TABLE 0x000002B /* 0b00101011 */
#define DEMO_PLU_LUT_2_TRUTH_TABLE 0x0000017 /* 0b00010111 */

/* GPIO module */
#define DEMO_NO_HAS_GPIO_PORT_PARAM 1U
#define DEMO_GPIO_BASE              GPIO4
#define DEMO_GPIO_PLU_SRC_0_PIN     18U
#define DEMO_GPIO_PLU_SRC_1_PIN     19U
#define DEMO_GPIO_PLU_SRC_2_PIN     20U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
