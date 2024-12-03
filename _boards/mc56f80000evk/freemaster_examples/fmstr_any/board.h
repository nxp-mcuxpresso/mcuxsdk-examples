/*
 * Copyright 2020-2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "fsl_gpio.h"
#include "fsl_qsci.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_DEBUG_UART_TYPE kSerialPort_Uart

#define BOARD_DEBUG_UART_INSTANCE   0U
#define BOARD_DEBUG_UART_CLOCK_NAME kCLOCK_QSCI0

#define BOARD_DEBUG_UART_RX_IRQ       QSCI0_RCV_IRQn
#define BOARD_DEBUG_UART_RX_ERR_IRQ   QSCI0_RERR_IRQn
#define BOARD_DEBUG_UART_TX_IRQ       QSCI0_TDRE_IRQn
#define BOARD_DEBUG_UART_TRX_IDLE_IRQ QSCI0_TRIDLE_IRQn

#define BOARD_DEBUG_UART_ISR_PRORITY 1U

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/* LED Macro Definition */
#ifndef BOARD_LED_RED_GPIO
#define BOARD_LED_RED_GPIO GPIOF
#endif
#ifndef BOARD_LED_RED_GPIO_PIN
#define BOARD_LED_RED_GPIO_PIN kGPIO_Pin6
#endif

#ifndef BOARD_LED_BLUE_GPIO
#define BOARD_LED_BLUE_GPIO GPIOF
#endif
#ifndef BOARD_LED_BLUE_GPIO_PIN
#define BOARD_LED_BLUE_GPIO_PIN kGPIO_Pin4
#endif

#ifndef BOARD_LED_YELLOW_GPIO
#define BOARD_LED_YELLOW_GPIO GPIOF
#endif
#ifndef BOARD_LED_YELLOW_GPIO_PIN
#define BOARD_LED_YELLOW_GPIO_PIN kGPIO_Pin5
#endif

/* Button macro definition */
#ifndef BOARD_BUTTON_SW2_GPIO
#define BOARD_BUTTON_SW2_GPIO GPIOE
#endif
#ifndef BOARD_BUTTON_SW2_GPIO_PIN
#define BOARD_BUTTON_SW2_GPIO_PIN kGPIO_Pin6
#endif

#ifndef BOARD_BUTTON_SW3_GPIO
#define BOARD_BUTTON_SW3_GPIO GPIOE
#endif
#ifndef BOARD_BUTTON_SW3_GPIO_PIN
#define BOARD_BUTTON_SW3_GPIO_PIN kGPIO_Pin7
#endif

#ifndef BOARD_BUTTON_SW4_GPIO
#define BOARD_BUTTON_SW4_GPIO GPIOF
#endif
#ifndef BOARD_BUTTON_SW4_GPIO_PIN
#define BOARD_BUTTON_SW4_GPIO_PIN kGPIO_Pin8
#endif

#ifndef BOARD_BUTTON_SW5_GPIO
#define BOARD_BUTTON_SW5_GPIO GPIOC
#endif
#ifndef BOARD_BUTTON_SW5_GPIO_PIN
#define BOARD_BUTTON_SW5_GPIO_PIN kGPIO_Pin13
#endif

/* Board led color mapping */
#define LOGIC_LED_ON  kGPIO_OutputLow
#define LOGIC_LED_OFF kGPIO_OutputHigh

#define LED_RED_INIT(output) BOARD_LedInit(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, (output))
#define LED_RED_ON()         GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, LOGIC_LED_ON)
#define LED_RED_OFF()        GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, LOGIC_LED_OFF)
#define LED_RED_TOGGLE()     GPIO_PinToggle(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN)

#define LED_BLUE_INIT(output) BOARD_LedInit(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, (output))
#define LED_BLUE_ON()         GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, LOGIC_LED_ON)
#define LED_BLUE_OFF()        GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, LOGIC_LED_OFF)
#define LED_BLUE_TOGGLE()     GPIO_PinToggle(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN)

#define LED_YELLOW_INIT(output) BOARD_LedInit(BOARD_LED_YELLOW_GPIO, BOARD_LED_YELLOW_GPIO_PIN, (output))
#define LED_YELLOW_ON()         GPIO_PinWrite(BOARD_LED_YELLOW_GPIO, BOARD_LED_YELLOW_GPIO_PIN, LOGIC_LED_ON)
#define LED_YELLOW_OFF()        GPIO_PinWrite(BOARD_LED_YELLOW_GPIO, BOARD_LED_YELLOW_GPIO_PIN, LOGIC_LED_OFF)
#define LED_YELLOW_TOGGLE()     GPIO_PinToggle(BOARD_LED_YELLOW_GPIO, BOARD_LED_YELLOW_GPIO_PIN)

static inline void BOARD_LedInit(GPIO_Type *base, gpio_pin_t ePin, gpio_output_level_t eOutputLevel)
{
    gpio_config_t sLedConfig = {
        .eDirection     = kGPIO_DigitalOutput,
        .eMode          = kGPIO_ModeGpio,
        .eOutMode       = kGPIO_OutputPushPull,
        .eDriveStrength = kGPIO_DriveStrengthHigh,
        .eOutLevel      = eOutputLevel,
        .eSlewRate      = kGPIO_SlewRateFast,
        .ePull          = kGPIO_PullDisable,
        .eInterruptMode = kGPIO_InterruptDisable,
    };
    GPIO_PinInit(base, ePin, &sLedConfig);
}

/* The board name */
#define BOARD_NAME "MC56F80000-EVK"

void BOARD_InitHardware(void);
void BOARD_InitDebugConsole(void);

#endif /* _BOARD_H_ */
