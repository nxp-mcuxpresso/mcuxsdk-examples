/*
 * Copyright 2024-2025 NXP
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

/*${header:end}*/

/*${variable:start}*/
/*${variable:end}*/

/*${function:start}*/
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
    /* Setup Ext(CLKIN) 32Mhz clock */
    CLOCK_SetupExtClocking(kFreq_32MHz);

    /* Setup FRO1Mhz, FRO24Mhz and FRO32Mhz clock */
    CLOCK_Enable1MFRO(true);
    ANACTRL->FRO192M_CTRL |= ANACTRL_FRO192M_CTRL_ENA_24MHZCLK(1);
    ANACTRL->FRO192M_CTRL |= ANACTRL_FRO192M_CTRL_ENA_32MHZCLK(1);

    /* Configure Oscillator 32 kHz Clock */
    POWER_PeripheralPowerOn(kPOWERCFG_FRO32K);
    CLOCK_Select32kOscClkSrc(kCLOCK_Osc32kClockSrc_FRO);
    CLOCK_EnableClock(kCLOCK_Rtc);
    RTC->CTRL &= ~RTC_CTRL_SWRESET_MASK;
    RTC->CTRL |= RTC_CTRL_RTC_EN_MASK;
}
/*${function:end}*/
