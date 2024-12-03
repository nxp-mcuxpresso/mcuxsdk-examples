/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "peripherals.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
#pragma interrupt alignsp saveall
void ivINT_ADC_CC0(void);
void ivINT_ADC_CC0(void)
{
    CADC_ConverterA_End_Scan_IRQHandler();
}
#pragma interrupt off

#pragma interrupt alignsp saveall
void ivINT_ADC_CC1(void);
void ivINT_ADC_CC1(void)
{
    CADC_ConverterB_End_Scan_IRQHandler();
}
#pragma interrupt off

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    BOARD_InitDebugConsole();

    EnableIRQWithPriority(BOARD_DEBUG_UART_RX_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    EnableIRQWithPriority(BOARD_DEBUG_UART_TX_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    EnableIRQWithPriority(BOARD_DEBUG_UART_RX_ERR_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    EnableIRQWithPriority(BOARD_DEBUG_UART_TRX_IDLE_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    SetIRQBasePriority(0);
}
/*${function:end}*/
