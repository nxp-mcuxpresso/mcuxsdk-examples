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
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
extern void vPortGptIsr(void);
IRQn_Type vPortGetGptIrqn(void);
GPT_Type *vPortGetGptBase(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
