/*
 * Copyright 2024 NXP
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
 #define BOARD_NAME "FRDM-MCXW23"
 /*! @brief The manufacturer name */
 #define MANUFACTURER_NAME "NXP"
 
 /*! @brief The UART to use for debug messages. */
 
 /* FLEXCOMM0 is standard debug UART port because it is routed to the USB port on the board */
 #define BOARD_DEBUG_UART_TYPE       kSerialPort_Uart
 #define BOARD_DEBUG_UART_BASEADDR   (uint32_t) USART0
 #define BOARD_DEBUG_UART_INSTANCE   0U
 #define BOARD_DEBUG_UART_CLK_ATTACH kFRO12M_to_FLEXCOMM0
 #define BOARD_DEBUG_UART_RST        kFC0_RST_SHIFT_RSTn
 #define BOARD_DEBUG_UART_CLKSRC     kCLOCK_Flexcomm0
 #define BOARD_DEBUG_UART_CLK_FREQ   12000000U
 #ifndef BOARD_DEBUG_UART_BAUDRATE
 #define BOARD_DEBUG_UART_BAUDRATE 115200U
 #endif
 
 #define BOARD_DEBUG2_UART_TYPE       kSerialPort_Uart
 #define BOARD_DEBUG2_UART_BASEADDR   (uint32_t) USART2
 #define BOARD_DEBUG2_UART_INSTANCE   2U
 #define BOARD_DEBUG2_UART_CLK_ATTACH kFRO12M_to_FLEXCOMM2
 #define BOARD_DEBUG2_UART_RST        kFC2_RST_SHIFT_RSTn
 #define BOARD_DEBUG2_UART_CLKSRC     kCLOCK_Flexcomm2
 #define BOARD_DEBUG2_UART_CLK_FREQ   12000000U
 #ifndef BOARD_DEBUG2_UART_BAUDRATE
 #define BOARD_DEBUG2_UART_BAUDRATE 115200U
 #endif
 
 #define BOARD_ACCEL_I2C_BASEADDR   I2C4
 #define BOARD_ACCEL_I2C_CLOCK_FREQ 12000000
 
 /* Board led color mapping */
 #define LOGIC_LED_ON  0U
 #define LOGIC_LED_OFF 1U
 
 /* User led */
 #ifndef BOARD_LED_BLUE_GPIO
 #define BOARD_LED_BLUE_GPIO GPIO
 #endif
 #define BOARD_LED_BLUE_GPIO_PORT 0U
 #ifndef BOARD_LED_BLUE_GPIO_PIN
 #define BOARD_LED_BLUE_GPIO_PIN 19U
 #endif
 #ifndef BOARD_LED_BLUE_DEFAULT_STATE
 #define BOARD_LED_BLUE_DEFAULT_STATE 0U
 #endif
 /* RGB R */
 #ifndef BOARD_RGB_LED_RED_GPIO
 #define BOARD_RGB_LED_RED_GPIO GPIO
 #endif
 #define BOARD_RGB_LED_RED_GPIO_PORT 0U
 #ifndef BOARD_RGB_LED_RED_GPIO_PIN
 #define BOARD_RGB_LED_RED_GPIO_PIN 1U
 #endif
 #ifndef BOARD_RGB_LED_RED_DEFAULT_STATE
 #define BOARD_RGB_LED_RED_DEFAULT_STATE 0U
 #endif
 /* RGB G */
 #ifndef BOARD_RGB_LED_GREEN_GPIO
 #define BOARD_RGB_LED_GREEN_GPIO GPIO
 #endif
 #define BOARD_RGB_LED_GREEN_GPIO_PORT 0U
 #ifndef BOARD_RGB_LED_GREEN_GPIO_PIN
 #define BOARD_RGB_LED_GREEN_GPIO_PIN 0U
 #endif
 #ifndef BOARD_RGB_LED_GREEN_DEFAULT_STATE
 #define BOARD_RGB_LED_GREEN_DEFAULT_STATE 0U
 #endif
 /* RGB B */
 #ifndef BOARD_RGB_LED_BLUE_GPIO
 #define BOARD_RGB_LED_BLUE_GPIO GPIO
 #endif
 #define BOARD_RGB_LED_BLUE_GPIO_PORT 0U
 #ifndef BOARD_RGB_LED_BLUE_GPIO_PIN
 #define BOARD_RGB_LED_BLUE_GPIO_PIN 4U
 #endif
 #ifndef BOARD_RGB_LED_BLUE_DEFAULT_STATE
 #define BOARD_RGB_LED_BLUE_DEFAULT_STATE 0U
 #endif
 
 #ifndef BOARD_LED_GPIO
 #define BOARD_LED_GPIO BOARD_LED_BLUE_GPIO
 #endif
 #define BOARD_LED_GPIO_PORT BOARD_LED_BLUE_GPIO_PORT
 #ifndef BOARD_LED_GPIO_PIN
 #define BOARD_LED_GPIO_PIN BOARD_LED_BLUE_GPIO_PIN
 #endif
 
 #ifndef BOARD_BUTTON1_GPIO
 #define BOARD_BUTTON1_GPIO GPIO
 #endif
 #define BOARD_BUTTON1_GPIO_PORT 0U
 #ifndef BOARD_BUTTON1_GPIO_PIN
 #define BOARD_BUTTON1_GPIO_PIN 18U
 #endif
 #ifndef BOARD_BUTTON1_DEFAULT_STATE
 #define BOARD_BUTTON1_DEFAULT_STATE 1U
 #endif
 
 #ifndef BOARD_WAKEUP_GPIO
 #define BOARD_WAKEUP_GPIO GPIO
 #endif
 #define BOARD_WAKEUP_GPIO_PORT 0U
 #ifndef BOARD_WAKEUP_GPIO_PIN
 #define BOARD_WAKEUP_GPIO_PIN 21U
 #endif
 #ifndef BOARD_WAKEUP_GPIO_DEFAULT_STATE
 #define BOARD_WAKEUP_GPIO_DEFAULT_STATE 1U
 #endif
 
 #define LED_BLUE_INIT(output)                                                                        \
     {                                                                                                \
         IOCON_PinMuxSet(IOCON, BOARD_LED_BLUE_GPIO_PORT, BOARD_LED_BLUE_GPIO_PIN, IOCON_DIGITAL_EN); \
         GPIO_PinInit(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PORT, BOARD_LED_BLUE_GPIO_PIN,         \
                      &(gpio_pin_config_t){kGPIO_DigitalOutput, (output)});                           \
     }
 #define LED_BLUE_OFF()    GPIO_PortSet(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PORT, 1U << BOARD_LED_BLUE_GPIO_PIN)
 #define LED_BLUE_ON()     GPIO_PortClear(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PORT, 1U << BOARD_LED_BLUE_GPIO_PIN)
 #define LED_BLUE_TOGGLE() GPIO_PortToggle(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PORT, 1U << BOARD_LED_BLUE_GPIO_PIN)
 
 #define LED_INIT   LED_BLUE_INIT
 #define LED_OFF    LED_BLUE_OFF
 #define LED_ON     LED_BLUE_ON
 #define LED_TOGGLE LED_BLUE_TOGGLE
 
 #define RGB_LED_BLUE_INIT(output)                                                                            \
     {                                                                                                        \
         IOCON_PinMuxSet(IOCON, BOARD_RGB_LED_BLUE_GPIO_PORT, BOARD_RGB_LED_BLUE_GPIO_PIN, IOCON_DIGITAL_EN); \
         GPIO_PinInit(BOARD_RGB_LED_BLUE_GPIO, BOARD_RGB_LED_BLUE_GPIO_PORT, BOARD_RGB_LED_BLUE_GPIO_PIN,     \
                      &(gpio_pin_config_t){kGPIO_DigitalOutput, (output)});                                   \
     }
 #define RGB_LED_BLUE_OFF() \
     GPIO_PortSet(BOARD_RGB_LED_BLUE_GPIO, BOARD_RGB_LED_BLUE_GPIO_PORT, 1U << BOARD_RGB_LED_BLUE_GPIO_PIN)
 #define RGB_LED_BLUE_ON() \
     GPIO_PortClear(BOARD_RGB_LED_BLUE_GPIO, BOARD_RGB_LED_BLUE_GPIO_PORT, 1U << BOARD_RGB_LED_BLUE_GPIO_PIN)
 #define RGB_LED_BLUE_TOGGLE() \
     GPIO_PortToggle(BOARD_RGB_LED_BLUE_GPIO, BOARD_RGB_LED_BLUE_GPIO_PORT, 1U << BOARD_RGB_LED_BLUE_GPIO_PIN)
 
 #define RGB_LED_RED_INIT(output)                                                                           \
     {                                                                                                      \
         IOCON_PinMuxSet(IOCON, BOARD_RGB_LED_RED_GPIO_PORT, BOARD_RGB_LED_RED_GPIO_PIN, IOCON_DIGITAL_EN); \
         GPIO_PinInit(BOARD_RGB_LED_RED_GPIO, BOARD_RGB_LED_RED_GPIO_PORT, BOARD_RGB_LED_RED_GPIO_PIN,      \
                      &(gpio_pin_config_t){kGPIO_DigitalOutput, (output)});                                 \
     }
 #define RGB_LED_RED_OFF() \
     GPIO_PortSet(BOARD_RGB_LED_RED_GPIO, BOARD_RGB_LED_RED_GPIO_PORT, 1U << BOARD_RGB_LED_RED_GPIO_PIN)
 #define RGB_LED_RED_ON() \
     GPIO_PortClear(BOARD_RGB_LED_RED_GPIO, BOARD_RGB_LED_RED_GPIO_PORT, 1U << BOARD_RGB_LED_RED_GPIO_PIN)
 #define RGB_LED_RED_TOGGLE() \
     GPIO_PortToggle(BOARD_RGB_LED_RED_GPIO, BOARD_RGB_LED_RED_GPIO_PORT, 1U << BOARD_RGB_LED_RED_GPIO_PIN)
 
 #define RGB_LED_GREEN_INIT(output)                                                                             \
     {                                                                                                          \
         IOCON_PinMuxSet(IOCON, BOARD_RGB_LED_GREEN_GPIO_PORT, BOARD_RGB_LED_GREEN_GPIO_PIN, IOCON_DIGITAL_EN); \
         GPIO_PinInit(BOARD_RGB_LED_GREEN_GPIO, BOARD_RGB_LED_GREEN_GPIO_PORT, BOARD_RGB_LED_GREEN_GPIO_PIN,    \
                      &(gpio_pin_config_t){kGPIO_DigitalOutput, (output)});                                     \
     }
 #define RGB_LED_GREEN_OFF() \
     GPIO_PortSet(BOARD_RGB_LED_GREEN_GPIO, BOARD_RGB_LED_GREEN_GPIO_PORT, 1U << BOARD_RGB_LED_GREEN_GPIO_PIN)
 #define RGB_LED_GREEN_ON() \
     GPIO_PortClear(BOARD_RGB_LED_GREEN_GPIO, BOARD_RGB_LED_GREEN_GPIO_PORT, 1U << BOARD_RGB_LED_GREEN_GPIO_PIN)
 #define RGB_LED_GREEN_TOGGLE() \
     GPIO_PortToggle(BOARD_RGB_LED_GREEN_GPIO, BOARD_RGB_LED_GREEN_GPIO_PORT, 1U << BOARD_RGB_LED_GREEN_GPIO_PIN)
 
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
 void BOARD_InitHardware(void);
 void BOARD_InitBootPins(void);
 void BOARD_InitDebugConsole(void);
 void BOARD_UninitDebugConsole(void);
 void BOARD_InitDebugConsole2(void);
 void BOARD_UninitDebugConsole2(void);
 void BOARD_InitI2c(void);
 status_t BOARD_I2cWriteFunc(uint8_t deviceAddress, uint32_t subAddress, uint8_t *txBuff, size_t txBuffSize);
 status_t BOARD_I2cReadFunc(uint8_t deviceAddress, uint32_t subAddress, uint8_t *rxBuff, size_t rxBuffSize);
 
 #endif /* _BOARD_H_ */