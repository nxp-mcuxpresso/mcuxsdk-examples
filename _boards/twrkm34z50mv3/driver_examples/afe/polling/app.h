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
#define DEMO_AFE_CHANNEL 0U
/* About 23 mV */
#define DEMO_AFE_VREF_TRIM 2U
#define DEMO_AFE_BASEADDR  AFE
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_VREF_Config(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
