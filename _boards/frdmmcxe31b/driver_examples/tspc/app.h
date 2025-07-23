/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_TSPC                 TSPC
#define DEMO_TSPC_GROUP           kTSPC_Group1
#define DEMO_TSPC_PAD1            kTSPC_Group1PTC8
#define DEMO_TSPC_PAD2            kTSPC_Group1PTC9
#define DEMO_TSPC_PAD2_IRQHandler SIUL2_2_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void DEMO_Pad1Init(void);
void DEMO_Pad2Init(void);
void DEMO_DisablePad1OutputBuffer(void);
void DEMO_ClearPad2InterruptStatusFlags(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
