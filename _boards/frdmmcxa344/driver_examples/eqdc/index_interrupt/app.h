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
#define DEMO_EQDC              EQDC0
#define DEMO_INPUTMUX          INPUTMUX0
#define DEMO_EQDC_INDEX_IRQ_ID EQDC0_INDEX_IRQn
#define ENC_INDEX_IRQHandler   EQDC0_INDEX_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
