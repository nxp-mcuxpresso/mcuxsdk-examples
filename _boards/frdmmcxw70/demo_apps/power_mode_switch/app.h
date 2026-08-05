/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#define APP_MODE_NAME_ARRAY                                                                                              \
    {                                                                                                                    \
        "Active", "Sleep1", "DeepSleep1", "DeepSleep2", "DeepSleep3", "DeepSleep4", "PowerDown1", "PowerDown2", \
            "PowerDown3", "PowerDown4", "DeepPowerDown1", "DeepPowerDown2"                                         \
    }

#define APP_MODE_DESC_ARRAY                                                                                              \
    {                                                                                                                    \
        "Active: Normal run mode.",                                                                                     \
            "Sleep1: Core, platform, peripheral clocks and radio domain enter sleep mode.",                            \
            "DeepSleep1: Core, platform, peripheral clocks and radio domain enter deep sleep mode.",                   \
            "DeepSleep2: Deep sleep profile with alternate SRAM retention settings.",                                   \
            "DeepSleep3: Deep sleep profile with reduced SRAM retention settings.",                                     \
            "DeepSleep4: Deep sleep profile with minimum SRAM retention settings.",                                     \
            "PowerDown1: Power down profile with wake domain retained.",                                                \
            "PowerDown2: Power down profile with main and wake domains in power down.",                                \
            "PowerDown3: Power down profile with radio RAM powered off.",                                               \
            "PowerDown4: Power down profile with radio RAM off and reduced SRAM retention.",                           \
            "DeepPowerDown1: Deep power down profile. Wakeup may restart the application from reset.",                 \
            "DeepPowerDown2: Deep power down profile with the lowest retention settings. On KW43, wakeup restarts through normal boot." \
    }

#define APP_CMC   CMC0
#define APP_LPTMR DEMO_LPTMR_PERIPHERAL
#define APP_WUU   DEMO_WUU_PERIPHERAL
#define APP_SPC   SPC0

#define WUU_WAKEUP_LPTMR_IDX  1U
#define WUU_WAKEUP_BUTTON_NAME BOARD_SW4_NAME
#define WUU_WAKEUP_BUTTON_IDX BOARD_SW4_GPIO_PIN
#define WUU_WAKEUP_BUTTON_FLAG (1UL << WUU_WAKEUP_BUTTON_IDX)

#define APP_LPTMR_IRQ_HANDLER DEMO_LPTMR_IRQHANDLER

#define APP_LPTMR_LOW_POWER_ISO_DOMAINS         (0x0U)
#define APP_WAKEUP_BUTTON_LOW_POWER_ISO_DOMAINS (0x0U)

/* KW43 hardware does not provide VBAT or Smart Power Switch. */
#define DEMO_NOT_SUPPORT_WAKEUP_BOOT           1
#define APP_CLEAR_POWER_DOMAIN3_REQUEST        1

/*
 * KW43 LP_CFG reset default has CORELDO_VDD_LVL = 10b (Normal 1.1 V) and
 * CORELDO_VDD_DS = 0 (Low). The demo's target LP Core LDO state is
 * Mid + Low, so a single SPC_SetLowPowerModeRegulatorsConfig call would
 * try to change voltage and drop drive strength in the same write, which
 * the SPC driver rejects with kStatus_SPC_CORELDOVoltageSetFail. Enabling
 * this knob makes APP_SetSPCConfiguration stage the transition in two
 * steps. See the comment block in power_mode_switch.c for details.
 */
#define DEMO_NEED_STAGED_LP_CORELDO_WRITE      1

/*
 * On KW43 (MW30) the PMC low-power IREF is a shared current reference:
 * it biases OSC32K and it also biases the VDD_SYS_LV always-on domain.
 * Disabling LP_CFG[LP_IREFEN] in Deep Power Down would corrupt
 * VDD_SYS_LV AON, so the board keeps LP_IREF on.
 */
#define DEMO_KEEP_LP_IREF_ENABLED              1

/*
 * KW43 system SRAM partitions (CMC SRAMRET0/SRAMDIS0 register bits):
 *   Bit 0 (RET0/DIS0): SRAM0 lower  (64 KB)
 *   Bit 1 (RET1/DIS1): SRAM0 upper  (64 KB)
 *   Bit 2 (RET2/DIS2): SRAM1         (64 KB)
 *   Bit 3 (RET3/DIS3): SRAM2         (32 KB)
 *   Bit 4 (RET4/DIS4): SRAM3 lower  (16 KB)
 *   Bit 5 (RET5/DIS5): SRAM3 upper  (16 KB)
 *
 * Writing 1 to a RETn bit powers off that SRAM partition in low-power mode.
 *
 * Deep Sleep: default retains 64 KB, reconfigurable to retain full 256 KB.
 * Power Down: max 64 KB retained, reconfigurable to retain fewer banks.
 * Deep Power Down: hardware limits retention to 32 KB, no SRAM config needed.
 */
#define APP_DEEPSLEEP2_SRAM_POWER_MODE (0x38U) /* Power off SRAM2/3, retain SRAM0 and SRAM1 192KB. */
#define APP_DEEPSLEEP3_SRAM_POWER_MODE (0x3CU) /* Power off SRAM1/2/3, retain SRAM0 128KB. */
#define APP_DEEPSLEEP4_SRAM_POWER_MODE (0x3EU) /* Power off upper SRAM0/SRAM1/2/3, retain SRAM0 lower 64KB. */
#define APP_POWERDOWN1_SRAM_POWER_MODE (0x3EU) /* Power off upper SRAM0/SRAM1/2/3, retain SRAM0 lower 64KB. */
#define APP_POWERDOWN2_SRAM_POWER_MODE (0x3EU) /* Power off upper SRAM0/SRAM1/2/3, retain SRAM0 lower 64KB. */
#define APP_POWERDOWN3_SRAM_POWER_MODE (0x3EU) /* Power off upper SRAM0/SRAM1/2/3, retain SRAM0 lower 64KB. */
#define APP_POWERDOWN4_SRAM_POWER_MODE     (0x3FU) /* Power off all SRAM in LP, minimum retention. */
#define APP_DEEPPOWERDOWN1_SRAM_POWER_MODE (0x3EU) /* Retain SRAM0 lower 64KB for partial state restore. */
#define APP_DEEPPOWERDOWN2_SRAM_POWER_MODE (0x3FU) /* Power off all SRAM, wakeup causes Normal Boot. */

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

void APP_InitDebugConsole(void);
void APP_DeinitDebugConsole(void);
void APP_DeinitWakeupButton(void);
void BOARD_InitHardware(void);

#endif /* _APP_H_ */
