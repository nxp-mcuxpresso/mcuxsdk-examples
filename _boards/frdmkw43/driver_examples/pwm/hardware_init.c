/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_trgmux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_Flexpwm0);
    CLOCK_EnableClock(kCLOCK_Trgmux0);

    /* Request Core#1 to stall when accessing busy flash */
    SYSCON->AUTHENTICATE = SYSCON_UNLOCK_CODE;
    SYSCON->PWM_SUBCTL |= SYSCON_PWM_SUBCTL_CLK0_EN_MASK;
    SYSCON->PWM_SUBCTL |= SYSCON_PWM_SUBCTL_CLK1_EN_MASK;
    SYSCON->PWM_SUBCTL |= SYSCON_PWM_SUBCTL_CLK2_EN_MASK;
    SYSCON->AUTHENTICATE = 0UL;

    /*
     * Route external logic-level signals from the TRGMUX0 input pins to the
     * FLEXPWM0 fault inputs so that an external high/low level can trigger the
     * PWM fault protection.
     *
     * Signal path (per KW43 RM Ch.43 TRGMUX and Ch.54 FlexPWM):
     *   external pin -> TRGMUX0_INx (package pin, muxed in pin_mux.c)
     *                -> TRGMUX FLEXPWM0 device SELx output
     *                -> FLEXPWM0 FAULTx input
     *
     * The FLEXPWM0 TRGMUX device (kTRGMUX_Trgmux0FlexPwm0) has four trigger
     * inputs (kTRGMUX_TriggerInput0..3) that map directly to FLEXPWM0
     * FAULT0..FAULT3.
     *
     * The example enables all four fault inputs in DEMO_FAULT_MASK, but any
     * FLEXPWM0 fault input whose TRGMUX source is left at its reset default
     * (kTRGMUX_SourceDisabled) is driven low. Because the fault inputs are
     * configured active-high (faultInputActiveLevel = true), those unrouted
     * faults never assert. Therefore only the fault inputs actually wired to
     * an external pin need a TRGMUX source; here FAULT1 and FAULT2 are used:
     *   PTA0 -> TRGMUX0_IN1 -> FLEXPWM0 FAULT1
     *   PTD3 -> TRGMUX0_IN2 -> FLEXPWM0 FAULT2
     */
    (void)TRGMUX_SetTriggerSource(TRGMUX_0, kTRGMUX_Trgmux0FlexPwm0, kTRGMUX_TriggerInput1,
                                  (uint32_t)kTRGMUX_SourceTrgmux0Input1);
    (void)TRGMUX_SetTriggerSource(TRGMUX_0, kTRGMUX_Trgmux0FlexPwm0, kTRGMUX_TriggerInput2,
                                  (uint32_t)kTRGMUX_SourceTrgmux0Input2);
}


/*${function:end}*/
