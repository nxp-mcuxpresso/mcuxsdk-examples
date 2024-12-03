/*
 * Copyright 2024 NXP
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
#define APP_POWER_MODE_NAME                                          \
    {                                                                \
        "Active", "Sleep", "DeepSleep", "PowerDown", "DeepPowerDown" \
    }

#define APP_POWER_MODE_DESC                                                                                           \
    {                                                                                                                 \
        "Acitve: Core clock is 96MHz.", "Sleep: CPU clock is off, and the system clock and bus clock remain ON. ",    \
            "Deep Sleep: Core/System/Bus clock are gated off. ",                                                      \
            "Power Down: Core/System/Bus clock are gated off, CORE_MAIN is in static state, Flash memory is powered " \
            "off.",                                                                                                   \
            "Deep Power Down: The whole VDD_CORE voltage domain is power gated."                                      \
    }

#define APP_CMC CMC

#define APP_SPC                                 SPC0
#define APP_SPC_WAKEUP_TIMER_ISO_VALUE          (0x2U) /* VDD_USB. */
#define APP_SPC_WAKEUP_TIMER_LPISO_VALUE        (0x2U) /* VDD_USB. */
#define APP_SPC_WAKEUP_TIMER_ISO_DOMAINS        "VDD_USB"

#define APP_SPC_WAKEUP_BUTTON_ISO_VALUE         (0x2U) /* VDD_USB. */
#define APP_SPC_WAKEUP_BUTTON_LPISO_VALUE       (0x2U) /* VDD_USB. */
#define APP_SPC_WAKEUP_BUTTON_ISO_DOMAINS       "VDD_USB"
#define APP_SPC_MAIN_POWER_DOMAIN               (kSPC_PowerDomain0)

#define APP_WUU                    WUU0
#define APP_WUU_IRQN               WUU0_IRQn
#define APP_WUU_IRQ_HANDLER        WUU0_IRQHandler
#define APP_WUU_WAKEUP_TIMER_IDX   2U
#define APP_WUU_WAKEUP_BUTTON_IDX  9U
#define APP_WUU_WAKEUP_BUTTON_NAME "SW3"

#define APP_WAKETIMER_IRQN WAKETIMER0_IRQn

/* LPUART RX */
#define APP_DEBUG_CONSOLE_RX_PORT   PORT0
#define APP_DEBUG_CONSOLE_RX_PIN    2U
/* LPUART TX */
#define APP_DEBUG_CONSOLE_TX_PORT   PORT0
#define APP_DEBUG_CONSOLE_TX_PIN    3U

#define APP_ACTIVE_CFG_LDO_VOLTAGE_LEVEL kSPC_CoreLDO_NormalVoltage
#define APP_LDO_LPWKUP_DELAY             (0x5BU)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_InitDebugConsole(void);
void APP_DeinitDebugConsole(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
