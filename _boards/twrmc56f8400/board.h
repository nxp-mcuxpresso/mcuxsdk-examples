/*
 * Copyright 2020-2021 NXP
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

#define BOARD_DEBUG_UART_INSTANCE   1U
#define BOARD_DEBUG_UART_CLOCK_NAME kCLOCK_QSCI1

#define BOARD_DEBUG_UART_RX_IRQ      QSCI1_RCV_IRQn
#define BOARD_DEBUG_UART_RX_ERR_IRQ  QSCI1_RERR_IRQn
#define BOARD_DEBUG_UART_TX_IRQ      QSCI1_TDRE_IRQn
#define BOARD_DEBUG_UART_TX_IDLE_IRQ QSCI1_TIDLE_IRQn

#define BOARD_DEBUG_UART_ISR_PRORITY 1U

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/* LED Macro Definition */
#ifndef BOARD_LED_0_GPIO
#define BOARD_LED_0_GPIO GPIOE
#endif
#ifndef BOARD_LED_0_GPIO_PIN
#define BOARD_LED_0_GPIO_PIN kGPIO_Pin0
#endif

#ifndef BOARD_LED_1_GPIO
#define BOARD_LED_1_GPIO GPIOE
#endif
#ifndef BOARD_LED_1_GPIO_PIN
#define BOARD_LED_1_GPIO_PIN kGPIO_Pin1
#endif

#ifndef BOARD_LED_2_GPIO
#define BOARD_LED_2_GPIO GPIOE
#endif
#ifndef BOARD_LED_2_GPIO_PIN
#define BOARD_LED_2_GPIO_PIN kGPIO_Pin2
#endif

#ifndef BOARD_LED_3_GPIO
#define BOARD_LED_3_GPIO GPIOE
#endif
#ifndef BOARD_LED_3_GPIO_PIN
#define BOARD_LED_3_GPIO_PIN kGPIO_Pin3
#endif

#ifndef BOARD_LED_4_GPIO
#define BOARD_LED_4_GPIO GPIOE
#endif
#ifndef BOARD_LED_4_GPIO_PIN
#define BOARD_LED_4_GPIO_PIN kGPIO_Pin4
#endif

#ifndef BOARD_LED_5_GPIO
#define BOARD_LED_5_GPIO GPIOE
#endif
#ifndef BOARD_LED_5_GPIO_PIN
#define BOARD_LED_5_GPIO_PIN kGPIO_Pin5
#endif

#ifndef BOARD_LED_6_GPIO
#define BOARD_LED_6_GPIO GPIOE
#endif
#ifndef BOARD_LED_6_GPIO_PIN
#define BOARD_LED_6_GPIO_PIN kGPIO_Pin6
#endif

#ifndef BOARD_LED_7_GPIO
#define BOARD_LED_7_GPIO GPIOE
#endif
#ifndef BOARD_LED_7_GPIO_PIN
#define BOARD_LED_7_GPIO_PIN kGPIO_Pin7
#endif

#ifndef BOARD_LED_8_GPIO
#define BOARD_LED_8_GPIO GPIOF
#endif
#ifndef BOARD_LED_8_GPIO_PIN
#define BOARD_LED_8_GPIO_PIN kGPIO_Pin6
#endif

/* Button macro definition */
#ifndef BOARD_BUTTON_SW1_GPIO
#define BOARD_BUTTON_SW1_GPIO GPIOC
#endif
#ifndef BOARD_BUTTON_SW1_GPIO_PIN
#define BOARD_BUTTON_SW1_GPIO_PIN kGPIO_Pin2
#endif

#ifndef BOARD_BUTTON_SW2_GPIO
#define BOARD_BUTTON_SW2_GPIO GPIOF
#endif
#ifndef BOARD_BUTTON_SW2_GPIO_PIN
#define BOARD_BUTTON_SW2_GPIO_PIN kGPIO_Pin8
#endif

/* Board led color mapping */
#define LOGIC_LED_ON  kGPIO_OutputHigh
#define LOGIC_LED_OFF kGPIO_OutputLow

#define LED_0_INIT(output) BOARD_LedInit(BOARD_LED_0_GPIO, BOARD_LED_0_GPIO_PIN, (output))
#define LED_0_ON()         GPIO_PinWrite(BOARD_LED_0_GPIO, BOARD_LED_0_GPIO_PIN, LOGIC_LED_ON)
#define LED_0_OFF()        GPIO_PinWrite(BOARD_LED_0_GPIO, BOARD_LED_0_GPIO_PIN, LOGIC_LED_OFF)
#define LED_0_TOGGLE()     GPIO_PinToggle(BOARD_LED_0_GPIO, BOARD_LED_0_GPIO_PIN)

#define LED_1_INIT(output) BOARD_LedInit(BOARD_LED_1_GPIO, BOARD_LED_1_GPIO_PIN, (output))
#define LED_1_ON()         GPIO_PinWrite(BOARD_LED_1_GPIO, BOARD_LED_1_GPIO_PIN, LOGIC_LED_ON)
#define LED_1_OFF()        GPIO_PinWrite(BOARD_LED_1_GPIO, BOARD_LED_1_GPIO_PIN, LOGIC_LED_OFF)
#define LED_1_TOGGLE()     GPIO_PinToggle(BOARD_LED_1_GPIO, BOARD_LED_1_GPIO_PIN)

