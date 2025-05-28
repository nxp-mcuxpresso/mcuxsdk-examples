/*
 * Copyright 2025 NXP
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
#define APP_MU         MUB
#define APP_MU_CHANNEL (0)

#define APP_POWER_MODE_NAME \
    {"Sleep",               \
     "Deep Sleep",          \
     "Power Down1",         \
     "Power Down2",         \
     "Deep Power Down1",    \
     "Deep Power Down2",    \
     "Deep Power Down3",    \
     "Shut Down",           \
     "Active"}
#define APP_LPTMR_BASE       AON__LPTMR0
#define LPTMR_SOURCE_CLOCK   (CLOCK_GetFreq(kCLOCK_Fro16k))
#define LPTMR_USEC_COUNT     10000000
#define APP_ENABLE_DEBUG_LOG (1)

#define APP_TAMPER_BUTTON_NAME "SW6"
#define APP_TAMPER_BUTTON_IRQ  GPIO00_AON_IRQn
#define APP_TAMPER_BUTTON_GPIO AON__GPIO0
#define APP_TAMPER_BUTTON_PORT AON__PORT0
#define APP_TAMPER_BUTTON_PIN  4
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitTamper_ButtonPins(void);
void BOARD_DeinitTamper_ButtonPins(void);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
