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
#define DEMO_ACMP_BASEADDR         WAKE__ACMP_0
#define DEMO_ACMP_USER_CHANNEL     1U  /* INA0: PIO3_4 (J94-17) */
#define DEMO_ACMP_MINUS_INPUT      DEMO_ACMP_USER_CHANNEL
#define DEMO_ACMP_PLUS_INPUT       7U  /* Internal 8-bit DAC output (placeholder) */
#define DEMO_ACMP_IRQ_ID           WAKE_ACMP0_IRQn
#define DEMO_ACMP_IRQ_HANDLER_FUNC WAKE_ACMP0_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
