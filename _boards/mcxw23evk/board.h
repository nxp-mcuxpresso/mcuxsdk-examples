/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "fsl_common.h"
#include "fsl_iocon.h"
#include "fsl_gpio.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief The board name */
#define BOARD_NAME "MCXW23-EVK"
/*! @brief The manufacturer name */
#define MANUFACTURER_NAME "NXP"

/*! @brief The UART to use for debug messages. */

#define BOARD_DEBUG_UART_TYPE     kSerialPort_Uart
#define BOARD_DEBUG_UART_CLK_FREQ 12000000U

#ifdef USE_UART2_FOR_DEBUG
#define BOARD_DEBUG_UART_BASEADDR   (uint32_t) USART2
#define BOARD_DEBUG_UART_INSTANCE   2U
#define BOARD_DEBUG_UART_CLK_ATTACH kFRO12M_to_FLEXCOMM2
#define BOARD_DEBUG_UART_RST        kFC2_RST_SHIFT_RSTn
#define BOARD_DEBUG_UART_CLKSRC     kCLOCK_Flexcomm2
#define BOARD_UART_IRQ_HANDLER      FLEXCOMM2_IRQHandler
#define BOARD_UART_IRQ              FLEXCOMM2_IRQn
#else
#define BOARD_DEBUG_UART_BASEADDR   (uint32_t) USART0
#define BOARD_DEBUG_UART_INSTANCE   0U
#define BOARD_DEBUG_UART_CLK_ATTACH kFRO12M_to_FLEXCOMM0
#define BOARD_DEBUG_UART_RST        kFC0_RST_SHIFT_RSTn
#define BOARD_DEBUG_UART_CLKSRC     kCLOCK_Flexcomm0
#define BOARD_UART_IRQ_HANDLER      FLEXCOMM0_IRQHandler
#define BOARD_UART_IRQ              FLEXCOMM0_IRQn
#endif

#define BOARD_ACCEL_I2C_BASEADDR   I2C4
#define BOARD_ACCEL_I2C_CLOCK_FREQ 12000000

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200U
#endif

/* Board led color mapping */
#define LOGIC_LED_ON  1U
#define LOGIC_LED_OFF 0U

/* User led */
#ifndef BOARD_LED_RED_GPIO
#define BOARD_LED_RED_GPIO GPIO
#endif
#define BOARD_LED_RED_GPIO_PORT 0U
#ifndef BOARD_LED_RED_GPIO_PIN
#define BOARD_LED_RED_GPIO_PIN 19U
#endif

#ifndef BOARD_LED_GPIO
#define BOARD_LED_GPIO BOARD_LED_RED_GPIO
#endif
#define BOARD_LED_GPIO_PORT BOARD_LED_RED_GPIO_PORT
#ifndef BOARD_LED_GPIO_PIN
#define BOARD_LED_GPIO_PIN BOARD_LED_RED_GPIO_PIN
#endif

#ifndef BOARD_BUTTON1_GPIO
#define BOARD_BUTTON1_GPIO GPIO
#endif
#define BOARD_BUTTON1_GPIO_PORT 0U
#ifndef BOARD_BUTTON1_GPIO_PIN
#define BOARD_BUTTON1_GPIO_PIN 18U
#endif

#ifndef BOARD_WAKEUP_GPIO
#define BOARD_WAKEUP_GPIO GPIO
#endif
#define BOARD_WAKEUP_GPIO_PORT 0U
#ifndef BOARD_WAKEUP_GPIO_PIN
#define BOARD_WAKEUP_GPIO_PIN 21U
#endif

#define LED_RED_INIT(output)                                                                        \
    {                                                                                                \
        IOCON_PinMuxSet(IOCON, BOARD_LED_RED_GPIO_PORT, BOARD_LED_RED_GPIO_PIN, IOCON_DIGITAL_EN); \
        GPIO_PinInit(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PORT, BOARD_LED_RED_GPIO_PIN,         \
                     &(gpio_pin_config_t){kGPIO_DigitalOutput, (output)});                           \
    }
#define LED_RED_OFF()    GPIO_PortClear(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PORT, 1U << BOARD_LED_RED_GPIO_PIN)
#define LED_RED_ON()     GPIO_PortSet(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PORT, 1U << BOARD_LED_RED_GPIO_PIN)
#define LED_RED_TOGGLE() GPIO_PortToggle(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PORT, 1U << BOARD_LED_RED_GPIO_PIN)

#define LED_INIT   LED_RED_INIT
#define LED_OFF    LED_RED_OFF
#define LED_ON     LED_RED_ON
#define LED_TOGGLE LED_RED_TOGGLE

/** I2C master base */
#define BOARD_I2C_MASTER ((I2C_Type *)I2C1_BASE)
/** I2C master clock frequency */
#define BOARD_I2C_MASTER_CLOCK_FREQUENCY (12000000)
/** I2C clock frequency */
#define BOARD_I2C_BAUDRATE 100000U

/** Accelerometer I2C address */
#define FXLS8964AF_I2C_ADDR 0x19
/** Accelerometer config 4 reg address */
#define FXLS8964AF_CONFIG4_REG_ADDR 0x18
/** Accelerometer config 4 pp_od position */
#define FXLS8964AF_CONFIG4_PP_OD_POS 1
/** Accelerometer config 4 pp_od open drain */
#define FXLS8964AF_CONFIG4_PP_OD_OPEN_DRAIN 1

/*******************************************************************************
 * API
 ******************************************************************************/
void BOARD_InitBootPins(void);
void BOARD_InitDebugConsole(void);
void BOARD_InitI2c(void);
status_t BOARD_I2cWriteFunc(uint8_t deviceAddress, uint32_t subAddress, uint8_t *txBuff, size_t txBuffSize);
status_t BOARD_I2cReadFunc(uint8_t deviceAddress, uint32_t subAddress, uint8_t *rxBuff, size_t rxBuffSize);

#endif /* _BOARD_H_ */
