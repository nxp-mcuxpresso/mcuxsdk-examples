/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_EMIOS               EMIOS_0
#define EXAMPLE_COUNTERBUS_CHANNEL  (23U)
#define EXAMPLE_PWM_CHANNEL_0       (3U)

/* Get frequency of eMIOS clock */
#define EXAMPLE_EMIOS_CLK_FREQ  (CLOCK_GetFreq(kCLOCK_EmiosClk))
#define EXAMPLE_COUNTERBUS_FREQ (10000000U)     /* Counter Bus frequency 10MHz */
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
