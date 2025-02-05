/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_iocon.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
#include "fsl_dma.h"
#include "fsl_pint.h"
#include "fsl_inputmux.h"

/*${header:end}*/

/*${variable:start}*/
dma_channel_trigger_t s_channelTrigger = {
    .type  = kDMA_RisingEdgeTrigger,
    .burst = kDMA_SingleTransfer,
    .wrap  = kDMA_NoWrap,
};
/*${variable:end}*/

/*${function:start}*/
void PINT_Callback(pint_pin_int_t pintr, pint_status_t *status)
{
    PRINTF("\r\n\r\nSW4 is pressed.");
}

void BOARD_InitHardware(void)
{
    POWER_Init();

    CLOCK_EnableClock(kCLOCK_Iocon);
    CLOCK_EnableClock(kCLOCK_Gpio0);
    GPIO_PortInit(GPIO, 0);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM0);
    CLOCK_EnableClock(kCLOCK_FlexComm0);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

void DMA_HardwareTriggerConfig()
{
    PINT_Init(PINT);
    /* Connect trigger sources to PINT */
    INPUTMUX_Init(INPUTMUX);
    INPUTMUX_AttachSignal(INPUTMUX, kPINT_PinInt0, kINPUTMUX_GpioPort0Pin18ToPintsel);
    /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */
    INPUTMUX_Deinit(INPUTMUX);

    PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableRiseEdge);
    PINT_SetCallback(PINT, PINT_Callback);

    PINT_EnableCallbackByIndex(PINT, kPINT_PinInt0);

    PRINTF("\r\n\r\nPress SW4 to trigger one shot DMA transfer.");
}
/*${function:end}*/
