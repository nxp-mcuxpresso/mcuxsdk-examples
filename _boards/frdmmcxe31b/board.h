/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "clock_config.h"
#include "fsl_common.h"
#include "fsl_siul2.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME "FRDM-MCXE31B"

/*! @brief The UART to use for debug messages. */
#define BOARD_DEBUG_UART_TYPE kSerialPort_Uart

#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART_5
#define BOARD_DEBUG_UART_INSTANCE 5U

#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetFreq(kCLOCK_Lpuart5Clk)

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/* Board RGB LED color mapping */
#define LOGIC_LED_ON 0U
#define LOGIC_LED_OFF 1U
#ifndef BOARD_LED_RED_GPIO
#define BOARD_LED_RED_GPIO kSIUL2_PTC
#endif
#ifndef BOARD_LED_RED_GPIO_PIN
#define BOARD_LED_RED_GPIO_PIN 16U
#endif
#ifndef BOARD_LED_GREEN_GPIO
#define BOARD_LED_GREEN_GPIO kSIUL2_PTB
#endif
#ifndef BOARD_LED_GREEN_GPIO_PIN
#define BOARD_LED_GREEN_GPIO_PIN 22U
#endif
#ifndef BOARD_LED_BLUE_GPIO
#define BOARD_LED_BLUE_GPIO kSIUL2_PTC
#endif
#ifndef BOARD_LED_BLUE_GPIO_PIN
#define BOARD_LED_BLUE_GPIO_PIN 14U
#endif

#define GPIO_OUTPUT_LOW_LOGIC  0U
#define GPIO_OUTPUT_HIGH_LOGIC 1U
#define BOARD_ACCEL_I2C_SCL_PIN  14U
#define BOARD_ACCEL_I2C_SDA_PIN  13U
#define BOARD_ACCEL_I2C_SDA_GPIO kSIUL2_PTD
#define BOARD_ACCEL_I2C_SCL_GPIO kSIUL2_PTD
#define SIUL2_MSCR_REGISTER_OFFSET_INDEX(Port, Pin_Index) (Port * 32 + Pin_Index)
#define BOARD_ACCEL_I2C_SCL_PIN_MSCR_OFFSET SIUL2_MSCR_REGISTER_OFFSET_INDEX(BOARD_ACCEL_I2C_SCL_GPIO, BOARD_ACCEL_I2C_SCL_PIN)
#define BOARD_ACCEL_I2C_SDA_PIN_MSCR_OFFSET SIUL2_MSCR_REGISTER_OFFSET_INDEX(BOARD_ACCEL_I2C_SDA_GPIO, BOARD_ACCEL_I2C_SDA_PIN)
#define BOARD_ACCEL_I2C_SCL_PIN_IMCR_OFFSET 212
#define BOARD_ACCEL_I2C_SDA_PIN_IMCR_OFFSET 214

#define LED_RED_INIT(output)                                                   \
  SIUL2_PortPinWrite(SIUL2, BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, output)
#define LED_RED_ON()                                                           \
  SIUL2_PortPinWrite(SIUL2, BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN,        \
                     LOGIC_LED_ON)
#define LED_RED_OFF()                                                          \
  SIUL2_PortPinWrite(SIUL2, BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN,        \
                     LOGIC_LED_OFF)
#define LED_RED_TOGGLE()                                                       \
  SIUL2_PortToggle(SIUL2, BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN)

#define LED_GREEN_INIT(output)                                                 \
  SIUL2_PortPinWrite(SIUL2, BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN,    \
                     output)
#define LED_GREEN_ON()                                                         \
  SIUL2_PortPinWrite(SIUL2, BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN,    \
                     LOGIC_LED_ON)
#define LED_GREEN_OFF()                                                        \
  SIUL2_PortPinWrite(SIUL2, BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN,    \
                     LOGIC_LED_OFF)
#define LED_GREEN_TOGGLE()                                                     \
  SIUL2_PortToggle(SIUL2, BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN)

