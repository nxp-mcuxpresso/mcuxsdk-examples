/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "app.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_smartdma.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Attach peripheral clock */
    CLOCK_AttachClk(kFRO_HF_to_FLEXIO0);
    CLOCK_SetClockDiv(kCLOCK_DivFLEXIO0, 1u);

    CLOCK_EnableClock(kCLOCK_GateFLEXIO0);

    BOARD_InitBootPins();
    BOARD_InitFlexioMculcdPins();
    BOARD_InitFlexioMculcdSmartdmaTrigger();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

void BOARD_SetCSPin(bool set)
{
    GPIO_PinWrite(BOARD_LCD_CS_GPIO, BOARD_LCD_CS_PIN, (uint8_t)set);
}

void BOARD_SetRSPin(bool set)
{
    GPIO_PinWrite(BOARD_LCD_RS_GPIO, BOARD_LCD_RS_PIN, (uint8_t)set);
}

void BOARD_SetResetPin(bool set)
{
    GPIO_PinWrite(BOARD_LCD_RST_GPIO, BOARD_LCD_RST_PIN, (uint8_t)set);
}

void BOARD_InitSmartDMA(void)
{
    SMARTDMA_InitWithoutFirmware();

    NVIC_EnableIRQ(SMARTDMA_IRQn);
    NVIC_SetPriority(SMARTDMA_IRQn, 3);
}
/*${function:end}*/
