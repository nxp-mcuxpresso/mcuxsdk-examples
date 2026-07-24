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
    BOARD_InitQTMRPins();

    /* Input: PIO3_0(GPIO, J94-13) -> GPIO1_TrigOut0 -> XBAR2_OUT0 -> XBAR0_IN2 -> QTMR0_IN0 */
    GPIO_PortInit(HSP__GPIO_1);
    gpio_pin_config_t inputConfig = {kGPIO_DigitalInput, 0U};
    GPIO_PinInit(HSP__GPIO_1, 0U, &inputConfig);
    GPIO_SetPinInterruptConfig(HSP__GPIO_1, 0U, kGPIO_ActiveHighTriggerOutputEnable);

    XBAR_Init(kXBAR_HSP_XBAR2);
    XBAR_SetSignalsConnection(kHSP_XBAR_2_InputHspGpio1TrigOut0, kHSP_XBAR_2_OutputHspXbar2Out0);

    XBAR_Init(kXBAR_HSP_XBAR0);
    XBAR_SetSignalsConnection(kHSP_XBAR_0_InputHspXbar2Out0, kHSP_XBAR_0_OutputHspQtmr0In0);

    /* Output: QTMR0_OUT1 -> XBAR2_OUT1 -> XBAR0_IN3 -> XBAR0_OUT1 -> PIO3_1(XBAR0_INOUT01, J94-14) */
    XBAR_SetSignalsConnection(kHSP_XBAR_2_InputHspQtmr0Out1, kHSP_XBAR_2_OutputHspXbar2Out1);
    XBAR_SetSignalsConnection(kHSP_XBAR_0_InputHspXbar2Out1, kHSP_XBAR_0_OutputHspXbar1Out1);
}
/*${function:end}*/
