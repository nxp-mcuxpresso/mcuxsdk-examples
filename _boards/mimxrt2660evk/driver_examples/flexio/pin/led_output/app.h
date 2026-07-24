/*
 * Copyright 2024, 2026 NXP
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
#define BOARD_LED_FLEXIO     HSP__FLEXIO_1
#define BOARD_LED_FLEXIO_PIN 0U
#else
#define BOARD_LED_FLEXIO     HSP__FLEXIO_0
#define BOARD_LED_FLEXIO_PIN 26U
#endif
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
