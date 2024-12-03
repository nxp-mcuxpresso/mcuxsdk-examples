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

#define APP_MODE_NAME_ARRAY                                                                                     \
    {                                                                                                           \
        "Active", "Sleep1", "DeepSleep1", "DeepSleep2", "DeepSleep3", "DeepSleep4", \
    }

#define APP_MODE_DESC_ARRAY                                                                                        \
    {                                                                                                              \
        "Active: DCDC in normal mode(voltage 1.5V); core voltage is 1.1V; core clock is 96MHz; ",                  \
        "Sleep1: Core, platform and peripheral clocks are gated; CORE_MAIN, CORE_WAKE and Radio domain enter "     \
        "into sleep mode; DCDC voltage is 1.25V; core voltage is 1.0V;",                                           \
        "Deep Sleep1: Core, platform and peripheral clocks are gated; CORE_MAIN, CORE_WAKE and Radio domain "      \
        "enter into deep sleep mode;",                                                                             \
        "Deep Sleep2: Core, platform and peripheral clocks are gated; CORE_MAIN, CORE_WAKE and Radio domain "      \
        "enter into deep sleep mode; DCDC voltage is 1.25V; core voltages is 1.0V;",                               \
        "Deep Sleep3: Core, platform and peripheral clocks are gated; CORE_MAIN, CORE_WAKE and Radio domain "      \
        "enter into deep sleep mode; DCDC voltage is 1.25V; core voltages is 1.0V;",                               \
        "Deep Sleep4: Core, platform and peripheral clocks are gated; CORE_MAIN, CORE_WAKE and Radio domain "      \
        "enter into deep sleep mode;",                                                                             \
}
#define APP_CMC   CMC0
#define APP_LPTMR DEMO_LPTMR_PERIPHERAL
#define APP_WUU   DEMO_WUU_PERIPHERAL
#define APP_SPC   SPC0
#define APP_VBAT  VBAT0

#define WUU_WAKEUP_LPTMR_IDX  0U
#define WUU_WAKEUP_VBAT_IDX   4U
#define WUU_WAKEUP_BUTTON_IDX 11U

#define WUU_WAKEUP_BUTTON_NAME "SW2"
#define SWITCH_WAKEUP_BUTTON_NAME "SW5"

#define APP_LPTMR_IRQ_HANDLER DEMO_LPTMR_IRQHANDLER
#define APP_VBAT_IRQ_HANDLER  VBAT_IRQHandler

#define APP_LPTMR_LOW_POWER_ISO_DOMAINS         (0x5U)
#define APP_WAKEUP_BUTTON_LOW_POWER_ISO_DOMAINS (0x4U)
#define APP_VBAT_LOW_POWER_ISO_DOMAINS          (0x5U)
      
#define DEMO_NOT_SUPPORT_WAKEUP_BOOT    (1)
      
#define APP_DEEPSLEEP2_SRAM_POWER_MODE (0x0U)
#define APP_DEEPSLEEP3_SRAM_POWER_MODE (0x0U)
#define APP_DEEPSLEEP4_SRAM_POWER_MODE (0x0U)
#define APP_POWERDOWN1_SRAM_POWER_MODE (0x0U)
#define APP_POWERDOWN2_SRAM_POWER_MODE (0x0U)
#define APP_POWERDOWN3_SRAM_POWER_MODE (0x0U)
#define APP_POWERDOWN4_SRAM_POWER_MODE (0x0U)
      
#define DEMO_ASSERT_SW_RESET    
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
void APP_InitDebugConsole(void);
void APP_DeinitDebugConsole(void);
void APP_DeinitWakeupButton(void);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
