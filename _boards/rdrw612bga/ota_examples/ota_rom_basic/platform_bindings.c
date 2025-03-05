/*
 * Copyright 2016-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "platform_bindings.h"
#include "fsl_usart.h"
#include "board.h"

/* Board specific code to access Debug UART by XMODEM */

static USART_Type *xmodem_usart = (USART_Type *)BOARD_DEBUG_UART;


int xmodem_putc(int c)
{
    uint8_t c8 = c;
    USART_WriteBlocking(xmodem_usart, &c8, 1);
    return c;
}

/* The debug console layer input is already taken by the shell implementation so the read
 * function for xmodem must be done using lower layer
 */
int xmodem_getc(void)
{
    while (USART_GetRxFifoCount(xmodem_usart) == 0)
    {
        ;
    }

    return USART_ReadByte(xmodem_usart);
}

int xmodem_canread(void)
{
    return USART_GetRxFifoCount(xmodem_usart);
}