#define LED_2_INIT(output) BOARD_LedInit(BOARD_LED_2_GPIO, BOARD_LED_2_GPIO_PIN, (output))
#define LED_2_ON()         GPIO_PinWrite(BOARD_LED_2_GPIO, BOARD_LED_2_GPIO_PIN, LOGIC_LED_ON)
#define LED_2_OFF()        GPIO_PinWrite(BOARD_LED_2_GPIO, BOARD_LED_2_GPIO_PIN, LOGIC_LED_OFF)
#define LED_2_TOGGLE()     GPIO_PinToggle(BOARD_LED_2_GPIO, BOARD_LED_2_GPIO_PIN)

#define LED_3_INIT(output) BOARD_LedInit(BOARD_LED_3_GPIO, BOARD_LED_3_GPIO_PIN, (output))
#define LED_3_ON()         GPIO_PinWrite(BOARD_LED_3_GPIO, BOARD_LED_3_GPIO_PIN, LOGIC_LED_ON)
#define LED_3_OFF()        GPIO_PinWrite(BOARD_LED_3_GPIO, BOARD_LED_3_GPIO_PIN, LOGIC_LED_OFF)
#define LED_3_TOGGLE()     GPIO_PinToggle(BOARD_LED_3_GPIO, BOARD_LED_3_GPIO_PIN)

#define LED_4_INIT(output) BOARD_LedInit(BOARD_LED_4_GPIO, BOARD_LED_4_GPIO_PIN, (output))
#define LED_4_ON()         GPIO_PinWrite(BOARD_LED_4_GPIO, BOARD_LED_4_GPIO_PIN, LOGIC_LED_ON)
#define LED_4_OFF()        GPIO_PinWrite(BOARD_LED_4_GPIO, BOARD_LED_4_GPIO_PIN, LOGIC_LED_OFF)
#define LED_4_TOGGLE()     GPIO_PinToggle(BOARD_LED_4_GPIO, BOARD_LED_4_GPIO_PIN)

#define LED_5_INIT(output) BOARD_LedInit(BOARD_LED_5_GPIO, BOARD_LED_5_GPIO_PIN, (output))
#define LED_5_ON()         GPIO_PinWrite(BOARD_LED_5_GPIO, BOARD_LED_5_GPIO_PIN, LOGIC_LED_ON)
#define LED_5_OFF()        GPIO_PinWrite(BOARD_LED_5_GPIO, BOARD_LED_5_GPIO_PIN, LOGIC_LED_OFF)
#define LED_5_TOGGLE()     GPIO_PinToggle(BOARD_LED_5_GPIO, BOARD_LED_5_GPIO_PIN)

#define LED_6_INIT(output) BOARD_LedInit(BOARD_LED_6_GPIO, BOARD_LED_6_GPIO_PIN, (output))
#define LED_6_ON()         GPIO_PinWrite(BOARD_LED_6_GPIO, BOARD_LED_6_GPIO_PIN, LOGIC_LED_ON)
#define LED_6_OFF()        GPIO_PinWrite(BOARD_LED_6_GPIO, BOARD_LED_6_GPIO_PIN, LOGIC_LED_OFF)
#define LED_6_TOGGLE()     GPIO_PinToggle(BOARD_LED_6_GPIO, BOARD_LED_6_GPIO_PIN)

#define LED_7_INIT(output) BOARD_LedInit(BOARD_LED_7_GPIO, BOARD_LED_7_GPIO_PIN, (output))
#define LED_7_ON()         GPIO_PinWrite(BOARD_LED_7_GPIO, BOARD_LED_7_GPIO_PIN, LOGIC_LED_ON)
#define LED_7_OFF()        GPIO_PinWrite(BOARD_LED_7_GPIO, BOARD_LED_7_GPIO_PIN, LOGIC_LED_OFF)
#define LED_7_TOGGLE()     GPIO_PinToggle(BOARD_LED_7_GPIO, BOARD_LED_7_GPIO_PIN)

#define LED_8_INIT(output) BOARD_LedInit(BOARD_LED_8_GPIO, BOARD_LED_8_GPIO_PIN, (output))
#define LED_8_ON()         GPIO_PinWrite(BOARD_LED_8_GPIO, BOARD_LED_8_GPIO_PIN, LOGIC_LED_ON)
#define LED_8_OFF()        GPIO_PinWrite(BOARD_LED_8_GPIO, BOARD_LED_8_GPIO_PIN, LOGIC_LED_OFF)
#define LED_8_TOGGLE()     GPIO_PinToggle(BOARD_LED_8_GPIO, BOARD_LED_8_GPIO_PIN)

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
#define BOARD_NAME "TWR-MC56F8400"

void BOARD_InitHardware(void);
void BOARD_InitDebugConsole(void);

#endif /* _BOARD_H_ */
