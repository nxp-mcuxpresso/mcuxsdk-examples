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
        "Active", "Sleep1", "DeepSleep1", "DeepSleep2", "DeepSleep3", "DeepSleep4", "PowerDown1", "PowerDown2", \
            "PowerDown3", "PowerDown4", "DeepPowerDown1", "DeepPowerDown2", "PowerSwitchOff"                    \
    }

#define APP_MODE_DESC_ARRAY                                                                                            \
    {                                                                                                                  \
        "Active: DCDC in normal mode(voltage 1.5V); core voltage is 1.1V; core clock is 96MHz; power consumption is "  \
        "about 7.5 mA",                                                                                                \
            "Sleep1: Core, platform and peripheral clocks are gated; CORE_MAIN, CORE_WAKE and Radio domain enter "     \
            "into sleep mode; DCDC voltage is 1.25V; core voltage is 1.0V; all CM33 RAM retained; power consumption "  \
            "is about 53.5uA.",                                                                                        \
            "Deep Sleep1: Core, platform and peripheral clocks are gated; CORE_MAIN, CORE_WAKE and Radio domain "      \
            "enter into deep sleep mode; DCDC voltage is 1.25V; core voltages is 1.0V; all RAMs retained; OSC32K "     \
            "enabled; power consumption is about 2.8uA.",                                                              \
            "Deep Sleep2: Core, platform and peripheral clocks are gated; CORE_MAIN, CORE_WAKE and Radio domain "      \
            "enter into deep sleep mode; DCDC voltage is 1.25V; core voltages is 1.0V; 48KB of CM33 RAM retained; "    \
            "all radio RAMs retained; OSC32K enabled; power consumption is about 2.4uA. \r\nPlease note that core "    \
            "will fail to back to current context due to some used CM33 RAMs are powered off, will assert the "        \
            "software reset after waking up!",                                                                         \
            "Deep Sleep3: Core, platform and peripheral clocks are gated; CORE_MAIN, CORE_WAKE and Radio domain "      \
            "enter into deep sleep mode; DCDC voltage is 1.25V; core voltages is 1.0V; 16KB of CM33 RAM retained; "    \
            "all radio RAMs retained; OSC32K enabled; power consumption is about 1.8uA. \r\nPlease note that core "    \
            "will fail to back to current context due to some used CM33 RAMs are powered off, will assert the "        \
            "software reset after waking up!",                                                                         \
            "Deep Sleep4: Core, platform and peripheral clocks are gated; CORE_MAIN, CORE_WAKE and Radio domain "      \
            "enter into deep sleep mode; DCDC voltage is 1.25V; core voltages is 1.0V; 8KB of CM33 RAM retained; all " \
            "radio RAMs retained; OSC32K enabled; power consumption is about 1.8uA.\r\nPlease note that core will "    \
            "fail to back to current context due to all CM33 RAMs are powered off, will assert the software reset "    \
            "after waking up!",                                                                                        \
            "Power Down1: Core, platform and peripheral clocks are gated; CORE_MAIN, CORE_WAKE and Radio domain "      \
            "enter into power down mode; DCDC voltage is 1.25V; core voltages is 1.0V; 32KB of CM33 RAM retained; "    \
            "all radio RAMs retained; OSC32K enabled; power consumption is about 1.8uA.",                              \
            "Power Down2: Core, platform and peripheral clocks are gated; CORE_MAIN, CORE_WAKE and Radio domain "      \
            "enter into power down mode; DCDC voltage is 1.25V; core voltages is 1.0V; 16KB of CM33 RAM retained; "    \
            "all radio RAMs retained; OSC32K enabled; power consumption is about 1.8uA.",                              \
            "Power Down3: Core, platform and peripheral clocks are gated; CORE_MAIN, CORE_WAKE and Radio domain "      \
            "enter into power down mode; DCDC voltage is 1.25V; core voltages is 1.0V; 16KB of CM33 RAM retained; no " \
            "radio RAM retained; OSC32K enabled; power consumption is about 1.8uA.",                                   \
            "Power Down4: Core, platform and peripheral clocks are gated; CORE_MAIN, CORE_WAKE and Radio domain "      \
            "enter into power down mode; DCDC voltage is 1.25V; core voltages is 1.0V; 8KB of CM33 RAM retained; no "  \
            "radio RAM retained; OSC32K enabled; power consumption is about 1.8uA.",                                   \
            "Deep Power Down1: Core, platform and peripheral clocks are gated; CORE_MAIN, CORE_WAKE and Radio domain " \
            "enter into deep power down mode; DCDC voltage is 1.25V; core voltages is 1.0V; 8KB of CM33 RAM "          \
            "retained; no radio RAM retained; OSC32K enabled; power consumption is about 1.1uA.",                      \
            "Deep Power Down2: Core, platform and peripheral clocks are gated; CORE_MAIN, CORE_WAKE and Radio domain " \
            "enter into deep power down mode; DCDC voltage is 1.25V; core voltages is 1.0V; 8KB of CM33 RAM "          \
            "retained; no radio RAM retained; OSC32K enabled; power consumption is about 0.9uA. \r\nPlease note that " \
            "core will fail to back to current context due to all CM33 RAMs are powered off!",                         \
            "Switch off: All power domain except VBAT power domain are powered off; power consumption is about 0.2 "   \
            "uA."                                                                                                      \
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

#define APP_LPTMR_IRQ_HANDLER DEMO_LPTMR_IRQHANDLER
#define APP_VBAT_IRQ_HANDLER  VBAT_IRQHandler

#define APP_LPTMR_LOW_POWER_ISO_DOMAINS         (0x0U)
#define APP_WAKEUP_BUTTON_LOW_POWER_ISO_DOMAINS (0x0U)
#define APP_VBAT_LOW_POWER_ISO_DOMAINS          (0x0U)

#define APP_SRAM_ARRAYS_TO_POWER_OFF                                                                       \
    (kCMC_SRAMBank0 | kCMC_SRAMBank1 | kCMC_SRAMBank2 | kCMC_SRAMBank3 | kCMC_SRAMBank4 | kCMC_SRAMBank5 | \
     kCMC_SRAMBank6 | kCMC_SRAMBank7)

#define APP_DEEPSLEEP2_SRAM_POWER_MODE (0x1FU) // 48KB of RAM retained: STCM5(8K), STCM4(8K), STCM3(32K).
#define APP_DEEPSLEEP3_SRAM_POWER_MODE (0x3FU) // 16KB of RAM retained: STCM5(8K), STCM4(8K).
#define APP_DEEPSLEEP4_SRAM_POWER_MODE (0xCFU) // 8KB of RAM retained:  STCM5(8KB).
#define APP_POWERDOWN1_SRAM_POWER_MODE (0x3CU) // 32KB of RAM retained: STCM5(8K), STCM4(8K),CTCM1(8K),CTCM0(8K).
#define APP_POWERDOWN2_SRAM_POWER_MODE (0x3FU) // 16KB of RAM retained: STCM5(8K),STCM4(8K).
#define APP_POWERDOWN3_SRAM_POWER_MODE (0x3FU) // 16KB of RAM retained: STCM5(8K),STCM4(8K).
#define APP_POWERDOWN4_SRAM_POWER_MODE (0xCFU) // 8KB(STCM5) RAM retained.

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
