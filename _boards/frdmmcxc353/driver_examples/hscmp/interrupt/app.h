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
#define DEMO_HSCMP_BASE             CMP0
#define DEMO_HSCMP_USER_CHANNEL     0U      /* CMP0_INP0 */
#define DEMO_HSCMP_DAC_CHANNEL      4U      /* DAC0_OUT */
#define DEMO_HSCMP_FUNC_CLK_SEL     kHSCMP_FuncClockSource3
#define DEMO_HSCMP_IRQ_ID           CMP0_IRQn
#define DEMO_HSCMP_IRQ_HANDLER_FUNC CMP0_IRQHandler
#define LED_INIT()                  LED_RED_INIT(LOGIC_LED_ON)
#define LED_ON()                    LED_RED_OFF()
#define LED_OFF()                   LED_RED_ON()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
