/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_gpio.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define ERPC_TRANSPORT_RPMSG_LITE_LINK_ID (RL_PLATFORM_KW47B42_LINK_ID)

/* Address where the image for core1 starts */
#define CORE1_BOOT_ADDRESS 0x48800000

extern char rpmsg_lite_base[];

/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);

/*${prototype:end}*/

#endif /* _APP_H_ */
