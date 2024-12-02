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
#define DEMO_SYSTICK_CLK_FREQ      CLOCK_GetSystickClkFreq()
#define DEMO_SECURE_SW1_GPIO       SECGPIO
#define DEMO_SECURE_SW1_GPIO_PORT  0U
#define DEMO_SECURE_SW1_GPIO_PIN   10U
#define DEMO_SW1_GPIO              GPIO
#define DEMO_SW1_GPIO_PORT         0U
#define DEMO_SW1_GPIO_PIN          10U
#define DEMO_SW2_GPIO              GPIO
#define DEMO_SW2_GPIO_PORT         1U
#define DEMO_SW2_GPIO_PIN          1U
#define DEMO_SECURE_GPIO_CLOCK     kCLOCK_ShsGpio0
#define DEMO_SECURE_GPIO_RST       kSHSGPIO0_RST_SHIFT_RSTn
#define DEMO_BLUE_LED_PIN_SEC_MASK AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN10_SEC_MASK_MASK

#if (DEMO_CODE_START_NS == 0x08100000U)
#define BOARD_InitTrustZone XIP_BOARD_InitTrustZone
#else
#define BOARD_InitTrustZone RAM_BOARD_InitTrustZone
#endif
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
