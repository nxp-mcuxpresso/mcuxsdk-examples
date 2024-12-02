/*
 * Copyright 2017 - 2018 , NXP
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
#define DEMO_CODE_START_NS 0x00010000

#define DEMO_SECURE_SW1_GPIO      SECGPIO
#define DEMO_SECURE_SW1_GPIO_PORT 0U
#define DEMO_SECURE_SW1_GPIO_PIN  5U

#define DEMO_SW1_GPIO      BOARD_SW4_GPIO
#define DEMO_SW1_GPIO_PORT BOARD_SW4_GPIO_PORT
#define DEMO_SW1_GPIO_PIN  BOARD_SW4_GPIO_PIN

#define DEMO_SW2_GPIO      BOARD_SW1_GPIO
#define DEMO_SW2_GPIO_PORT BOARD_SW1_GPIO_PORT
#define DEMO_SW2_GPIO_PIN  BOARD_SW1_GPIO_PIN

#define DEMO_BLUE_LED_PIN_SEC_MASK AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN5_SEC_MASK_MASK

#define DEMO_SECURE_GPIO_CLOCK kCLOCK_Gpio_Sec
#define DEMO_SECURE_GPIO_RST   kGPIOSEC_RST_SHIFT_RSTn

#define DEMO_SYSTICK_CLK_FREQ CLOCK_GetSystickClkFreq(0U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
