/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "fsl_gpio.h"
#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME "FRDM-MCXL255"
/*! @brief The manufacturer name */
#define MANUFACTURER_NAME "NXP"
/*! @brief The UART to use for debug messages. */
#define BOARD_DEBUG_UART_TYPE     kSerialPort_Uart

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200U
#endif

#if __CORTEX_M == (33U) /* Building on the main core */
#define BOARD_DEBUG_UART_BASEADDR   (uint32_t) LPUART0
#define BOARD_DEBUG_UART_INSTANCE   0U
#define BOARD_DEBUG_UART_CLK_ATTACH kFRO12M_to_LPUART0
#define BOARD_DEBUG_UART_RST        kLPUART0_RST_SHIFT_RSTn
#define BOARD_DEBUG_UART_CLKSRC     kCLOCK_LPUART0
#define BOARD_UART_IRQ_HANDLER      LPUART0_IRQHandler
#define BOARD_UART_IRQ              LPUART0_IRQn
#define BOARD_DEBUG_UART_CLK_FREQ   CLOCK_GetLpuartClkFreq(BOARD_DEBUG_UART_INSTANCE)
#else /* Build on AON */
#define BOARD_DEBUG_UART_BASEADDR   (uint32_t) AON__LPUART0
#define BOARD_DEBUG_UART_INSTANCE   2U
#define BOARD_DEBUG_UART_CLK_ATTACH kFROdiv4_to_AON_COM
#define BOARD_DEBUG_UART_RST        kAonUART_RST_SHIFT_RSTn
#define BOARD_DEBUG_UART_CLKSRC     kCLOCK_GateAonUART
#define BOARD_UART_IRQ_HANDLER      LPUART0_IRQHandler
#define BOARD_UART_IRQ              LPUART0_IRQn
#define BOARD_DEBUG_UART_CLK_FREQ   CLOCK_GetLpuartClkFreq(BOARD_DEBUG_UART_INSTANCE)
#endif /*__CORTEX_M == (33U) */

#if __CORTEX_M == (33U) /* Building on the main core */
/* @Brief Board accelerator sensor configuration */
#define BOARD_ACCEL_I2C_BASEADDR   LPI2C0
#define BOARD_ACCEL_I2C_INSTANCE   0U
#define BOARD_ACCEL_I2C_CLOCK_FREQ CLOCK_GetLpi2cClkFreq(BOARD_ACCEL_I2C_INSTANCE)

/* @Brief Board I2C magnetic switch configuration */
#define BOARD_MAGSWITCH_I2C_BASEADDR   AON__LPI2C0
#define BOARD_MAGSWITCH_I2C_INSTANCE   2U
#define BOARD_MAGSWITCH_I2C_CLOCK_FREQ CLOCK_GetLpi2cClkFreq(BOARD_MAGSWITCH_I2C_INSTANCE)
#endif /*__CORTEX_M == (33U) */

#if __CORTEX_M == (33U) /* Building on the main core */
/*! @brief GPIO for LED. */
#ifndef BOARD_LED_RED_GPIO
#define BOARD_LED_RED_GPIO GPIO1
#endif
#ifndef BOARD_LED_RED_GPIO_PIN
#define BOARD_LED_RED_GPIO_PIN 15U
#endif

#ifndef BOARD_LED_GREEN_GPIO
#define BOARD_LED_GREEN_GPIO GPIO1
#endif
#ifndef BOARD_LED_GREEN_GPIO_PIN
#define BOARD_LED_GREEN_GPIO_PIN 16U
#endif

#ifndef BOARD_LED_BLUE_GPIO
#define BOARD_LED_BLUE_GPIO GPIO2
#endif
#ifndef BOARD_LED_BLUE_GPIO_PIN
#define BOARD_LED_BLUE_GPIO_PIN 12U
#endif
#endif /*__CORTEX_M == (33U) */

/*! @brief GPIO for SW. */
#ifndef BOARD_SW2_GPIO
#define BOARD_SW2_GPIO AON__GPIO0
#endif
#ifndef BOARD_SW2_GPIO_PIN
#define BOARD_SW2_GPIO_PIN 9U
#endif
#define BOARD_SW2_NAME        "SW2"
#define BOARD_SW2_IRQ         GPIO00_AON_IRQn
#define BOARD_SW2_IRQ_HANDLER GPIO00_AON_IRQHandler

