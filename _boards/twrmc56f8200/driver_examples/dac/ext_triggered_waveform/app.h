/*
 * Copyright 2021 NXP
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
#define DEMO_CLOCK_IP_NAME      kCLOCK_DACA
#define DEMO_DAC_BASE           DACA
#define DEMO_DAC_ENABLE_ONESHOT false
#define DEMO_XBARA_BASE         XBARA
#define DEMO_XBARA_XB_CHANNEL   kXBARA_InputXbIn2
#define DEMO_XBARA_DAC_CHANNEL  kXBARA_OutputDaca12bSync

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
