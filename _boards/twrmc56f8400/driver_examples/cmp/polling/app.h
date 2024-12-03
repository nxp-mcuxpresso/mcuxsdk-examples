/*
 * Copyright 2021 NXP
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
#define LED_INIT() LED_8_INIT(kGPIO_OutputHigh)
#define LED_ON()   LED_8_ON()
#define LED_OFF()  LED_8_OFF()

#define DEMO_CMP_BASE         CMPA
#define DEMO_CMP_USER_CHANNEL kCMP_CMPAInputMux0GPIOA1
#define DEMO_CMP_DAC_CHANNEL  kCMP_CMPAInputMux7InternalDAC
#define DEMO_CLOCK_IP_NAME    kCLOCK_CMPA
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