#if __CORTEX_M == (33U) /* Building on the main core */
#ifndef BOARD_SW5_GPIO
#define BOARD_SW5_GPIO GPIO1
#endif
#ifndef BOARD_SW5_GPIO_PIN
#define BOARD_SW5_GPIO_PIN 14U
#endif
#define BOARD_SW5_NAME        "SW5"
#define BOARD_SW5_IRQ         GPIO10_IRQn
#define BOARD_SW5_IRQ_HANDLER GPIO10_IRQHandler
#endif /*__CORTEX_M == (33U) */

#if __CORTEX_M == (33U) /* Building on the main core */
/* Board LED color mapping */
/* Logic already inverted in pin_mux.c */
#define LOGIC_LED_ON  0U
#define LOGIC_LED_OFF 1U

#define LED_RED_INIT(output)                                           \
    GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, output); \
    BOARD_LED_RED_GPIO->PDDR |= (1U << BOARD_LED_RED_GPIO_PIN)               /*!< Enable target LED_RED */
#define LED_RED_ON() \
    GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, LOGIC_LED_ON)  /*!< Turn on target LED_RED */
#define LED_RED_OFF() \
    GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, LOGIC_LED_OFF) /*!< Turn off target LED_RED */
#define LED_RED_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN)        /*!< Toggle on target LED_RED */

#define LED_GREEN_INIT(output)                                             \
    GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, output); \
    BOARD_LED_GREEN_GPIO->PDDR |= (1U << BOARD_LED_GREEN_GPIO_PIN)             /*!< Enable target LED_GREEN */
#define LED_GREEN_ON() \
    GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, LOGIC_LED_ON)  /*!< Turn on target LED_GREEN */
#define LED_GREEN_OFF() \
    GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, LOGIC_LED_OFF) /*!< Turn off target LED_GREEN */
#define LED_GREEN_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN)      /*!< Toggle on target LED_GREEN */

#define LED_BLUE_INIT(output)                                            \
    GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, output); \
    BOARD_LED_BLUE_GPIO->PDDR |= (1U << BOARD_LED_BLUE_GPIO_PIN)              /*!< Enable target LED_BLUE */
#define LED_BLUE_ON() \
    GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, LOGIC_LED_ON)  /*!< Turn on target LED_BLUE */
#define LED_BLUE_OFF() \
    GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, LOGIC_LED_OFF) /*!< Turn off target LED_BLUE */
#define LED_BLUE_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN)       /*!< Toggle on target LED_BLUE */

#else /* Build on AON */
#define LED_RED_INIT(output)                                           
#define LED_RED_ON() 
#define LED_RED_OFF() 
#define LED_RED_TOGGLE() 

#define LED_GREEN_INIT(output)
#define LED_GREEN_ON() 
#define LED_GREEN_OFF() 
#define LED_GREEN_TOGGLE() 
      
#define LED_BLUE_INIT(output)                                            
#define LED_BLUE_ON() 
#define LED_BLUE_OFF() 
#define LED_BLUE_TOGGLE() 

#endif /* Build on AON */

/*******************************************************************************
 * API
 ******************************************************************************/
void BOARD_InitDebugConsole(void);
#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
void BOARD_LPI2C_Init(LPI2C_Type *base, uint32_t clkSrc_Hz);
status_t BOARD_LPI2C_Send(LPI2C_Type *base,
                          uint8_t deviceAddress,
                          uint32_t subAddress,
                          uint8_t subaddressSize,
                          uint8_t *txBuff,
                          uint8_t txBuffSize);
status_t BOARD_LPI2C_Receive(LPI2C_Type *base,
                             uint8_t deviceAddress,
                             uint32_t subAddress,
                             uint8_t subaddressSize,
                             uint8_t *rxBuff,
                             uint8_t rxBuffSize);
void BOARD_Accel_I2C_Init(void);
status_t BOARD_Accel_I2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint32_t txBuff);
status_t BOARD_Accel_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);

void BOARD_MagSwitch_I2C_Init(void);
status_t BOARD_MagSwitch_I2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint32_t txBuff);
status_t BOARD_MagSwitch_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
#endif /* SDK_I2C_BASED_COMPONENT_USED */
#endif /* _BOARD_H_ */
