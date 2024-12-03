/*
 * Copyright 2017 NXP
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
#define DEMO_LPADC_BASE             LPADC0
#define DEMO_LPADC_IRQn             LPADC0_IRQn
#define DEMO_LPADC_IRQ_HANDLER_FUNC LPADC0_IRQHandler
#define DEMO_LPADC_USER_CHANNEL     6U
#define DEMO_LPADC_USER_CMDID       1U /* CMD1 */
                                       /*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
