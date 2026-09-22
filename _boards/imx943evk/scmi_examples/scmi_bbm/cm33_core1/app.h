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
/* BBM sub-tests exercised on this board. Set a switch to 0 to skip the
 * matching group, for example on an emulation platform that has no physical
 * ON/OFF button.
 */
/* This board is real silicon with a physical ON/OFF button and an RTC that
 * runs at real speed, so every group is valid here. The button and GPR groups
 * are still off, because neither decides anything on its own in an unattended
 * run: the button reports whatever state it happens to be in unless somebody
 * presses it, and no System Manager configuration grants a GPR to this logical
 * machine yet. Turn either on when doing that specific check by hand.
 */
#define DEMO_BBM_TEST_RTC        1
#define DEMO_BBM_TEST_RTC_TIME   1
#define DEMO_BBM_TEST_RTC_ALARM  1
#define DEMO_BBM_TEST_BUTTON     0
#define DEMO_BBM_TEST_GPR        0

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
