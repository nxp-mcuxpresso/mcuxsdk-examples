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
#define TEMP_CONSTRIANTS                                                                                           \
    11U, PM_RESC_BUS_SYS_CLK_ON, PM_RESC_CTCM0_ACTIVE, PM_RESC_CTCM1_ACTIVE, PM_RESC_STCM0_ACTIVE,                 \
        PM_RESC_STCM1_ACTIVE, PM_RESC_STCM2_ACTIVE, PM_RESC_STCM3_ACTIVE, PM_RESC_FRO_192M_ON, PM_RESC_FRO_6M_ON,  \
        PM_RESC_MAIN_PD_PERI_OPERATIONAL, PM_RESC_WAKE_PD_PERI_ACTIVE

#define APP_POWER_NAME       {"Sleep", "Deep Sleep", "Power Down", "Deep Power Down"}
#define APP_TARGET_POWER_NUM (4U)

#define APP_SLEEP_CONSTRAINTS                                                                                      \
    8U, PM_RESC_CTCM0_DEEPSLEEP, PM_RESC_CTCM1_DEEPSLEEP, PM_RESC_STCM0_DEEPSLEEP, PM_RESC_STCM1_DEEPSLEEP,        \
        PM_RESC_STCM2_DEEPSLEEP, PM_RESC_STCM3_DEEPSLEEP, PM_RESC_MAIN_PD_PERI_OPERATIONAL,                        \
        PM_RESC_WAKE_PD_PERI_ACTIVE

#define APP_DEEP_SLEEP_CONSTRAINTS                                                                                 \
    8U, PM_RESC_CTCM0_DEEPSLEEP, PM_RESC_CTCM1_DEEPSLEEP, PM_RESC_STCM0_DEEPSLEEP, PM_RESC_STCM1_DEEPSLEEP,        \
        PM_RESC_STCM2_DEEPSLEEP, PM_RESC_STCM3_DEEPSLEEP, PM_RESC_MAIN_PD_PERI_STATE_RETENTION,                    \
        PM_RESC_WAKE_PD_PERI_OPERATIONAL

#define APP_POWER_DOWN_CONSTRAINTS                                                                                 \
    7U, PM_RESC_CTCM0_DEEPSLEEP, PM_RESC_CTCM1_POWEROFF, PM_RESC_STCM0_POWEROFF, PM_RESC_STCM1_POWEROFF,           \
        PM_RESC_STCM2_POWEROFF, PM_RESC_STCM3_POWEROFF, PM_RESC_WAKE_PD_PERI_STATE_RETENTION

#define APP_DEEP_POWER_DOWN_CONSTRAINTS 0U

#define APP_CMC   CMC0
#define APP_SPC   SPC0
#define APP_WUU   WUU0
#define APP_LPTMR LPTMR0

#define APP_LPTMR_CLOCK_FREQ       (32768UL)
#define APP_LPTMR_TICKS_PER_SECOND APP_LPTMR_CLOCK_FREQ

/* Debug console RX pin: PORTC2 MUX: 3 */
#define DEBUG_CONSOLE_RX_PORT   PORTC
#define DEBUG_CONSOLE_RX_GPIO   GPIOC
#define DEBUG_CONSOLE_RX_PIN    2U
#define DEBUG_CONSOLE_RX_PINMUX kPORT_MuxAlt3
/* Debug console TX pin: PORTC3 MUX: 3 */
#define DEBUG_CONSOLE_TX_PORT   PORTC
#define DEBUG_CONSOLE_TX_GPIO   GPIOC
#define DEBUG_CONSOLE_TX_PIN    3U
#define DEBUG_CONSOLE_TX_PINMUX kPORT_MuxAlt3

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
status_t APP_UartControlCallback(pm_event_type_t eventType, uint8_t powerState, void *data);
void APP_InitWakeupSource(void);
void APP_Lptmr0WakeupService(void);
void APP_StartLptmr(uint64_t timeOutTickes);
void APP_StopLptmr(void);
uint32_t APP_GetWakeupTimeout(void);
void APP_RegisterNotify(void);
void APP_SetConstraints(uint8_t powerMode);
void APP_ReleaseConstraints(uint8_t powerMode);
/*${prototype:end}*/

#endif /* _APP_H_ */
