/*
 * Copyright  2019 NXP
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
#define DEMO_ACOMP                  ACOMP
#define DEMO_ACOMP_POSITIVE_INPUT   0U /* Voltage ladder output. */
#define DEMO_ACOMP_NEGATIVE_INPUT   3U /* ACMP_I4. */
#define DEMO_ACOMP_IRQ_NUMBER       CMP_CAPT_IRQn
#define DEMO_ACOMP_IRQ_HANDLER_FUNC CMP_CAPT_IRQHandler
#define BOARD_LED_PORT              1U
#define BOARD_LED_PIN               2U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
