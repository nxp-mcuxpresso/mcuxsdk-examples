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
#ifndef APP_ENABLE_ADVC
#define APP_ENABLE_ADVC           (CONFIG_APP_ENABLE_ADVC)
#endif
#ifndef APP_ENABLE_CONTEXT_SAVING
#define APP_ENABLE_CONTEXT_SAVING (CONFIG_APP_ENABLE_CONTEXT_SAVING)
#endif

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
#define APP_LPTMR_BASE     AON__LPTMR0
#define LPTMR_SOURCE_CLOCK (CLOCK_GetFreq(kCLOCK_Fro16k))
#define LPTMR_USEC_COUNT   10000000

#define APP_RTC_BASE AON__RTC_AON

#define APP_EXT_INT_BUTTON "SW5"

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_BootAs3MHzClocks(void);
void BOARD_InitTamper_ButtonPins(void);
void BOARD_DeinitTamper_ButtonPins(void);
void BOARD_InitHardware(void);
void BOARD_DeinitDebugConsole(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
