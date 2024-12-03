/*
 * Copyright 2023 NXP
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
#define DEMO_TMPSNS_BASE                 TMPSNS1
#define DEMO_TMPSNS_LOW_TEMPERATURE      26.0F
#define DEMO_TMPSNS_LOW_IRQn             TMPSNS1_THR2_IRQn
#define DEMO_TMPSNS_LOW_IRQ_HANDLER_FUNC TMPSNS1_THR2_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
