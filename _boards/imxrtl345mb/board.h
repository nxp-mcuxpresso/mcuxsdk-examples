/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#if (IMXRTL3459AAA_cm55_core0_SERIES)
#include "clock_config.h"
#include "pin_mux.h"
#include "fsl_swm.h"
#include "fsl_gpio.h"
#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME "IMXRTL345MB"
/*! @brief The manufacturer name */
#define MANUFACTURER_NAME "NXP"

/*! @brief The UART to use for debug messages. */
#define BOARD_DEBUG_UART_TYPE       kSerialPort_Uart
#define BOARD_DEBUG_UART_BASEADDR   (uint32_t) CTRLSS__USART_3
#define BOARD_DEBUG_UART_INSTANCE   3U
/* NOTE: All flexcomm clocks are pre-divided by 2 by a flip flop before reaching the flexcomm divisor.
 * E.g., a divisor of 2 will result in 5MHz input (10MHz / 2 = 5MHz) */
#define BOARD_DEBUG_UART_CLK_FREQ   10000000U
#define BOARD_DEBUG_UART_CLK_ATTACH kFROL_to_FLEXCOMM3
#define BOARD_DEBUG_UART_PERIPH_CLK kCLOCK_Flexcomm3Periph
#define BOARD_DEBUG_UART_CLKSRC     kCLOCK_Flexcomm3
#define BOARD_UART_IRQ_HANDLER      FLEXCOMM3_IRQHandler
#define BOARD_UART_IRQ              FLEXCOMM3_IRQn
#define BOARD_DEBUG_UART_BAUDRATE   115200

/*! @brief The SWO to use for debug messages. */
#define BOARD_DEBUG_CONSOLE_SWO_PORT     (0U)
#define BOARD_DEBUG_CONSOLE_SWO_BAUDRATE (2000000U)

#ifndef BOARD_LED_RED_GPIO
#define BOARD_LED_RED_GPIO CTRLSS__GPIO
#endif
#define BOARD_LED_RED_GPIO_PORT SWM_TO_GPIO_PORT(APP_LED_RED_SWM_PIN) 
#ifndef BOARD_LED_RED_GPIO_PIN
#define BOARD_LED_RED_GPIO_PIN SWM_TO_GPIO_PIN(APP_LED_RED_SWM_PIN)
#endif

#ifndef BOARD_LED_BLUE_GPIO
#define BOARD_LED_BLUE_GPIO CTRLSS__GPIO
#endif
#define BOARD_LED_BLUE_GPIO_PORT SWM_TO_GPIO_PORT(APP_LED_BLUE_SWM_PIN)
#ifndef BOARD_LED_BLUE_GPIO_PIN
#define BOARD_LED_BLUE_GPIO_PIN SWM_TO_GPIO_PIN(APP_LED_BLUE_SWM_PIN)
#endif

#ifndef BOARD_LED_GREEN_GPIO
#define BOARD_LED_GREEN_GPIO CTRLSS__GPIO
#endif
#define BOARD_LED_GREEN_GPIO_PORT SWM_TO_GPIO_PORT(APP_LED_GREEN_SWM_PIN)
#ifndef BOARD_LED_GREEN_GPIO_PIN
#define BOARD_LED_GREEN_GPIO_PIN SWM_TO_GPIO_PIN(APP_LED_GREEN_SWM_PIN)
#endif

#ifndef BOARD_SW1_GPIO
#define BOARD_SW1_GPIO CTRLSS__GPIO
#endif
#define BOARD_SW1_GPIO_PORT SWM_TO_GPIO_PORT(APP_SW1_SWM_PIN)
#ifndef BOARD_SW1_GPIO_PIN
#define BOARD_SW1_GPIO_PIN SWM_TO_GPIO_PIN(APP_SW1_SWM_PIN)
#endif
#define BOARD_SW1_NAME        "SW1"
#define BOARD_SW1_IRQ         GPIO_INT0_IRQn
#define BOARD_SW1_IRQ_HANDLER GPIO_INT0_IRQHandler

/*#ifndef BOARD_SW3_GPIO
#define BOARD_SW3_GPIO CTRLSS__GPIO
#endif
#define BOARD_SW3_GPIO_PORT 0U
#ifndef BOARD_SW3_GPIO_PIN
#define BOARD_SW3_GPIO_PIN 17U
#endif
#define BOARD_SW3_NAME              "SW3"
#define BOARD_SW3_IRQ               GPIO_INT0_IRQn
#define BOARD_SW3_IRQ_HANDLER       GPIO_INT0_IRQHandler
#define BOARD_SW3_GPIO_PININT_INDEX 1 */

/* Board led color mapping */
#define LOGIC_LED_ON  1U
#define LOGIC_LED_OFF 0U

/*#define BOARD_HAS_NO_CTIMER_OUTPUT_PIN_CONNECTED_TO_LED (1)*/

#define LED_RED_INIT(output)                                                                          \
    {                                                                                                 \
        GPIO_PinInit(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PORT, BOARD_LED_RED_GPIO_PIN,             \
                     &(gpio_pin_config_t){kGPIO_DigitalOutput, (output)}); /*!< Enable target LED1 */ \
    }
#define LED_RED_OFF()                                           \
    GPIO_PortClear(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PORT, \
                   1U << BOARD_LED_RED_GPIO_PIN) /*!< Turn off target LED1 */
#define LED_RED_ON()                                                                        \
    GPIO_PortSet(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PORT,                               \
                 1U << BOARD_LED_RED_GPIO_PIN) /*!< Turn on target LED1 \ \ \ \ \ \ \ \ \ \ \
                                                */
#define LED_RED_TOGGLE()                                         \
    GPIO_PortToggle(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PORT, \
                    1U << BOARD_LED_RED_GPIO_PIN) /*!< Toggle on target LED1 */

#define LED_BLUE_INIT(output)                                                                         \
    {                                                                                                 \
        GPIO_PinInit(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PORT, BOARD_LED_BLUE_GPIO_PIN,          \
                     &(gpio_pin_config_t){kGPIO_DigitalOutput, (output)}); /*!< Enable target LED1 */ \
    }
#define LED_BLUE_OFF()                                            \
    GPIO_PortClear(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PORT, \
                   1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Turn off target LED1 */
#define LED_BLUE_ON()                                           \
    GPIO_PortSet(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PORT, \
                 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Turn on target LED1 */
#define LED_BLUE_TOGGLE()                                          \
    GPIO_PortToggle(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PORT, \
                    1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Toggle on target LED1 */

#define LED_GREEN_INIT(output)                                                              \
    GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PORT, BOARD_LED_GREEN_GPIO_PIN, \
                 &(gpio_pin_config_t){kGPIO_DigitalOutput, (output)}) /*!< Enable target LED1 */
#define LED_GREEN_OFF()                                             \
    GPIO_PortClear(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PORT, \
                   1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn off target LED1 */
#define LED_GREEN_ON()                                            \
    GPIO_PortSet(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PORT, \
                 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn on target LED1 */
#define LED_GREEN_TOGGLE()                                           \
    GPIO_PortToggle(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PORT, \
                    1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Toggle on target LED1 */
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/
void BOARD_InitDebugConsole(void);
void BOARD_InitDebugConsoleSWO(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */


#endif/* IMXRTL3459AAA_cm55_core0_SERIES */

#endif /* _BOARD_H_ */
