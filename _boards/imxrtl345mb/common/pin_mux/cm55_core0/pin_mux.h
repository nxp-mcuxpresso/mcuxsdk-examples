/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/*!
 * @addtogroup pin_mux
 * @{
 */

#define APP_GPIO_PORT0         0U
#define APP_GPIO_PORT1         1U

#define APP_UART_TX_SWM_PIN         SWM0_07
#define APP_UART_TX_SWM_MODE        SWM_MODE_U_FC3_TXD_SCL_MISO_IO0
#define APP_UART_RX_SWM_PIN         SWM0_06
#define APP_UART_RX_SWM_MODE        SWM_MODE_U_FC3_RXD_SDA_MOSI_CSN

/* USART (FC0) pin definitions */
#define APP_UART0_TX_SWM_PIN         SWM0_03
#define APP_UART0_TX_SWM_MODE        SWM_MODE_U_FC0_TXD_SCL_MISO_IO0
#define APP_UART0_RX_SWM_PIN         SWM0_02
#define APP_UART0_RX_SWM_MODE        SWM_MODE_U_FC0_RXD_SDA_MOSI_CSN

/* SPI (FC0) pin definitions */
#define APP_SPI_SCK_SWM_PIN         SWM0_01
#define APP_SPI_SCK_SWM_MODE        SWM_MODE_U_FC0_SCK
#define APP_SPI_MOSI_SWM_PIN        SWM0_02
#define APP_SPI_MOSI_SWM_MODE       SWM_MODE_U_FC0_RXD_SDA_MOSI_CSN
#define APP_SPI_MISO_SWM_PIN        SWM0_03
#define APP_SPI_MISO_SWM_MODE       SWM_MODE_U_FC0_TXD_SCL_MISO_IO0
#define APP_SPI_CS_SWM_PIN          SWM0_00
#define APP_SPI_CS_SWM_MODE         SWM_MODE_U_FC0_CTS_SDAX_SSELN0_IO1

/* QSPI (FC0) pin definitions */
#define APP_QSPI_CS_SWM_PIN         SWM0_00
#define APP_QSPI_CS_SWM_MODE        SWM_MODE_U_FC0_RXD_SDA_MOSI_CSN
#define APP_QSPI_SCK_SWM_PIN        SWM0_01
#define APP_QSPI_SCK_SWM_MODE       SWM_MODE_U_FC0_SCK
#define APP_QSPI_IO0_SWM_PIN        SWM0_02
#define APP_QSPI_IO0_SWM_MODE       SWM_MODE_U_FC0_TXD_SCL_MISO_IO0
#define APP_QSPI_IO1_SWM_PIN        SWM0_03
#define APP_QSPI_IO1_SWM_MODE       SWM_MODE_U_FC0_CTS_SDAX_SSELN0_IO1
#define APP_QSPI_IO2_SWM_PIN        SWM0_04
#define APP_QSPI_IO2_SWM_MODE       SWM_MODE_U_FC0_RTS_SCLX_SSELN1_IO2
#define APP_QSPI_IO3_SWM_PIN        SWM0_05
#define APP_QSPI_IO3_SWM_MODE       SWM_MODE_U_FC0_SSELN2_IO3

#define APP_SWD_CLK_SWM_PIN         SWM3_12
#define APP_SWD_CLK_SWM_MODE        SWM_MODE_U_DAP_SWCLK
#define APP_SWD_IO_SWM_PIN          SWM3_13
#define APP_SWD_IO_SWM_MODE         SWM_MODE_U_DAP_SWDIO
#define APP_SWD_SWO_SWM_PIN         SWM0_10
#define APP_SWD_SWO_SWM_MODE        SWM_MODE_U_DAP_SWO

/* LEDs pins definitions */
#define APP_LED_RED_SWM_PIN         SWM2_01
#define APP_LED_RED_SWM_MODE        SWM_MODE_U_GPIO_GPO25
#define APP_LED_BLUE_SWM_PIN        SWM2_02
#define APP_LED_BLUE_SWM_MODE       SWM_MODE_U_GPIO_GPO26
#define APP_LED_GREEN_SWM_PIN       SWM2_03
#define APP_LED_GREEN_SWM_MODE      SWM_MODE_U_GPIO_GPO27

/* BUTTONs pins definitions */
#define APP_SW1_SWM_PIN             SWM1_08
#define APP_SW1_SWM_MODE            SWM_MODE_U_GPIO_GPO20

/* I2C (FC3) pin definitions */
#define APP_I2C_SCL_SWM_PIN         SWM3_08
#define APP_I2C_SCL_SWM_MODE        SWM_MODE_U_FC4_TXD_SCL_MISO_IO0
#define APP_I2C_SDA_SWM_PIN         SWM3_09
#define APP_I2C_SDA_SWM_MODE        SWM_MODE_U_FC4_RXD_SDA_MOSI_CSN

/* I3C pin definitions */
#define APP_I3C_SCL_SWM_PIN         SWM3_14
#define APP_I3C_SCL_SWM_MODE        SWM_MODE_U_I3C_SCL
#define APP_I3C_SDA_SWM_PIN         SWM3_15
#define APP_I3C_SDA_SWM_MODE        SWM_MODE_U_I3C_SDA

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void); /* Function assigned for the Cortex-M33 */

/*!
 * @brief Configures Debug UART pins routing.
 *
 */
void BOARD_InitDEBUG_UARTPins(void);

/*!
 * @brief Configures UART0 pins routing.
 *
 */
void BOARD_InitUARTPins(void);

/*!
 * @brief Configures BUTTONs pins routing.
 *
 */
void BOARD_InitBUTTONsPins(void);

/*!
 * @brief Configures CTRLSS__SPI_0 (FC0) pin routing.
 *
 */
void BOARD_InitSPIPins(void);

/*!
 * @brief Configures CTRLSS__QSPI0 (FC0) pin routing.
 *
 */
void BOARD_InitQSPIPins(void);

/*!
 * @brief Configures CTRLSS__I2C_4 (FC4) pin routing.
 *
 */
void BOARD_InitI2CPins(void);

/*!
 * @brief Configures CTRLSS__I3C_0 pin routing.
 *
 */
void BOARD_InitI3CPins(void);

/*!
 * @brief Configures LEDs pin routing.
 *
 */
void BOARD_InitLEDsPins(void);

/*!
 * @brief Configures CTimer Capture pins routing.
 *
 */
void BOARD_InitCtimerCapPins(void);


/*!
 * @brief Configures CTimer Match0 pin routing.
 *
 */
void BOARD_InitCtimerMatch0Pins(void);

/*!
 * @brief Configures CTimer Match1 pin routing.
 *
 */
void BOARD_InitCtimerMatch1Pins(void);

/*!
 * @brief Configures SCT output pins routing.
 *
 */
void BOARD_InitSCtimerPins(void);


#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */
