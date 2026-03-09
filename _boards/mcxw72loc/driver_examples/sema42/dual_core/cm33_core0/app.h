/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_imu.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define CORE0_BOOT_CORE1              1U
/* To avoid complicated TrustZone configuration for LED access from both cores
   the LED state indication is disabled in this example */
#define APP_BOARD_HAS_LED             0U
#define CORE0_BOOT_CORE1_SPECIFIC_WAY 1U
#define USE_STATIC_DOMAIN_ID          0U
#define USE_MU_NOTIFICATIONS          0U

#define APP_SEMA42 SEMA42

/* Address where the image for core1 starts */
#define CORE1_BOOT_ADDRESS 0x48800000

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);

void APP_InitDomain(void);
void APP_DeinitDomain(void);
uint8_t APP_GetCore0DomainID(void);
void APP_InitInterCoreNotifications(void);
uint32_t APP_GetInterCoreNotificationsData(void);
void APP_SetInterCoreNotificationsData(uint32_t data);
void APP_BootCore1(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
