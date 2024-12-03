/*
 * Copyright 2023 NXP
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
#include "fsl_inputmux.h"

/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    const gpio_pin_config_t resetPinConfig = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic  = 1,
    };

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, 1u);
    CLOCK_AttachClk(kPLL0_to_FLEXIO);

    CLOCK_EnableClock(kCLOCK_Gpio0);
    CLOCK_EnableClock(kCLOCK_Gpio4);

    /* Set SSD1963 CS, RS, and reset pin to output. */
    GPIO_PinInit(DEMO_SSD1963_RST_GPIO, DEMO_SSD1963_RST_PIN, &resetPinConfig);
    GPIO_PinInit(DEMO_SSD1963_CS_GPIO, DEMO_SSD1963_CS_PIN, &resetPinConfig);
    GPIO_PinInit(DEMO_SSD1963_RS_GPIO, DEMO_SSD1963_RS_PIN, &resetPinConfig);
}

void BOARD_SetCSPin(bool set)
{
    GPIO_PinWrite(DEMO_SSD1963_CS_GPIO, DEMO_SSD1963_CS_PIN, (uint8_t)set);
}

void BOARD_SetRSPin(bool set)
{
    GPIO_PinWrite(DEMO_SSD1963_RS_GPIO, DEMO_SSD1963_RS_PIN, (uint8_t)set);
}

void BOARD_SetResetPin(bool set)
{
    GPIO_PinWrite(DEMO_SSD1963_RST_GPIO, DEMO_SSD1963_RST_PIN, (uint8_t)set);
}

void BOARD_InitSmartDMA(void)
{
    RESET_ClearPeripheralReset(kMUX_RST_SHIFT_RSTn);

    INPUTMUX_Init(INPUTMUX0);
    INPUTMUX_AttachSignal(INPUTMUX0, 0, kINPUTMUX_FlexioShifterDmaRequest0ToSmartDma);

    /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */
    INPUTMUX_Deinit(INPUTMUX0);

    SMARTDMA_InitWithoutFirmware();

    NVIC_EnableIRQ(SMARTDMA_IRQn);
    NVIC_SetPriority(SMARTDMA_IRQn, 3);
}
/*${function:end}*/
