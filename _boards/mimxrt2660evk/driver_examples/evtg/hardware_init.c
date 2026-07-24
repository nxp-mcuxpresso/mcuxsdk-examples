/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "app.h"
#include "fsl_xbar.h"
#include "fsl_gpio.h"
/*${header:end}*/

/*${function:start}*/
void IO_Config(void)
{
    /* Pin → EVTG path: PIO3_0/3_1 (GPIO1 input) → GPIO1_TrigOut0/1 → XBAR2 → XBAR1 → EVTG0_INA0/INB0
     * EVTG0_OUTA0 → XBAR0 → XBAR0_INOUT26 (mux=A) → PIO2_26 (LED) */

    /* GPIO1: enable clock, configure PIO3_0/3_1 as inputs */
    GPIO_PortInit(HSP__GPIO_1);
    gpio_pin_config_t inputConfig = {kGPIO_DigitalInput, 0U};
    GPIO_PinInit(HSP__GPIO_1, 0U, &inputConfig);
    GPIO_PinInit(HSP__GPIO_1, 1U, &inputConfig);

    /* PIO3_0 → TrigOut0 (channel 0, active-high level) */
    GPIO_SetPinInterruptConfig(HSP__GPIO_1, 0U, kGPIO_ActiveHighTriggerOutputEnable);

    /* PIO3_1 → TrigOut1 (channel 1, active-high level) */
    GPIO_SetPinInterruptConfig(HSP__GPIO_1, 1U, kGPIO_ActiveHighTriggerOutputEnable);
    GPIO_SetPinInterruptChannel(HSP__GPIO_1, 1U, kGPIO_InterruptOutput1);

    /* XBAR2: GPIO1_TrigOut0 → XBAR2_OUT0, GPIO1_TrigOut1 → XBAR2_OUT1 */
    XBAR_Init(kXBAR_HSP_XBAR2);
    XBAR_SetSignalsConnection(kHSP_XBAR_2_InputHspGpio1TrigOut0, kHSP_XBAR_2_OutputHspXbar2Out0);
    XBAR_SetSignalsConnection(kHSP_XBAR_2_InputHspGpio1TrigOut1, kHSP_XBAR_2_OutputHspXbar2Out1);

    /* XBAR1: XBAR2_OUT0 → EVTG0_INA0, XBAR2_OUT1 → EVTG0_INB0 */
    XBAR_Init(kXBAR_HSP_XBAR1);
    XBAR_SetSignalsConnection(kHSP_XBAR_1_InputHspXbar2Out0, kHSP_XBAR_1_OutputHspEvtg0Ina0);
    XBAR_SetSignalsConnection(kHSP_XBAR_1_InputHspXbar2Out1, kHSP_XBAR_1_OutputHspEvtg0Inb0);

    /* XBAR0: EVTG0_OUTA0 → XBAR0_INOUT26 (PIO2_26, mux=A, LED) */
    XBAR_Init(kXBAR_HSP_XBAR0);
    XBAR_SetSignalsConnection(kHSP_XBAR_0_InputHspEvtg0Outa0, kHSP_XBAR_0_OutputHspXbar1Out26);
}

void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitEVTGPins();
    CLOCK_EnableClock(kCLOCK_MAIN_hsp_evtg0);
}
/*${function:end}*/
