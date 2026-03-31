/*
 * Copyright 2025 NXP
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
#define DEMO_LPCMP_BASE                                LPCMP_1
#define DEMO_LPCMP_IRQ_ID                              CMP1_IRQn
#define DEMO_LPCMP_IRQ_HANDLER_FUNC                    CMP1_IRQHandler
#define DEMO_LPCMP_ROUND_ROBIN_FIXED_MUX_PORT          kLPCMP_FixedPlusMuxPort
#define DEMO_LPCMP_ROUND_ROBIN_FIXED_CHANNEL           0U
#define DEMO_LPCMP_ROUND_ROBIN_CHANNELS_CHECKER_MASK   0x7U
#define DEMO_LPCMP_ROUND_ROBIN_CHANNELS_PRE_STATE_MASK 0x2U

/*
 * (LPCMPx_RRCR0[RR_NSAM] / roundrobin clock period) should bigger than CMP propagation delay,
 * roundrobin clock comes from RTC_CLK (32.768KHz), the propagation delay specified in reference manual is 5us.
 * Note that the range of LPCMPx_RRCR0[RR_NSAM] is limited, so the user needs to set the appropriate
 * roundrobin clock period.
 */
#define DEMO_LPCMP_ROUND_ROBIN_SAMPLE_CLOCK_NUMBERS (uint8_t)(5U * 32768 / 1000000UL)

/*
 * (LPCMPx_RRCR0[RR_INITMOD] / roundrobin clock period) should bigger than Initialization time, 
 * roundrobin clock comes from RTC_CLK (32.768KHz), the initialization time specified in Reference Manual is 30us.
 * Note that the range of LPCMPx_RRCR0[RR_INITMOD] is limited, so the user needs to set the appropriate
 * roundrobin clock period.
 */
#define DEMO_LPCMP_ROUND_ROBIN_INIT_DELAY_MODULES (uint8_t)(40U * 32768 / 1000000UL)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