#define LED_BLUE_INIT(output)                                                  \
  SIUL2_PortPinWrite(SIUL2, BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN,      \
                     output)
#define LED_BLUE_ON()                                                          \
  SIUL2_PortPinWrite(SIUL2, BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN,      \
                     LOGIC_LED_ON)
#define LED_BLUE_OFF()                                                         \
  SIUL2_PortPinWrite(SIUL2, BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN,      \
                     LOGIC_LED_OFF)
#define LED_BLUE_TOGGLE()                                                      \
  SIUL2_PortToggle(SIUL2, BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN)

/* Board SW PIN */
#ifndef BOARD_SW2_GPIO
#define BOARD_SW2_GPIO kSIUL2_PTB
#endif
#ifndef BOARD_SW2_GPIO_PIN
#define BOARD_SW2_GPIO_PIN 23U
#endif

#ifndef BOARD_SW3_GPIO
#define BOARD_SW3_GPIO kSIUL2_PTD
#endif
#ifndef BOARD_SW3_GPIO_PIN
#define BOARD_SW3_GPIO_PIN 5U
#endif

/* @Brief Board accelerator sensor configuration */
#define BOARD_ACCEL_I2C_BASEADDR LPI2C_0
/* Clock divider for LPI2C clock source */
#define BOARD_ACCEL_I2C_CLOCK_FREQ (CLOCK_GetFreq(kCLOCK_Lpi2c0Clk))

/* @Brief Board I2C magnetic switch configuration */
#define BOARD_MAGSWITCH_I2C_BASEADDR   LPI2C_0
#define BOARD_MAGSWITCH_I2C_CLOCK_FREQ (CLOCK_GetFreq(kCLOCK_Lpi2c0Clk))

/*! @brief The EMAC PHY address. */
#define BOARD_EMAC_PHY_ADDRESS (0x0U) /* Phy address of EMAC port. */

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {

#endif /* __cplusplus */
/*! @brief Init the debug console. */
void BOARD_InitDebugConsole(void);
/*! @brief Config the MPU. */
void BOARD_ConfigMPU(void);
void BOARD_ClockPreConfig(void);
void BOARD_ClockPostConfig(void);
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
status_t BOARD_LPI2C_SendSCCB(LPI2C_Type *base,
                              uint8_t deviceAddress,
                              uint32_t subAddress,
                              uint8_t subaddressSize,
                              uint8_t *txBuff,
                              uint8_t txBuffSize);
status_t BOARD_LPI2C_ReceiveSCCB(LPI2C_Type *base,
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
status_t BOARD_MagSwitch_I2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t data);
status_t BOARD_MagSwitch_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
#endif /* SDK_I2C_BASED_COMPONENT_USED */
/*!
 * @brief Set the onboard I2C2 Pins to GPIO.
 */
void BOARD_InitI2c0PinAsGpio(void);
/*!
 * @brief Restore the pin mux configuration for onboard I2C2 pins.
 */
void BOARD_RestoreI2c0PinMux(void);
/*!
 * @brief Recover onboard I2C2 bus.
 */
void BOARD_I2c0RecoverBus(void);
/*!
 * @brief Release onboard I2C bus. This macro is used to release the onboard I2C bus which may connected to sensors or
 * codec. NOTE, the corresponding APIs called by this macro need to be defined.
 */
#define BOARD_I2C_ReleaseBus(n)        \
    do                                 \
    {                                  \
        BOARD_InitI2c##n##PinAsGpio(); \
        BOARD_I2c##n##RecoverBus();    \
        BOARD_RestoreI2c##n##PinMux(); \
    } while (0);

/*!
 * @brief Release Accel I2C bus. This macro is used to release the onboard accel I2C.
 */
#define BOARD_Accel_I2C_ReleaseBus() BOARD_I2C_ReleaseBus(0)
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* BOARD_H_ */
