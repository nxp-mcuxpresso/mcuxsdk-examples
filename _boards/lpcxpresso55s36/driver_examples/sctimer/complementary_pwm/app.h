/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define SCTIMER_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)
/* Two complementary output pairs (high side / low side). */
#define DEMO_PAIR1_OUT_HIGH kSCTIMER_Out_0 /* PIO1_4  - J10-5  */
#define DEMO_PAIR1_OUT_LOW  kSCTIMER_Out_1 /* PIO1_8  - J10-7  */
#define DEMO_PAIR2_OUT_HIGH kSCTIMER_Out_3 /* PIO1_10 - J92-9  */
#define DEMO_PAIR2_OUT_LOW  kSCTIMER_Out_4 /* PIO1_17 - J10-13 */
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
