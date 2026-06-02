/*
 * Copyright 2023-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "lvgl_support.h"
#include "fsl_smartdma.h"
#include "fsl_inputmux_connections.h"
#include "fsl_inputmux.h"
/*${header:end}*/

/*${function:start}*/
static void BOARD_InitSmartDMA(void)
{
    RESET_ClearPeripheralReset(kMUX_RST_SHIFT_RSTn);

    INPUTMUX_Init(INPUTMUX0);
    INPUTMUX_AttachSignal(INPUTMUX0, 0, kINPUTMUX_FlexioToSmartDma);

    /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */
    INPUTMUX_Deinit(INPUTMUX0);

    SMARTDMA_InitWithoutFirmware();

    NVIC_EnableIRQ(SMARTDMA_IRQn);
    NVIC_SetPriority(SMARTDMA_IRQn, 3);
}

void BOARD_InitHardware(void)
{
    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);

    /* attach FRO 12M to FLEXCOMM2 */
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM2);
    CLOCK_AttachClk(kPLL0_to_FLEXIO);

    CLOCK_SetClkDiv(kCLOCK_DivFlexcom2Clk, 1u);
    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, 1u);
    BOARD_InitBootClocks();
    CLOCK_EnableClock(kCLOCK_Port4);
    BOARD_I2C_ReleaseBus(2);
    BOARD_InitBootPins();
    BOARD_InitDebugConsole();
    
    /* Init smartdma. */
    BOARD_InitSmartDMA();
}

void BOARD_LCD_INT_IRQHandler(void)
{
    uint32_t flags = GPIO_GpioGetInterruptFlags(BOARD_LCD_INT_GPIO);

    GPIO_GpioClearInterruptFlags(BOARD_LCD_INT_GPIO, flags);

    if ((flags & (1UL << BOARD_LCD_INT_PIN)) != 0U)
    {
        BOARD_TouchIntHandler();
    }

    SDK_ISR_EXIT_BARRIER;
}
/*${function:end}*/
