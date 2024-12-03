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
#define DEMO_SYSTICK_CLK_FREQ (CLOCK_GetFreq(kCLOCK_RtcOscClk) * 6)
#define TEST_SW7_GPIO         GPIOB
#define TEST_SW8_GPIO         GPIOB
#define TEST_NS_GPIOA         GPIOA

#define TEST_SW7_GPIO_PIN 13U
#define TEST_SW8_GPIO_PIN 12U

#define TEST_GPIOA_PIN15 15U
#define TEST_GPIOA_PIN18 18U

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_SetTrdcGlobalConfig(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
