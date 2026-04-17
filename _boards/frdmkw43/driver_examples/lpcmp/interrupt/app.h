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
#define DEMO_LPCMP_BASE CMP_0
/* VREF_OUT is of no use. This is renamed to VREFH, it can be used as REF voltage for ADC & ACMP if needed. */
#define DEMO_LPCMP_REFERENCE        kLPCMP_VrefSourceVin2
#define DEMO_LPCMP_USER_CHANNEL     0U
#define DEMO_LPCMP_DAC_CHANNEL      7U
#define DEMO_LPCMP_IRQ_ID           CMP0_IRQn
#define LED_INIT()                  LED3_INIT(LOGIC_LED_OFF)
#define LED_ON()                    LED3_ON()
#define LED_OFF()                   LED3_OFF()
#define DEMO_LPCMP_IRQ_HANDLER_FUNC CMP0_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
