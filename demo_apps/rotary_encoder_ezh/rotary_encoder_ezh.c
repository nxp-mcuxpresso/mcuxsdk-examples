/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_pint.h"
#include "pin_mux.h"
#include "fsl_inputmux.h"
#include "clock_config.h"
#include "board.h"
#include "qei_config.h"
#include "fsl_spi.h"
#include "fsl_power.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Variables
 ******************************************************************************/
QEI_Para para;                           /* QEI configuration parameter */
volatile uint32_t coprocessor_stack[10]; /* stack of QEI */
volatile uint32_t QEI_data[16];          /* shared RAM buffer by QEI and ARM CPU */
uint8_t srcBuff[BUFFER_SIZE];
uint16_t shift_code      = 0x1111;
uint8_t shift_number     = 0;
uint8_t code_mode_number = 0;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void Led_Display(uint16_t Code)
{
    spi_transfer_t xfer = {0};

    /* Init Buffer*/
    srcBuff[0] = Code;
    srcBuff[1] = Code >> 8;

    /*Start Transfer*/
    xfer.txData      = srcBuff;
    xfer.rxData      = NULL;
    xfer.dataSize    = sizeof(srcBuff);
    xfer.configFlags = kSPI_FrameAssert;
    SPI_MasterTransferBlocking(EXAMPLE_SPI_MASTER, &xfer);
    gpio_pin_config_t HSPI_CS_HIGH_config = {.pinDirection = kGPIO_DigitalOutput, .outputLogic = 0U};
    /* Initialize GPIO functionality on pin PIO1_1 (pin 59)  */
    GPIO_PinInit(EXAMPLE_HSPI_CS_LOW_GPIO_GPIO, EXAMPLE_HSPI_CS_LOW_GPIO_PORT, EXAMPLE_HSPI_CS_LOW_GPIO_PIN,
                 &HSPI_CS_HIGH_config);

    gpio_pin_config_t HSPI_CS_LOW_config = {.pinDirection = kGPIO_DigitalOutput, .outputLogic = 1U};
    /* Initialize GPIO functionality on pin PIO1_1 (pin 59)  */
    GPIO_PinInit(EXAMPLE_HSPI_CS_LOW_GPIO_GPIO, EXAMPLE_HSPI_CS_LOW_GPIO_PORT, EXAMPLE_HSPI_CS_LOW_GPIO_PIN,
                 &HSPI_CS_LOW_config);
}

/**
 * brief	QEI interrupt handler
 * para 	None;
 * return	None
 */
void QEI_IRQHandler(void)
{
    if (QEI_data[8] == 1)
    { // if QEI have up-to-date data
        PRINTF("Counter: %d \r\n", QEI_data[2]);
        if (QEI_data[3] == 0xB)
        { // if direction is backward
            PRINTF("Direction:Anti-Clockwise\r\n\r\n");
            if (shift_number++ % 2 == 0)
            {
                shift_code = (shift_code >> 1 | (shift_code << 15)); // Rotate Right 1 bit
            }
            Led_Display(shift_code);
        }
        else if (QEI_data[3] == 0xF)
        { // if direction is forward
            PRINTF("Direction:Clockwise \r\n\r\n");
            if (shift_number++ % 2 == 0)
            {
                shift_code = (shift_code << 1 | (shift_code >> 15)); // Rotate Left	1 bit
            }
            Led_Display(shift_code);
        }
        else
        {
            PRINTF("Direction:stop \r\n");
        }
        QEI_data[8] = 0; // clear the flag after fetch by arm core
    }
}
/*!
 * @brief Call back for PINT Pin interrupt 0-7.
 */
void pint_intr_callback(pint_pin_int_t pintr, uint32_t pmatch_status)
{
    switch (code_mode_number++ % 4)
    {
        case 0:
            shift_code = 0x0001;
            break;
        case 1:
            shift_code = 0x0101;
            break;
        case 2:
            shift_code = 0x00FF;
            break;
        case 3:
            shift_code = 0x1111;
            break;
        default:
            break;
    }
    Led_Display(shift_code);
}
/*!
 * @brief Main function
 */
int main(void)
{
    /* SPI init */
    uint32_t srcClock_Hz = 0U;

    BOARD_InitHardware();

    PRINTF("QEI demo.\r\n");
    PRINTF("When the QEI state changes,the counter and direction will be got.\r\n");
    PRINTF("Push the WAKEUP button on %s board to change the LED display model.\r\n", EXAMPLE_BOARD_NAME);

    CLOCK_SetClkDiv(kCLOCK_DivPll0Clk, 0U, true);
    CLOCK_SetClkDiv(kCLOCK_DivPll0Clk, 3U, false);
    /* attach 50 MHz clock to HSLSPI */
    CLOCK_AttachClk(kFRO_HF_DIV_to_HSLSPI);
    /* reset FLEXCOMM for SPI */
    RESET_PeripheralReset(kHSLSPI_RST_SHIFT_RSTn);
    spi_master_config_t masterConfig;
    srcClock_Hz = EXAMPLE_SPI_MASTER_CLK_FREQ;
    SPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.sselNum      = (spi_ssel_t)EXAMPLE_SPI_SSEL;
    masterConfig.sselPol      = (spi_spol_t)EXAMPLE_MASTER_SPI_SPOL;
    masterConfig.baudRate_Bps = 50000000U;
    SPI_MasterInit(EXAMPLE_SPI_MASTER, &masterConfig, srcClock_Hz);
    Led_Display(shift_code);

    /* Connect trigger sources to PINT */
    INPUTMUX_Init(INPUTMUX);
    INPUTMUX_AttachSignal(INPUTMUX, kPINT_PinInt1, DEMO_PINT_PIN_INT1_SRC);
    /* Initialize PINT */
    PINT_Init(PINT);
    /* Setup Pin Interrupt 1 for falling edge */
    PINT_PinInterruptConfig(PINT, kPINT_PinInt1, kPINT_PinIntEnableFallEdge, pint_intr_callback);
    /* Enable callback for PINT1 by Index */
    PINT_EnableCallbackByIndex(PINT, kPINT_PinInt1);

    /* Initialize QEI data buffer. */
    for (int i = 0; i < 16; i++)
    {
        QEI_data[i] = 0;
    }
    QEI_data[10] = COUNT_MAX; /* set QEI count maximum value */
    CLOCK_EnableClock(kCLOCK_Ezhb);
    RESET_PeripheralReset(kEZHB_RST_SHIFT_RSTn);
    para.coprocessor_stack = (void *)coprocessor_stack;
    para.p_buffer          = (uint32_t *)QEI_data;
    QEI_Init(&para);     // camera engine initialization
    QEI_Start();         // start camera engine
    EnableIRQ(QEI_IRQn); /* enable QEI interrupt to ARM core */

    while (1)
    {
    }
}
