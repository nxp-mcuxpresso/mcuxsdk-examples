/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "pin_mux.h"
#include "fsl_swm.h"
#include "fsl_gpio.h"

#define GPIO_INIT_INPUT_PIN(swm_pin) \
    do { \
        gpio_pin_config_t conf = {.pinDirection = kGPIO_DigitalInput, .outputLogic = 0U}; \
        GPIO_PinInit(CTRLSS__GPIO, SWM_TO_GPIO_PORT(swm_pin), SWM_TO_GPIO_PIN(swm_pin), &conf); \
    } while(0)

#define GPIO_INIT_OUTPUT_PIN(swm_pin) \
    do { \
        gpio_pin_config_t conf = {.pinDirection = kGPIO_DigitalOutput, .outputLogic = 0U}; \
        GPIO_PinInit(CTRLSS__GPIO, SWM_TO_GPIO_PORT(swm_pin), SWM_TO_GPIO_PIN(swm_pin), &conf); \
    } while(0)

void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
}

void BOARD_InitPins(void)
{
}

void BOARD_InitDEBUG_UARTPins(void)
{
    /* Use FC3 for UART */
    SWM_CONFIGURE_PIN(DSWM, APP_UART_TX_SWM_PIN, APP_UART_TX_SWM_MODE);
    SWM_CONFIGURE_PIN(DSWM, APP_UART_RX_SWM_PIN, APP_UART_RX_SWM_MODE);
}

void BOARD_InitUARTPins(void)
{
    /* Use FC0 for UART */
    SWM_CONFIGURE_PIN(DSWM, APP_UART0_TX_SWM_PIN, APP_UART0_TX_SWM_MODE);
    SWM_CONFIGURE_PIN(DSWM, APP_UART0_RX_SWM_PIN, APP_UART0_RX_SWM_MODE);
}

void BOARD_InitLEDsPins(void)
{
    GPIO_INIT_OUTPUT_PIN(APP_LED_RED_SWM_PIN);
    GPIO_INIT_OUTPUT_PIN(APP_LED_BLUE_SWM_PIN);
    GPIO_INIT_OUTPUT_PIN(APP_LED_GREEN_SWM_PIN);
    SWM_SetPinMode(DSWM, APP_LED_RED_SWM_PIN, APP_LED_RED_SWM_MODE);
    SWM_SetPinMode(DSWM, APP_LED_BLUE_SWM_PIN, APP_LED_BLUE_SWM_MODE);
    SWM_SetPinMode(DSWM, APP_LED_GREEN_SWM_PIN, APP_LED_GREEN_SWM_MODE);
}

void BOARD_InitBUTTONsPins(void)
{
    /* TODO: Choose Button pins */
    GPIO_INIT_INPUT_PIN(APP_SW1_SWM_PIN);
    SWM_SetPinMode(DSWM, APP_SW1_SWM_PIN, APP_SW1_SWM_MODE);
}

void BOARD_InitSPIPins(void)
{
    /* Use FC0 for SPI */
    SWM_SetPinMode(DSWM, APP_SPI_SCK_SWM_PIN, APP_SPI_SCK_SWM_MODE);      /* Pin J3_13 on HTG-FMCA-SMA-LVDS breakout board */
    SWM_SetPinMode(DSWM, APP_SPI_MOSI_SWM_PIN, APP_SPI_MOSI_SWM_MODE);    /* Pin J3_14 on HTG-FMCA-SMA-LVDS breakout board */
    SWM_SetPinMode(DSWM, APP_SPI_MISO_SWM_PIN, APP_SPI_MISO_SWM_MODE);    /* Pin J3_15 on HTG-FMCA-SMA-LVDS breakout board */
    SWM_SetPinMode(DSWM, APP_SPI_CS_SWM_PIN, APP_SPI_CS_SWM_MODE);        /* Pin J3_16 on HTG-FMCA-SMA-LVDS breakout board */
}

void BOARD_InitI2CPins(void)
{
    SWM_SetPinMode(DSWM, APP_I2C_SCL_SWM_PIN, APP_I2C_SCL_SWM_MODE);
    SWM_SetPinMode(DSWM, APP_I2C_SDA_SWM_PIN, APP_I2C_SDA_SWM_MODE);
}

void BOARD_InitI3CPins(void)
{
    SWM_SetPinMode(DSWM, APP_I3C_SCL_SWM_PIN, APP_I3C_SCL_SWM_MODE);
    SWM_SetPinMode(DSWM, APP_I3C_SDA_SWM_PIN, APP_I3C_SDA_SWM_MODE);
}

void BOARD_InitQSPIPins(void)
{
    /* Use FC0 for QSPI */
    SWM_SetPinMode(DSWM, APP_QSPI_CS_SWM_PIN, APP_QSPI_CS_SWM_MODE);
    SWM_SetPinMode(DSWM, APP_QSPI_SCK_SWM_PIN, APP_QSPI_SCK_SWM_MODE);
    SWM_SetPinMode(DSWM, APP_QSPI_IO0_SWM_PIN, APP_QSPI_IO0_SWM_MODE);
    SWM_SetPinMode(DSWM, APP_QSPI_IO1_SWM_PIN, APP_QSPI_IO1_SWM_MODE);
    SWM_SetPinMode(DSWM, APP_QSPI_IO2_SWM_PIN, APP_QSPI_IO2_SWM_MODE);
    SWM_SetPinMode(DSWM, APP_QSPI_IO3_SWM_PIN, APP_QSPI_IO3_SWM_MODE);
}

void BOARD_InitCtimerMatch0Pins(void) 
{
    /* TODO: Placeholder implementation following the pattern of other methods */   
}

void BOARD_InitCtimerMatch1Pins(void) 
{
    /* TODO: Placeholder implementation following the pattern of other methods */   
}

void BOARD_InitCtimerCapPins(void)
{
    /* TODO: Placeholder implementation following the pattern of other methods */   
}

void BOARD_InitSCtimerPins(void)
{
    /* TODO: Placeholder implementation following the pattern of other methods */   
}
