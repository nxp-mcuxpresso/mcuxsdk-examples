/*
 * Copyright 2020 NXP
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

#define BOARD_DEBUG_UART_INSTANCE   2U
#define BOARD_DEBUG_UART_CLOCK_NAME kCLOCK_QSCI2

#define BOARD_DEBUG_UART_RX_IRQ      QSCI2_RCV_IRQn
#define BOARD_DEBUG_UART_RX_ERR_IRQ  QSCI2_RERR_IRQn
#define BOARD_DEBUG_UART_TX_IRQ      QSCI2_TDRE_IRQn
#define BOARD_DEBUG_UART_TX_IDLE_IRQ QSCI2_TRIDLE_IRQn

#define BOARD_DEBUG_UART_ISR_PRORITY 1U

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/* LED Macro Definition */
#ifndef BOARD_LED_RED_GPIO
#define BOARD_LED_RED_GPIO GPIOF
#endif
#ifndef BOARD_LED_RED_GPIO_PIN
#define BOARD_LED_RED_GPIO_PIN kGPIO_Pin8
#endif

#ifndef BOARD_LED_GREEN_GPIO
#define BOARD_LED_GREEN_GPIO GPIOF
#endif
#ifndef BOARD_LED_GREEN_GPIO_PIN
#define BOARD_LED_GREEN_GPIO_PIN kGPIO_Pin9
#endif

#ifndef BOARD_LED_BLUE_GPIO
#define BOARD_LED_BLUE_GPIO GPIOF
#endif
#ifndef BOARD_LED_BLUE_GPIO_PIN
#define BOARD_LED_BLUE_GPIO_PIN kGPIO_Pin10
#endif

#ifndef BOARD_LED_YELLOW_GPIO
#define BOARD_LED_YELLOW_GPIO GPIOF
#endif
#ifndef BOARD_LED_YELLOW_GPIO_PIN
#define BOARD_LED_YELLOW_GPIO_PIN kGPIO_Pin11
#endif

/* Button macro definition */
#ifndef BOARD_BUTTON_SW2_GPIO
#define BOARD_BUTTON_SW2_GPIO GPIOF
#endif
#ifndef BOARD_BUTTON_SW2_GPIO_PIN
#define BOARD_BUTTON_SW2_GPIO_PIN kGPIO_Pin15
#endif

#ifndef BOARD_BUTTON_SW3_GPIO
#define BOARD_BUTTON_SW3_GPIO GPIOF
#endif
#ifndef BOARD_BUTTON_SW3_GPIO_PIN
#define BOARD_BUTTON_SW3_GPIO_PIN kGPIO_Pin3
#endif

#ifndef BOARD_BUTTON_SW4_GPIO
#define BOARD_BUTTON_SW4_GPIO GPIOF
#endif
#ifndef BOARD_BUTTON_SW4_GPIO_PIN
#define BOARD_BUTTON_SW4_GPIO_PIN kGPIO_Pin6
#endif

#ifndef BOARD_BUTTON_SW5_GPIO
#define BOARD_BUTTON_SW5_GPIO GPIOC
#endif
#ifndef BOARD_BUTTON_SW5_GPIO_PIN
#define BOARD_BUTTON_SW5_GPIO_PIN kGPIO_Pin4
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

#define LED_GREEN_INIT(output) BOARD_LedInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, (output))
#define LED_GREEN_ON()         GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, LOGIC_LED_ON)
#define LED_GREEN_OFF()        GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, LOGIC_LED_OFF)
#define LED_GREEN_TOGGLE()     GPIO_PinToggle(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN)

#define LED_YELLOW_INIT(output) BOARD_LedInit(BOARD_LED_YELLOW_GPIO, BOARD_LED_YELLOW_GPIO_PIN, (output))
#define LED_YELLOW_ON()         GPIO_PinWrite(BOARD_LED_YELLOW_GPIO, BOARD_LED_YELLOW_GPIO_PIN, LOGIC_LED_ON)
#define LED_YELLOW_OFF()        GPIO_PinWrite(BOARD_LED_YELLOW_GPIO, BOARD_LED_YELLOW_GPIO_PIN, LOGIC_LED_OFF)
#define LED_YELLOW_TOGGLE()     GPIO_PinToggle(BOARD_LED_YELLOW_GPIO, BOARD_LED_YELLOW_GPIO_PIN)

#define ACCEL_I2C_BASE          I2C1
#define ACCEL_I2C_DEVICE_ADDR   0x1EU
#define ACCEL_SENSOR_DESIGNATOR "U11"
#define ACCEL_SCL_PORT          GPIOF
#define ACCEL_SCL_PIN           kGPIO_Pin2
#define ACCEL_SDA_PORT          GPIOF
#define ACCEL_SDA_PIN           kGPIO_Pin3

/* The board name */
#define BOARD_NAME "MC56F83000-EVK"

void BOARD_InitHardware(void);
void BOARD_InitDebugConsole(void);

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

#endif /* _BOARD_H_ */
