/*
 * Copyright 2023 NXP
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
#ifndef DEMO_TMU_IMMEDIATE_THRESOLD
#define DEMO_TMU_IMMEDIATE_THRESOLD 0U
#endif
#ifndef DEMO_TMU_INTERVAL_VALUE
#define DEMO_TMU_INTERVAL_VALUE 8U
#endif
#define DEMO_TMU_IRQ              TEMPMON_IRQn
#define DEMO_TMU_IRQ_HANDLER_FUNC TEMPMON_IRQHandler

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
