/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

#include "fsl_clock.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_LPCMP_BASE                                CMP0
#define DEMO_LPCMP_IRQ_ID                              CMP0_IRQn
#define DEMO_LPCMP_IRQ_HANDLER_FUNC                    CMP0_IRQHandler
#define DEMO_LPCMP_ROUND_ROBIN_FIXED_CHANNEL           7U
#define DEMO_LPCMP_ROUND_ROBIN_CHANNELS_CHECKER_MASK   0x07U
#define DEMO_LPCMP_ROUND_ROBIN_CHANNELS_PRE_STATE_MASK 0x05U
#define DEMO_LPCMP_ROUND_ROBIN_FIXED_MUX_PORT          kLPCMP_FixedPlusMuxPort

/* (LPCMPx_RRCR0[RR_NSAM] / roundrobin clock period) should bigger than CMP propagation delay, roundrobin clock period
 * can be obtained by the CLOCK_GetCmpRRClkFreq function, the propagation delay specified in reference manual is 5us.
 * Note that the range of LPCMPx_RRCR0[RR_NSAM] is limited, so the user needs to set the appropriate roundrobin clock
 * period.
 */
#define DEMO_LPCMP_ROUND_ROBIN_SAMPLE_CLOCK_NUMBERS (uint8_t)(5U * CLOCK_GetCmpRRClkFreq(0U) / 1000000UL)

/* (LPCMPx_RRCR0[RR_INITMOD] / roundrobin clock period) should bigger than initialization time, roundrobin clock period
 * can be obtained by the CLOCK_GetCmpRRClkFreq function, the initialization time specified in reference manual is 40us.
 * Note that the range of LPCMPx_RRCR0[RR_INITMOD] is limited, so the user needs to set the appropriate roundrobin clock
 * period.
 */
#define DEMO_LPCMP_ROUND_ROBIN_INIT_DELAY_MODULES (uint8_t)(40U * CLOCK_GetCmpRRClkFreq(0U) / 1000000UL)

/*
 * The roundrobin internal trigger signal generates rate is(LPCMPx_RRCR2[RR_TIMER_RELOAD] + 1) * roundrobin clock
 * period, roundrobin clock period can be obtained by the CLOCK_GetCmpRRClkFreq function, set the internal trigger
 * signal generates rate to 1s, LPCMPx_RRCR2[RR_TIMER_RELOAD] is (roundrobin clock period - 1).
 */
#define DEMO_LPCMP_ROUND_ROBIN_INTERAL_TIMER_RATE (CLOCK_GetCmpRRClkFreq(0U) - 1U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
