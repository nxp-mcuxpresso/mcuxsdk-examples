/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_port.h"
#include "fsl_vbat.h"
#include "fsl_debug_console.h"

void BOARD_InitHardware(void)
{
    BOARD_BootClockFROHF48M();
    BOARD_InitBUTTONsPins();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();
    
    /*! Bring FRO16k to a clean status  */
    VBAT0->FROLCKA &= ~VBAT_FROLCKA_LOCK_MASK; /*!< Unlock FRO16k  */
    VBAT_EnableFRO16k(APP_VBAT, false); /*!< Disable RFO16k */
    VBAT_GateFRO16k(APP_VBAT, (kVBAT_EnableClockToDomain0 | kVBAT_EnableClockToDomain1 |
                               kVBAT_EnableClockToDomain2));
                 
    CLOCK_SetClockDiv(kCLOCK_DivLPTMR0, 1U);
    CLOCK_AttachClk(kFRO_LF_DIV_to_LPTMR0);    
}
