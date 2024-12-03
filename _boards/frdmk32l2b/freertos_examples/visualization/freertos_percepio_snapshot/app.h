/*
 * Copyright 2019 NXP
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

#define APP_SYSVIEW_APP_NAME    "FRDMK32L2B System Viewer Demo"
#define APP_SYSVIEW_DEVICE_NAME "Cortex-M0Plus"
#define APP_SYSVIEW_RAM_BASE    (0x1FFFE000)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
