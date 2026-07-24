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

#define BOARD_INPUT_PIN_FLEXIO     HSP__FLEXIO_1
#define BOARD_GPIO_OUTPUT_PORT     HSP__GPIO_1
#define BOARD_INPUT_PIN_FLEXIO_PIN 0U
#define FLEXIO_PIN_UserCallback    HSP_FLEXIO1_IRQHandler

#if defined(RT2660_PRESILICON_DEVELOPMENT) && (RT2660_PRESILICON_DEVELOPMENT == 1)
#define BOARD_GPIO_OUTPUT_PORT_PIN 5U
#else
#define BOARD_GPIO_OUTPUT_PORT_PIN 1U
#endif

#define BOARD_OUTPUT_USE_GPIO

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
