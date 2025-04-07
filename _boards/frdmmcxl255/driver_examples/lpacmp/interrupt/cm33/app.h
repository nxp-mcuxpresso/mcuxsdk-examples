/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_
 
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_LPACMP_BASE                AON__LPACMP
#define DEMO_LPACMP_IRQ_HANDLER_FUNC    LPCMP_IRQHandler
#define DEMO_LPACMP_IRQ                 LPCMP_IRQn
#define DEMO_LPACMP_INTERVAL_TIME       0U
#define DEMO_LPACMP_DELAY               80U     /* 10MHz CMP CLOCK, 8us delay */
#define DEMO_LPACMP_CHANNEL             0U
#define DEMO_LPACMP_POSITIVE_PORT       kLPACMP_Positive_Input_In4
#define DEMO_LPACMP_NEGATIVE_PORT       kLPACMP_Negative_Input_5Avdd_Divide_15 /* 1.1v */
#define DEMO_LPACMP_MODE                kLPACMP_Continuous_Mode
#define DEMO_SYSTEM_CLOCK               SystemCoreClock
/*${macro:end}*/
 
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/
 
#endif /* _APP_H_ */
 