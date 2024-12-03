/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    SYSCON->CLOCK_CTRL |= SYSCON_CLOCK_CTRL_FRO1MHZ_CLK_ENA_MASK;
    ANACTRL->XO32M_CTRL |= ANACTRL_XO32M_CTRL_ENABLE_SYSTEM_CLK_OUT_MASK;

    /*!< Configure RTC OSC */
    POWER_DisablePD(kPDRUNCFG_PD_XTAL32K); /*!< Powered the XTAL 32 kHz RTC oscillator */
    POWER_EnablePD(kPDRUNCFG_PD_FRO32K);   /*!< Powered down the FRO 32 kHz RTC oscillator */
    CLOCK_AttachClk(kXTAL32K_to_OSC32K);   /*!< Switch OSC32K to XTAL32K */
    CLOCK_EnableClock(kCLOCK_Rtc);         /*!< Enable the RTC peripheral clock */
    RTC->CTRL &= ~RTC_CTRL_SWRESET_MASK;   /*!< Make sure the reset bit is cleared */

    /*!< Set up PLL1 */
    CLOCK_AttachClk(kEXT_CLK_to_PLL1);  /*!< Switch PLL1CLKSEL to EXT_CLK */
    POWER_DisablePD(kPDRUNCFG_PD_PLL1); /* Ensure PLL is on  */
    const pll_setup_t pll1Setup = {
        .pllctrl = SYSCON_PLL1CTRL_CLKEN_MASK | SYSCON_PLL1CTRL_SELI(53U) | SYSCON_PLL1CTRL_SELP(31U),
        .pllndec = SYSCON_PLL1NDEC_NDIV(8U),
        .pllpdec = SYSCON_PLL1PDEC_PDIV(1U),
        .pllmdec = SYSCON_PLL1MDEC_MDIV(150U),
        .pllRate = 150000000U,
        .flags   = PLL_SETUPFLAG_WAITLOCK};
    CLOCK_SetPLL1Freq(&pll1Setup); /*!< Configure PLL1 to the desired values */
}
/*${function:end}*/
