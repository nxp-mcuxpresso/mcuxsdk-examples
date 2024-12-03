/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define CORE_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
/* Using PUF_ALIAS_0 in this example */
#ifdef PUF_ALIAS_0
#define PUF PUF_ALIAS_0
#endif
/* Activation Code FFR offset */
#define APP_KEYSTORE_OFFSET (0x3e600)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
