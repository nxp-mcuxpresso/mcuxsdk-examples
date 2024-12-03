/*
 * Copyright 2020 NXP
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

/* @TEST_ANCHOR */

#define DEMO_TMU_BASE TMU
/* For real temperature value, bit-8 is sign bit: 1 means nagetive and 0 means positive. */
#ifndef DEMO_TMU_TEST
#define DEMO_TMU_IMMEDIATE_THRESOLD        (1U << 7 | 10U)
#define DEMO_TMU_AVERAGE_THRESOLD          (0U << 7 | 50U)
#define DEMO_TMU_AVERAGE_CRITICAL_THRESOLD (0U << 7 | 88U)
#endif
#define DEMO_TMU_IRQ              ANAMIX_IRQn
#define DEMO_TMU_IRQ_HANDLER_FUNC ANAMIX_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
