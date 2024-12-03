/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_ACMP_BASEADDR CMP0

#define DEMO_ACMP_IRQ_ID           CMP0_IRQn
#define DEMO_ACMP_IRQ_HANDLER_FUNC CMP0_IRQHandler

/* Select which channels is used to do round robin checker.
 * Example sets positive port as fixed port with DAC output as comparison reference. So Pre-state mask bit high
 * represents the pre-state of corresponding channel's input voltage is higher than DAC output voltage. If the round
 * robin check result shows that corresponding channel's actual input voltage is lower than DAC output voltage, wakeup
 * event will be generated. The case of pre-state mask bit low is contrary to the case of pre-state mask bit high.
 */
#define DEMO_ACMP_ROUND_ROBIN_FIXED_CHANNEL           0U
#define DEMO_ACMP_ROUND_ROBIN_CHANNELS_CHECKER_MASK   0x01U /* Left-most bit is for channel 7. */
#define DEMO_ACMP_ROUND_ROBIN_CHANNELS_PRE_STATE_MASK 0x01U /* Left-most bit is for channel 7. */
#define DEMO_ACMP_ROUND_ROBIN_PERIOD_MILLISECONDS     1000U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitAcmpRoundRobinTrigger(uint32_t triggerPeroid_Ms);
void BOARD_ClearAcmpRoundRobinTrigger(void);
void BOARD_EnterStopMode(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
