/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define CPU_NAME "iMXRT1152"

#define APP_WAKEUP_BUTTON_GPIO        BOARD_USER_BUTTON_GPIO
#define APP_WAKEUP_BUTTON_GPIO_PIN    BOARD_USER_BUTTON_GPIO_PIN
#define APP_WAKEUP_BUTTON_IRQ         BOARD_USER_BUTTON_IRQ
#define APP_WAKEUP_BUTTON_IRQ_HANDLER BOARD_USER_BUTTON_IRQ_HANDLER
#define APP_WAKEUP_BUTTON_NAME        BOARD_USER_BUTTON_NAME

#define APP_WAKEUP_SNVS_IRQ         SNVS_HP_NON_TZ_IRQn
#define APP_WAKEUP_SNVS_IRQ_HANDLER SNVS_HP_NON_TZ_IRQHandler

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t __Vectors[];
#define IMAGE_ENTRY_ADDRESS ((uint32_t)__Vectors)
#elif defined(__MCUXPRESSO)
extern uint32_t __Vectors[];
#define IMAGE_ENTRY_ADDRESS ((uint32_t)__Vectors)
#elif defined(__ICCARM__)
extern uint32_t __VECTOR_TABLE[];
#define IMAGE_ENTRY_ADDRESS ((uint32_t)__VECTOR_TABLE)
#elif defined(__GNUC__)
extern uint32_t __VECTOR_TABLE[];
#define IMAGE_ENTRY_ADDRESS ((uint32_t)__VECTOR_TABLE)
#endif
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
