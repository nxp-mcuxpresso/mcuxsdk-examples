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
#define TAMPER_PASSIVE        kTDET_TamperTamperPin0
#define TAMPER_PASSIVE_STATUS kTDET_StatusTamperPinTamper0
#define TAMPER_PASSIVE_EXTPIN kTDET_ExternalTamper0

#define TAMPER_TX             kTDET_TamperTamperPin1 
#define TAMPER_TX_STATUS      kTDET_StatusTamperPinTamper1
#define TAMPER_TX_EXTPIN      kTDET_ExternalTamper1

#define TAMPER_RX             kTDET_TamperTamperPin4
#define TAMPER_RX_STATUS      kTDET_StatusTamperPinTamper4
#define TAMPER_RX_EXTPIN      kTDET_ExternalTamper4
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
