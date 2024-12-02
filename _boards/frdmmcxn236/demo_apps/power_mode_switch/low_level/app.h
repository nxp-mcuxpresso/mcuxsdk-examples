/*
 * Copyright 2023 NXP
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

#define APP_POWER_MODE_DESC                                                                                         \
    {                                                                                                               \
        "Acitve: Core clock is 48MHz, power consumption is about 7.8 mA.  ",                                        \
            "Sleep: CPU0 clock is off, System and Bus clock remain ON, power consumption is about 5.85 mA. ",       \
            "Deep Sleep: Core/System/Bus clock are gated off. ",                                                    \
            "Power Down: Core/System/Bus clock are gated off, both CORE_MAIN and CORE_WAKE power domains are put "  \
            "into state retention mode.",                                                                           \
            "Deep Power Down: The whole VDD_CORE voltage domain is power gated. \r\nPlease note that exiting from " \
            "DPD mode will cause wakeup reset."                                                                     \
    }

#define APP_CMC           CMC0
#define APP_RAM_ARRAYS_DS (0x13000200)
#define APP_RAM_ARRAYS_PD (0x13000200)

#define APP_LPTMR             LPTMR0
#define APP_LPTMR_IRQN        LPTMR0_IRQn
#define APP_LPTMR_IRQ_HANDLER LPTMR0_IRQHandler
#define APP_LPTMR_CLK_SOURCE  (16000UL)

#define APP_SPC                           SPC0
#define APP_SPC_LPTMR_LPISO_VALUE         (0x1EU) /* VDD_USB, VDD_P2, VDD_P3, VDD_P4. */
#define APP_SPC_LPTMR_ISO_DOMAINS         "VDD_USB, VDD_P2, VDD_P3, VDD_P4"
#define APP_SPC_WAKEUP_BUTTON_LPISO_VALUE (0x3EU) /* VDD_USB, VDD_P2, VDD_P3, VDD_P4, VBAT. */
#define APP_SPC_WAKEUP_BUTTON_ISO_DOMAINS "VDD_USB, VDD_P2, VDD_P3, VDD_P4, VBAT."
#define APP_SPC_MAIN_POWER_DOMAIN         (kSPC_PowerDomain0)
#define APP_SPC_WAKE_POWER_DOMAIN         (kSPC_PowerDomain1)

#define APP_VBAT             VBAT0
#define APP_VBAT_IRQN        VBAT0_IRQn
#define APP_VBAT_IRQ_HANDLER VBAT0_IRQHandler

#define APP_WUU                    WUU0
#define APP_WUU_WAKEUP_LPTMR_IDX   6U
#define APP_WUU_WAKEUP_BUTTON_IDX  4U /* P0_20, SW2 on EVK board. */
#define APP_WUU_IRQN               WUU_IRQn
#define APP_WUU_IRQ_HANDLER        WUU_IRQHandler
#define APP_WUU_WAKEUP_BUTTON_NAME "SW2"

#define APP_DEBUG_CONSOLE_RX_PORT   PORT1
#define APP_DEBUG_CONSOLE_RX_GPIO   GPIO1
#define APP_DEBUG_CONSOLE_RX_PIN    8U
#define APP_DEBUG_CONSOLE_RX_PINMUX kPORT_MuxAlt2

#define APP_DEBUG_CONSOLE_TX_PORT   PORT1
#define APP_DEBUG_CONSOLE_TX_GPIO   GPIO1
#define APP_DEBUG_CONSOLE_TX_PIN    9U
#define APP_DEBUG_CONSOLE_TX_PINMUX kPORT_MuxAlt2

#define APP_INVALIDATE_CACHE        L1CACHE_InvalidateCodeCache()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void APP_InitDebugConsole(void);
void APP_DeinitDebugConsole(void);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
