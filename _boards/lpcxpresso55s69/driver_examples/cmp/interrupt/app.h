/*
 * Copyright 2018-2019 NXP
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
#define DEMO_USART         USART0
#define DEMO_USART_CLK_SRC kCLOCK_Fro32M

#define DEMO_CMP_P_CHANNEL   1U
#define DEMO_CMP_N_CHANNEL   0U
#define DEMO_CMP_VREF_SOURCE KCMP_VREFSourceVDDA

#define DEMO_GPIO_BASE GPIO
#define DEMO_GPIO_PORT 1U
#define DEMO_GPIO_PIN  7U
#define DEMO_LED_ON    0U
#define DEMO_LED_OFF   1U

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
