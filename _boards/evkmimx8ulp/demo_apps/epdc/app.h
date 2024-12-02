/*
 * Copyright 2021,2022 NXP
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
#define PANEL_HEIGHT         758U
#define PANEL_WIDTH          1024U
#define DEMO_WB_ADDR         0x80000000 /* size: height x width x 2bytes/pixel */
#define DEMO_Y4C_ADDR        0x80600000 /* size: height x width x 1bytes/pixel */
#define DEMO_UPD_ADDR        0x80400000 /* size: height x width x 1bytes/pixel */
#define DEMO_WV_ADDR         0x80200000
#define DEMO_EPDC_IRQHandler EPDC_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
