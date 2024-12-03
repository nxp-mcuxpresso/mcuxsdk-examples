/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_gpio.h"
#include "fsl_mailbox.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define CORE0_BOOT_CORE1     0U
#define APP_BOARD_HAS_LED    1U
#define USE_STATIC_DOMAIN_ID 0U
#define USE_MU_NOTIFICATIONS 0U

#define APP_SEMA42   SEMA42_0
#define LED_INIT()   LED_RED_INIT(LOGIC_LED_ON)
#define LED_ON()     LED_RED_ON()
#define LED_OFF()    LED_RED_OFF()
#define LED_TOGGLE() LED_RED_TOGGLE()

#define PRIMARY_CORE_MAILBOX_CPU_ID   kMAILBOX_CM33_Core0
#define SECONDARY_CORE_MAILBOX_CPU_ID kMAILBOX_CM33_Core1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_InitCore1Domain(void);
uint8_t APP_GetCore1DomainID(void);
void APP_InitInterCoreNotifications(void);
uint32_t APP_GetInterCoreNotificationsData(void);
void APP_SetInterCoreNotificationsData(uint32_t data);
/*${prototype:end}*/

#endif /* _APP_H_ */
