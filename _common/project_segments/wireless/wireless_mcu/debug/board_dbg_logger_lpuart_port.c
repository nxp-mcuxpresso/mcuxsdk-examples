/*
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board_dbg_logger_port.h"
#include "fsl_lpuart.h"
#include "fsl_clock.h"
#include "pin_mux.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/************************************************************************************
 * Private memory declarations
 ************************************************************************************/
static const lpuart_config_t uart_config = {
    .baudRate_Bps  = BOARD_DEBUG_UART_BAUDRATE,
    .dataBitsCount = kLPUART_EightDataBits,
    .parityMode    = kLPUART_ParityDisabled,
    .stopBitCount  = kLPUART_OneStopBit,
    .isMsb         = false,
    .enableTx      = true,
    .enableRx      = false,
};

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
int BOARD_DbgLoggerPortInit(void)
{
    int ret = 0;

    BOARD_InitPinLPUART0_TX();

    CLOCK_SetIpSrc(BOARD_DEBUG_UART_CLK, BOARD_DEBUG_UART_CLKSRC);
    CLOCK_EnableClockLPMode(BOARD_DEBUG_UART_CLK, kCLOCK_IpClkControl_fun3);

    if (LPUART_Init((LPUART_Type*)BOARD_DEBUG_UART_BASEADDR, &uart_config, BOARD_DEBUG_UART_CLK_FREQ) != kStatus_Success)
    {
        ret = -1;
    }

    return ret;
}

int BOARD_DbgLoggerPortWrite(const uint8_t *data, uint16_t len)
{
    int ret = 0;

    if (LPUART_WriteBlocking((LPUART_Type*)BOARD_DEBUG_UART_BASEADDR, data, len) != kStatus_Success)
    {
        ret = -1;
    }

    return ret;
}

int BOARD_DbgLoggerPortReinit(void)
{
    return BOARD_DbgLoggerPortInit();
}
