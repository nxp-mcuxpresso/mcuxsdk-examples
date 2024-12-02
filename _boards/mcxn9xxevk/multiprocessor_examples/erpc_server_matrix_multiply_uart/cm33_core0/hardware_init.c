/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void) // TODO to be updated
{
    SystemCoreClockUpdate();

    /*
      000b - No clock.
      001b - PLL divided clock.
      010b - FRO 12 MHz clock.
      011b - FRO_HF_DIV clock.
      100b - CLK_1MHz clock.
      101b - USB PLL clock.
      110b - LP Oscillator clock.
      111b - No clock.
    */
    SYSCON->FCCLKSEL[4] = SYSCON_FCCLKSEL_SEL(2);                                  // select FRO 12 MHz clock
    SYSCON->AHBCLKCTRL1 |= SYSCON_AHBCLKCTRL1_FC4_MASK;                            // enable FLEXCOMM4 clock
    SYSCON->FLEXCOMMCLKDIV[4] &= ~SYSCON_FLEXCOMMXCLKDIV_FLEXCOMMCLKDIV_HALT_MASK; // enable fc4 clock, div1
    SYSCON->PRESETCTRLCLR[1] |= 1 << 15;                                           // reset FLEXCOMM4 and release it...
}

uint32_t LPUART4_GetFreq()
{
    return 12000000U;
}
/*${function:end}*/
