/*
 * Copyright 2025-2026 NXP
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
#if defined(RT2660_PRESILICON_DEVELOPMENT) && (RT2660_PRESILICON_DEVELOPMENT == 1)
#define BOARD_LED_GPIO     BOARD_LED_BLUE_GPIO
#define BOARD_LED_GPIO_PIN BOARD_LED_BLUE_GPIO_PIN
#else
#define BOARD_LED_GPIO       BOARD_USER_LED_GPIO
#define BOARD_LED_GPIO_PIN   BOARD_USER_LED_GPIO_PIN
#endif
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
