/*
 * Copyright 2018 NXP
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
#define DEMO_ACTIVE_IN_DEEPSLEEP               (kPDSLEEPCFG_DeepSleepBODActive | kPDSLEEPCFG_DeepSleepWDTOscActive)
#define DEMO_USER_WAKEUP_KEY_GPIO              GPIO
#define DEMO_USER_WAKEUP_KEY_PORT              BOARD_SW2_GPIO_PORT
#define DEMO_USER_WAKEUP_KEY_PIN               BOARD_SW2_GPIO_PIN
#define DEMO_SYSCON_PIN_INT_SEL                (4U)
#define DEMO_SYSCON_STARTER0_MASK              (1U)
#define DEMO_SYSCON_STARTER1_MASK              (1U << 15U)
#define DEMO_WKT_TIMEOUT_VALUE                 (250000U * 12U)
#define DEMO_PINT_PIN_INT0_SRC                 kSYSCON_GpioPort0Pin4ToPintsel
#define DEMO_WKT_CLK_FREQ                      (10000U)
#define DEMO_SLEEP_WAKEUP_SOURCE               "\t1. Wkt timer\r\n\t2. SW2, wakeup key\r\n\t3. SW3, reset key\r\n"
#define DEMO_DEEP_SLEEP_WAKEUP_SOURCE          "\t1. Wkt timer\r\n\t2. SW2, wakeup key\r\n\t3. SW3, reset key\r\n"
#define DEMO_POWER_DOWN_WAKEUP_SOURCE          "\t1. Wkt timer\r\n\t2. SW2, wakeup key\r\n\t3. SW3, reset key\r\n"
#define DEMO_DEEP_POWERDOWN_WAKEUP_SOURCE      "\t1. Wkt timer\r\n\t2. SW2, wakeup key\r\n\t3. SW3, reset key\r\n"
#define DEMO_SLEEP_WAKEUP_SOURCE_SIZE          (3U)
#define DEMO_DEEP_SLEEP_WAKEUP_SOURCE_SIZE     (3U)
#define DEMO_POWER_DOWN_WAKEUP_SOURCE_SIZE     (3U)
#define DEMO_DEEP_POWERDOWN_WAKEUP_SOURCE_SIZE (3U)
#define DEMO_WAKEUP_CASE_WKT                   (0U)
#define DEMO_WAKEUP_CASE_WAKEUP                (1U)
#define DEMO_WAKEUP_CASE_RESET                 (2U)
#define DEMO_DEEP_POWERDOWN_RESET_ENABLE       (1U)
#define POWER_DPD_ENABLE_WAKEUP_PIN            POWER_EnableWakeupPinForDeepPowerDown(true, true);
#define POWER_DPD_ENABLE_RESET_PIN             POWER_EnableResetPinForDeepPowerDown(true, true);
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void DEMO_PreEnterLowPower(void);
void DEMO_LowPowerWakeup(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
