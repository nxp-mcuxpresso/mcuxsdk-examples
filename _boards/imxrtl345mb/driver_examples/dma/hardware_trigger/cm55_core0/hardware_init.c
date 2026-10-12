/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_dma.h"
#include "fsl_pint.h"
#include "fsl_inputmux.h"
#include "fsl_debug_console.h"
#include "app.h"
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
    PRINTF("\r\n\r\nSW1 is pressed.");
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootClocks();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();
    BOARD_InitBUTTONsPins();
}

void DMA_HardwareTriggerConfig()
{
    PINT_Init(PINT);
    /* Connect trigger sources to PINT */
    INPUTMUX_Init(INPUTMUX);
    INPUTMUX_AttachSignal(INPUTMUX, kINPUTMUX_INDEX_PINTSEL0, kINPUTMUX_IoPPadIn1_6ToPINTSELn);
    /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */
    INPUTMUX_Deinit(INPUTMUX);

    PINT_PinInterruptConfig(PINT, kPINT_PinInt0, kPINT_PinIntEnableRiseEdge);
    PINT_SetCallback(PINT, PINT_Callback);

    PINT_EnableCallbackByIndex(PINT, kPINT_PinInt0);

    PRINTF("\r\n\r\nPress SW1 to trigger one shot DMA transfer.");
}
/*${function:end}*/
