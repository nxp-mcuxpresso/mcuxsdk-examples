/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_lpuart_cmsis.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_ADC16_BASEADDR      ADC0
#define DEMO_ADC16_CHANNEL_GROUP 0U
#define DEMO_ADC16_USER_CHANNEL  8U /* PTB0, ADC0_SE8 */

#define DEMO_ADC16_IRQn             ADC0_IRQn
#define DEMO_ADC16_IRQ_HANDLER_FUNC ADC0_IRQHandler

#define BOARD_LED_GPIO     BOARD_LED_GREEN_GPIO
#define BOARD_LED_GPIO_PIN BOARD_LED_GREEN_GPIO_PIN

/* I2C source clock */

#define I2C_BAUDRATE   100000U
#define ACCEL_I2C_ADDR 1DU

/* Accelerometer Reset PIN */
#define BOARD_ACCEL_RESET_GPIO GPIOE
#define BOARD_ACCEL_RESET_PIN  1U

#define ERPC_DEMO_UART                Driver_USART0
#define ERPC_DEMO_UART_RX_TX_IRQn     BOARD_UART_IRQ
#define ERPC_DEMO_UART_RX_TX_IRQ_PRIO (2U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_ACCEL_Reset(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
