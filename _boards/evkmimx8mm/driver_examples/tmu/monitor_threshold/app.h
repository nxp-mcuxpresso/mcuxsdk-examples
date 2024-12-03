/*
 * Copyright 2018 NXP
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

#define DEMO_TMU_BASE          TMU
#define DEMO_TMU_AMPGAIN_VALUE kTMU_AmplifierGain8_08
#define DEMO_TMU_AMPVREF_VALUE kTMU_AmplifierReferenceVoltage652_5
#ifndef DEMO_TMU_TEST
#define DEMO_TMU_IMMEDIATE_THRESOLD        (48U - 21U)
#define DEMO_TMU_AVERAGE_THRESOLD          (50U - 21U)
#define DEMO_TMU_AVERAGE_CRITICAL_THRESOLD (88U - 21U)
#endif
#define DEMO_TMU_IRQ              TEMPMON_LOW_IRQn
#define DEMO_TMU_IRQ_HANDLER_FUNC TEMPMON_LOW_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
