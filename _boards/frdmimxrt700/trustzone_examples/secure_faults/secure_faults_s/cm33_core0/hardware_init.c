/*
 * Copyright 2026 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Disable AHBSC0 input for CHIP_RESET */
    ITRC->OUT_SEL_1[4][0] = (ITRC->OUT_SEL_1[4][0] & (~ITRC_OUT_SEL_1_IN21_SELn_MASK)) | 0x800U;
    
    /* Disable BusFault handler to use HardFault Handler. */
    SCB->SHCSR &= ~ SCB_SHCSR_BUSFAULTENA_Msk;
}
/*${function:end}*/
