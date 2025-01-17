/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "fsl_gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief FRDM-TOUCH board defines */

/* Push buttons - mutual electrodes */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_ELECTRODE_1  NT_TSI_TRANSFORM_MUTUAL(FRDM_TOUCH_BOARD_TSI_MUTUAL_RX_ELECTRODE_1,FRDM_TOUCH_BOARD_TSI_MUTUAL_TX_ELECTRODE_1)//TF_TSI_MUTUAL_CAP_TX_CHANNEL_3      /* PTE11 */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_ELECTRODE_2  NT_TSI_TRANSFORM_MUTUAL(FRDM_TOUCH_BOARD_TSI_MUTUAL_RX_ELECTRODE_2,FRDM_TOUCH_BOARD_TSI_MUTUAL_TX_ELECTRODE_2)//TF_TSI_MUTUAL_CAP_TX_CHANNEL_3      /* PTE11 */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_ELECTRODE_3  NT_TSI_TRANSFORM_MUTUAL(FRDM_TOUCH_BOARD_TSI_MUTUAL_RX_ELECTRODE_3,FRDM_TOUCH_BOARD_TSI_MUTUAL_TX_ELECTRODE_3)//TF_TSI_MUTUAL_CAP_TX_CHANNEL_2      /* PTE10 */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_ELECTRODE_4  NT_TSI_TRANSFORM_MUTUAL(FRDM_TOUCH_BOARD_TSI_MUTUAL_RX_ELECTRODE_4,FRDM_TOUCH_BOARD_TSI_MUTUAL_TX_ELECTRODE_4)//TF_TSI_MUTUAL_CAP_TX_CHANNEL_2      /* PTE10 */

#define FRDM_TOUCH_BOARD_TSI_MUTUAL_TX_ELECTRODE_1  TF_TSI_MUTUAL_CAP_TX_CHANNEL_3      /* PTE11 */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_RX_ELECTRODE_1  TF_TSI_MUTUAL_CAP_RX_CHANNEL_6      /* PTD5  */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_TX_ELECTRODE_2  TF_TSI_MUTUAL_CAP_TX_CHANNEL_3      /* PTE11 */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_RX_ELECTRODE_2  TF_TSI_MUTUAL_CAP_RX_CHANNEL_7      /* PTD6  */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_TX_ELECTRODE_3  TF_TSI_MUTUAL_CAP_TX_CHANNEL_2      /* PTE10 */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_RX_ELECTRODE_3  TF_TSI_MUTUAL_CAP_RX_CHANNEL_6      /* PTD5  */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_TX_ELECTRODE_4  TF_TSI_MUTUAL_CAP_TX_CHANNEL_2      /* PTE10 */
#define FRDM_TOUCH_BOARD_TSI_MUTUAL_RX_ELECTRODE_4  TF_TSI_MUTUAL_CAP_RX_CHANNEL_7      /* PTD6  */

/* Slider - self electrodes */
#define FRDM_TOUCH_BOARD_TSI_SLIDER_ELECTRODE_1  TF_TSI_SELF_CAP_CHANNEL_10     /* PTD7 */
#define FRDM_TOUCH_BOARD_TSI_SLIDER_ELECTRODE_2  TF_TSI_SELF_CAP_CHANNEL_5      /* PTD1 */
#define FRDM_TOUCH_BOARD_TSI_1  TF_TSI_SELF_CAP_CHANNEL_22                      /* PTC0 */
#define FRDM_TOUCH_BOARD_TSI_2  TF_TSI_SELF_CAP_CHANNEL_23                      /* PTC1 */  

/* Rotary - self electrodes */
#define FRDM_TOUCH_BOARD_TSI_ROTARY_ELECTRODE_1  TF_TSI_SELF_CAP_CHANNEL_0      /* PTE5 */
#define FRDM_TOUCH_BOARD_TSI_ROTARY_ELECTRODE_2  TF_TSI_SELF_CAP_CHANNEL_1      /* PTE4 */
#define FRDM_TOUCH_BOARD_TSI_ROTARY_ELECTRODE_3  TF_TSI_SELF_CAP_CHANNEL_9      /* PTB5 */
#define FRDM_TOUCH_BOARD_TSI_ROTARY_ELECTRODE_4  TF_TSI_SELF_CAP_CHANNEL_8      /* PTB4 */

