/*
 * Copyright 2026 NXP
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
#define TAMPER_INTERRUPT      TDET_IRQn

#define TAMPER_PASSIVE        kTDET_TamperTamperPin0
#define TAMPER_PASSIVE_STATUS kTDET_StatusTamperPinTamper0
#define TAMPER_PASSIVE_EXTPIN kTDET_ExternalTamper0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
