/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_BOD_IRQn              BOD_IRQn
#define APP_BOD_IRQHander         BOD_IRQHandler
#define APP_BOD_THRESHOLD_VOLTAGE "2.24V"
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_InitBod(void);
void APP_DeinitBod(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
