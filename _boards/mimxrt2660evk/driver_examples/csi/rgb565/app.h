/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "camera_support.h"
#include "display_support.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_FB0_ADDR 0x22000000U
#define DEMO_FB1_ADDR 0x22028000U
#define DEMO_FB2_ADDR 0x22050000U
#define DEMO_FB3_ADDR 0x22078000U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
