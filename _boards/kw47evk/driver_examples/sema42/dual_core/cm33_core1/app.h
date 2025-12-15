/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_imu.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define CORE0_BOOT_CORE1     0U
/* To avoid complicated TrustZone configuration for LED access from both cores
   the LED state indication is disabled in this example */
#define APP_BOARD_HAS_LED    0U
#define USE_STATIC_DOMAIN_ID 0U
#define USE_MU_NOTIFICATIONS 0U

#define APP_SEMA42   SEMA42
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