/*! @brief The board name */
#define BOARD_NAME "FRDM-KE15Z"

/*! @brief The UART to use for debug messages. */
#define BOARD_USE_UART
#define BOARD_DEBUG_UART_TYPE kSerialPort_Uart
#define BOARD_DEBUG_UART_BASEADDR LPUART1
#define BOARD_DEBUG_UART_INSTANCE 1U
#define BOARD_DEBUG_UART_CLKSRC kCLOCK_IpSrcSysOscAsync
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetIpFreq(kCLOCK_Lpuart1)
#define BOARD_UART_IRQ LPUART1_IRQn
#define BOARD_UART_IRQ_HANDLER LPUART1_IRQHandler

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/*! @brief The i2c instance used for i2c connection by default */
#define BOARD_I2C_BASEADDR I2C0

/*! @brief The CMP instance/channel used for board. */
#define BOARD_CMP_BASEADDR CMP0
#define BOARD_CMP_CHANNEL 7U

/*! @brief The rtc instance used for board. */
#define BOARD_RTC_FUNC_BASEADDR RTC

/*! @brief Indexes of the TSI channels for on-board electrodes */
#define BOARD_TSI_ELECTRODE_1 22U
#define BOARD_TSI_ELECTRODE_2 23U

/*! @brief Indexes of the TSI mutual channels for FRDM-TOUCH board */
#define BOARD_TSI_MUTUAL_TX_ELECTRODE_1  2U
#define BOARD_TSI_MUTUAL_RX_ELECTRODE_1  6U

/*! @brief Define the port interrupt number for the board switches */
#define BOARD_SW2_GPIO GPIOB
#define BOARD_SW2_PORT PORTB
#define BOARD_SW2_GPIO_PIN 11U
#define BOARD_SW2_IRQ PORTBCD_IRQn
#define BOARD_SW2_IRQ_HANDLER PORTBCD_IRQHandler
#define BOARD_SW2_NAME "SW2"

/* Board RGB LED color mapping */
#define LOGIC_LED_ON 0U
#define LOGIC_LED_OFF 1U
#define BOARD_LED_RED_GPIO FGPIOD
#define BOARD_LED_RED_GPIO_PORT PORTD
#define BOARD_LED_RED_GPIO_PIN 0U
#define BOARD_LED_GREEN_GPIO FGPIOD
#define BOARD_LED_GREEN_GPIO_PORT PORTD
#define BOARD_LED_GREEN_GPIO_PIN 16U
#define BOARD_LED_BLUE_GPIO FGPIOD
#define BOARD_LED_BLUE_GPIO_PORT PORTD
#define BOARD_LED_BLUE_GPIO_PIN 15U

#define LED_RED1_INIT(output)                                  \
    FGPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, output);\
    BOARD_LED_RED_GPIO->PDDR |= (1U << BOARD_LED_RED_GPIO_PIN)  /*!< Enable target LED_RED1 */
#define LED_RED1_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Turn on target LED_RED1 */
#define LED_RED1_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Turn off target LED_RED1 */
#define LED_RED1_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Toggle on target LED_RED1 */

#define LED_GREEN1_INIT(output)                                    \
    FGPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, output);\
    BOARD_LED_GREEN_GPIO->PDDR |= (1U << BOARD_LED_GREEN_GPIO_PIN)  /*!< Enable target LED_GREEN1 */
#define LED_GREEN1_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn on target LED_GREEN1 */
#define LED_GREEN1_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn off target LED_GREEN1 */
#define LED_GREEN1_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Toggle on target LED_GREEN1 */

#define LED_BLUE_INIT(output)                                  \
    FGPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, output);\
    BOARD_LED_BLUE_GPIO->PDDR |= (1U << BOARD_LED_BLUE_GPIO_PIN)  /*!< Enable target LED_BLUE */
#define LED_BLUE_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Turn on target LED_BLUE */
#define LED_BLUE_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Turn off target LED_BLUE */
#define LED_BLUE_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Toggle on target LED_BLUE */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

void BOARD_InitDebugConsole(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
