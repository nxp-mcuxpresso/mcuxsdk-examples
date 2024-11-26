/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_TMPSNS_BASE                 TMPSNS
#define DEMO_TMPSNS_LOW_TEMPERATURE      26.0F
#define DEMO_TMPSNS_LOW_IRQn             TMPSNS_THR2_IRQn
#define DEMO_TMPSNS_LOW_IRQ_HANDLER_FUNC TMPSNS_THR2_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
