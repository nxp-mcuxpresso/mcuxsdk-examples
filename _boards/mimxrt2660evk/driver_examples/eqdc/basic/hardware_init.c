/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_xbar.h"
#include "fsl_gpio.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
    BOARD_InitEQDCPins();
    
    /* PHASE_A: PIO3_0 (GPIO1_GPIO0,  J94-13) -> GPIO1_TrigOut0 -> XBAR2_OUT0 -> XBAR0 -> EQDC0_PHASE_A
     * PHASE_B: PIO3_1 (GPIO1_GPIO1,  J94-14) -> GPIO1_TrigOut1 -> XBAR2_OUT1 -> XBAR0 -> EQDC0_PHASE_B
     * INDEX:   PIO2_8 (GPIO0_GPIO8,  J33-4)  -> GPIO0_TrigOut0 -> XBAR2_OUT2 -> XBAR0 -> EQDC0_INDEX */

    GPIO_PortInit(HSP__GPIO_1);
    gpio_pin_config_t inputConfig = {kGPIO_DigitalInput, 0U};
    GPIO_PinInit(HSP__GPIO_1, 0U, &inputConfig);
    GPIO_PinInit(HSP__GPIO_1, 1U, &inputConfig);
    GPIO_SetPinInterruptConfig(HSP__GPIO_1, 0U, kGPIO_ActiveHighTriggerOutputEnable);
    GPIO_SetPinInterruptConfig(HSP__GPIO_1, 1U, kGPIO_ActiveHighTriggerOutputEnable);
    GPIO_SetPinInterruptChannel(HSP__GPIO_1, 1U, kGPIO_InterruptOutput1);

    GPIO_PortInit(HSP__GPIO_0);
    GPIO_PinInit(HSP__GPIO_0, 8U, &inputConfig);
    GPIO_SetPinInterruptConfig(HSP__GPIO_0, 8U, kGPIO_ActiveHighTriggerOutputEnable);

    XBAR_Init(kXBAR_HSP_XBAR2);
    XBAR_SetSignalsConnection(kHSP_XBAR_2_InputHspGpio1TrigOut0, kHSP_XBAR_2_OutputHspXbar2Out0);
    XBAR_SetSignalsConnection(kHSP_XBAR_2_InputHspGpio1TrigOut1, kHSP_XBAR_2_OutputHspXbar2Out1);
    XBAR_SetSignalsConnection(kHSP_XBAR_2_InputHspGpio0TrigOut0, kHSP_XBAR_2_OutputHspXbar2Out2);

    XBAR_Init(kXBAR_HSP_XBAR0);
    XBAR_SetSignalsConnection(kHSP_XBAR_0_InputHspXbar2Out0, kHSP_XBAR_0_OutputHspEqdc0PhaseAIn);
    XBAR_SetSignalsConnection(kHSP_XBAR_0_InputHspXbar2Out1, kHSP_XBAR_0_OutputHspEqdc0PhaseBIn);
    XBAR_SetSignalsConnection(kHSP_XBAR_0_InputHspXbar2Out2, kHSP_XBAR_0_OutputHspEqdc0IndexIn);
}
/*${function:end}*/
