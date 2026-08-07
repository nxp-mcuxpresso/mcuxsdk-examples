/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "clock_config.h"
#include "fsl_clock.h"

/*
 * BOARD_InitBootClocks for the mimxrt2660evk motor control example.
 *
 * Delegates to the shared board clock configuration (BOARD_BootClockHPRUN)
 * which sets CM85 = 1000 MHz, MAIN/CMPT bus = 333 MHz.
 * The motor control peripherals (PWM1, QTimer1, EQDC1, ADC1/2) all derive
 * their functional clocks from the MAIN HSP bus at ~333 MHz.
 */
void BOARD_InitBootClocks(void)
{
    extern void BOARD_BootClockHPRUN(void);
    BOARD_BootClockHPRUN();
}
