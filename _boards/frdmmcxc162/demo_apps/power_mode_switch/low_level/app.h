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
#define APP_POWER_MODE_NAME                                          \
    {                                                                \
        "Active", "Sleep", "DeepSleep", "PowerDown", "DeepPowerDown" \
    }

#define APP_POWER_MODE_DESC                                                                                     \
    {                                                                                                           \
        "Acitve: Core clock is 72MHz.",                                                                         \
        "Sleep: CPU clock is OFF, System/Bus clock remain ON. Most modules can remain operational.",            \
        "Deep Sleep: CPU/System/Bus clock are all OFF. "                                                        \
                    "Some modules can remain operational with low power asynchronous clock sources.",           \
        "Power Down: CPU/System/Bus clock are all OFF. Most modules in a static state.",                        \
        "Deep Power Down: The whole CORE domain is power gated. Modules in SYSTEM domain can be still alive."   \
    }

#define APP_CMC                                 CMC
#define APP_CMC_IRQN                            CMC_IRQn

#define APP_VBAT                                VBAT0

/* System Power Control (SPC) */
#define APP_SPC                                 SPC0
#define APP_SPC_WAKEUP_TIMER_ISO_VALUE          (0x2U)          /* VDD_USB. */
#define APP_SPC_WAKEUP_TIMER_LPISO_VALUE        (0x2U)          /* VDD_USB. */
#define APP_SPC_WAKEUP_TIMER_ISO_DOMAINS        "VDD_USB"

#define APP_SPC_WAKEUP_BUTTON_ISO_VALUE         (0x2U)          /* VDD_USB. */
#define APP_SPC_WAKEUP_BUTTON_LPISO_VALUE       (0x2U)          /* VDD_USB. */
#define APP_SPC_WAKEUP_BUTTON_ISO_DOMAINS       "VDD_USB."
#define APP_SPC_MAIN_POWER_DOMAIN               (kSPC_PowerDomain0)
#define APP_SPC_LDO_LPWKUP_DELAY                (0x6EU)

/* Wakeup Unit (WUU) */
#define APP_WUU                                 WUU0
#define APP_WUU_IRQN                            WUU0_IRQn
#define APP_WUU_IRQ_HANDLER                     WUU0_IRQHandler
#define APP_WUU_WAKEUP_BUTTON_IDX               25U
#define APP_WUU_WAKEUP_BUTTON_NAME              "SW2"
#define APP_WUU_WAKEUP_TIMER_IDX                6U
#define APP_WUU_WAKEUP_TIMER                    LPTMR0
#define APP_WUU_WAKEUP_TIMER_IRQN               LPTMR0_IRQn
#define APP_WUU_WAKEUP_TIMER_IRQ_HANDLER        LPTMR0_IRQHandler
#define APP_WUU_WAKEUP_TIMER_CLOCK_SOURCE       (16384U)

/* LPUART RX and TX */
#define APP_DEBUG_CONSOLE_RX_PORT               PORT0
#define APP_DEBUG_CONSOLE_RX_PIN                2U
#define APP_DEBUG_CONSOLE_TX_PORT               PORT0
#define APP_DEBUG_CONSOLE_TX_PIN                3U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitHardware(void);

#endif /* _APP_H_ */
