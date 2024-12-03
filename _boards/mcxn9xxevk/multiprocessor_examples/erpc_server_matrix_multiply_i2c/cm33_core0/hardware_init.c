/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    // BOARD_InitDebugConsole();
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

    SYSCON->FCCLKSEL[2] = SYSCON_FCCLKSEL_SEL(2);                                  // select FRO 12 MHz clock
    SYSCON->AHBCLKCTRL1 |= SYSCON_AHBCLKCTRL1_FC2_MASK;                            // enable FLEXCOMM1 clock
    SYSCON->FLEXCOMMCLKDIV[2] &= ~SYSCON_FLEXCOMMXCLKDIV_FLEXCOMMCLKDIV_HALT_MASK; // enable fc1 clock, div1
    SYSCON->PRESETCTRLCLR[2] |= 1 << 15;                                           // reset FLEXCOMM1 and release it...
}
/*${function:end}*/
