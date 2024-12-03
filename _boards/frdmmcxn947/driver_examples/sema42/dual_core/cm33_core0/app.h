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
#include "fsl_mailbox.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define CORE0_BOOT_CORE1              1U
#define APP_BOARD_HAS_LED             1U
#define CORE0_BOOT_CORE1_SPECIFIC_WAY 1U
#define USE_STATIC_DOMAIN_ID          0U
#define USE_MU_NOTIFICATIONS          0U

#define APP_SEMA42 SEMA42_0
/* To avoid complicated TrustZone configuration for LED access from both cores
   the LED_INIT is implemented the way that after the LED is switched on the access fron NonSecure domain is allowed
   and thus the secondary core / NonSecure domain can access the LED (access from primary core / Secure domain is not
   allowed anymore) */
#define LED_INIT()              \
    LED_RED_INIT(LOGIC_LED_ON); \
    GPIO_EnablePinControlNonSecure(BOARD_LED_RED_GPIO, (1 << BOARD_LED_RED_GPIO_PIN))
#define LED_ON()     LED_RED_ON()
#define LED_OFF()    LED_RED_OFF()
#define LED_TOGGLE() LED_RED_TOGGLE()

#define PRIMARY_CORE_MAILBOX_CPU_ID   kMAILBOX_CM33_Core0
#define SECONDARY_CORE_MAILBOX_CPU_ID kMAILBOX_CM33_Core1

/* Address of RAM, where the image for core1 should be copied */
#define CORE1_BOOT_ADDRESS 0x2004E000

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t Image$$CORE1_REGION$$Base;
extern uint32_t Image$$CORE1_REGION$$Length;
#define CORE1_IMAGE_START &Image$$CORE1_REGION$$Base
#elif defined(__ICCARM__)
extern unsigned char core1_image_start[];
#define CORE1_IMAGE_START core1_image_start
#elif (defined(__GNUC__)) && (!defined(__MCUXPRESSO))
extern const char core1_image_start[];
extern const char *core1_image_end;
extern uint32_t core1_image_size;
#define CORE1_IMAGE_START ((void *)core1_image_start)
#define CORE1_IMAGE_SIZE  ((void *)core1_image_size)
#endif
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void);
#endif

void APP_InitDomain(void);
void APP_DeinitDomain(void);
uint8_t APP_GetCore0DomainID(void);
void APP_InitInterCoreNotifications(void);
uint32_t APP_GetInterCoreNotificationsData(void);
void APP_SetInterCoreNotificationsData(uint32_t data);
/*${prototype:end}*/

#endif /* _APP_H_ */
