/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_dma.h"
#include "fsl_pint.h"
#include "fsl_syscon.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${variable:start}*/
dma_channel_trigger_t s_channelTrigger = {
    .type  = kDMA_RisingEdgeTrigger,
    .burst = kDMA_SingleTransfer,
    .wrap  = kDMA_NoWrap,
};
/*${variable:end}*/

/*${function:start}*/
void PINT_Callback(pint_pin_int_t pintr, uint32_t pmatch_status)
{
    PRINTF("\r\n\r\nSW1 is pressed.");
}

void BOARD_InitHardware(void)
{
    /* Attach 12 MHz clock to USART0 (debug console) */
    CLOCK_Select(BOARD_DEBUG_USART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_BootClockFRO30M();
    BOARD_InitDebugConsole();
}

void DMA_HardwareTriggerConfig()
{
    PINT_Init(PINT);

    SYSCON_AttachSignal(SYSCON, kPINT_PinInt4, kSYSCON_GpioPort0Pin12ToPintsel);

    PINT_PinInterruptConfig(PINT, kPINT_PinInt4, kPINT_PinIntEnableRiseEdge, PINT_Callback);

    PINT_EnableCallbackByIndex(PINT, kPINT_PinInt4);

    PRINTF("\r\n\r\nPress SW1 to trigger one shot DMA transfer.");
}
/*${function:end}*/
