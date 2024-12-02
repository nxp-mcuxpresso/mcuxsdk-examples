/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdarg.h>
#include "fsl_sema42.h"
#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_flexio_uart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifdef PRINTF
#undef PRINTF
#define PRINTF EZHV_PRINTF
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t txbuff[]   = "Flexio uart polling transfer example\r\nBoard will send back received characters\r\n";
uint8_t rxbuff[20] = {0U};

/*******************************************************************************
 * Code
 ******************************************************************************/
void EZHV_PRINTF(const char *ptr, ...)
{
    va_list ap;
    SEMA42_Lock(DEMO_SEMA42_INSTANCE, DEMO_SEMA42_PRINTF_GATE, DEMO_SEMA42_CORE_ID_EZHV);
    va_start(ap, ptr);
    DbgConsole_Vprintf(ptr, ap);
    va_end(ap);
    SEMA42_Unlock(DEMO_SEMA42_INSTANCE, DEMO_SEMA42_PRINTF_GATE);
}

/*! @brief Main function */
int main(void)
{
    FLEXIO_UART_Type uartDev;
    flexio_uart_config_t config;
    uint8_t ch;

    BOARD_InitHardware();

    /* SEMA42 init */
    SEMA42_Init(DEMO_SEMA42_INSTANCE);

    FLEXIO_UART_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
    config.enableUart   = true;

    uartDev.flexioBase      = BOARD_FLEXIO_BASE;
    uartDev.TxPinIndex      = FLEXIO_UART_TX_PIN;
    uartDev.RxPinIndex      = FLEXIO_UART_RX_PIN;
    uartDev.shifterIndex[0] = 0U;
    uartDev.shifterIndex[1] = 1U;
    uartDev.timerIndex[0]   = 0U;
    uartDev.timerIndex[1]   = 1U;

    (void)FLEXIO_UART_Init(&uartDev, &config, FLEXIO_CLOCK_FREQUENCY);

    PRINTF("Flexio uart polling transfer demo - message from ezhv core side.\r\n");
    PRINTF("Press any key to start the program.\r\n");
    GETCHAR();

    FLEXIO_UART_WriteBlocking(&uartDev, txbuff, sizeof(txbuff) - 1);
    while (1)
    {
        FLEXIO_UART_ReadBlocking(&uartDev, &ch, 1U);
        FLEXIO_UART_WriteBlocking(&uartDev, &ch, 1U);
    }
}
