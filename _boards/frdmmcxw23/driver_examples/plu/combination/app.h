/*
 * Copyright 2024 NXP
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
#define DEMO_PLU_BASE              PLU
#define DEMO_PLU_LUT_IN_SRC_0      kPLU_LUT_IN_SRC_PLU_IN_3
#define DEMO_PLU_LUT_IN_SRC_1      kPLU_LUT_IN_SRC_PLU_IN_4
#define DEMO_PLU_LUT_IN_SRC_2      kPLU_LUT_IN_SRC_PLU_IN_5
#define DEMO_PLU_LUT_OUT_SRC_0     kPLU_OUT_SRC_LUT_0
#define DEMO_PLU_LUT_OUT_SRC_1     kPLU_OUT_SRC_LUT_1
#define DEMO_PLU_LUT_OUT_SRC_2     kPLU_OUT_SRC_LUT_2
#define DEMO_PLU_LUT_0_TRUTH_TABLE 0x00 /* 0b00000000 */
#define DEMO_PLU_LUT_1_TRUTH_TABLE 0x05 /* 0b00000101 */
#define DEMO_PLU_LUT_2_TRUTH_TABLE 0x03 /* 0b00000011 */

/* GPIO module */
#define DEMO_GPIO_BASE          GPIO
#define DEMO_GPIO_PORT          0U
#define DEMO_GPIO_PLU_SRC_0_PIN 6U
#define DEMO_GPIO_PLU_SRC_1_PIN 7U
#define DEMO_GPIO_PLU_SRC_2_PIN 16U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
