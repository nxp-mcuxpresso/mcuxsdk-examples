/*
 * Copyright 2020 NXP
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
#define DEMO_XBARA_USER_CHANNEL_INPUT  kXBARA_InputTa1Out
#define DEMO_XBARA_USER_CHANNEL_OUTPUT kXBARA_OutputDmaReq0
#define DEMO_XBARA_BASEADDR            XBARA
#define DEMO_QTMR_BASEADDR             TMRA
#define DEMO_XBARA_IRQn                XBARA_IRQn
#define DEMO_XBARA_IRQHandler          ivINT_XBARA
#define DEMO_XBARA_PRIO                1

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
