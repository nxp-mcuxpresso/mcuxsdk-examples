/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_POWER_MODE_NAME \
    {                       \
        "Run", "Standby"    \
    }

#define APP_POWER_MODE_DESC                                                                                            \
    {                                                                                                                  \
        "Run: Full Performance mode, allows the microcontroller to run at maximum 160MHz.",                            \
            "Standby: Low Performance mode, the main core, peripheral, flash memory, and phase-locked loop (PLL) are " \
            "powered off. Most of the SoC is in an inactive state, which significantly reduces power consumption. "    \
            "There is 32KB of standby memory to ensure data retention in the low-power state."                         \
    }

#define APP_SWT     SWT_0
#define APP_RTC     RTC
#define APP_MC_RGM  MC_RGM
#define APP_PIT_RTI PIT_0
#define APP_WKPU    WKPU
#define APP_DCMGPR  DCM_GPR

#define APP_RTC_IRQN  RTC_IRQn
#define APP_PIT_IRQN  PIT0_IRQn
#define APP_WKPU_IRQN WKPU_IRQn

#define APP_PIT_RTI_CLOCK         kCLOCK_Pit0
#define APP_SWT_CLOCK             kCLOCK_Swt0
#define APP_RTC_CLOCK             kCLOCK_Rtc
#define APP_LPCMP_CLOCK           kCLOCK_Lpcmp1

#define APP_TIMER_CLOCK_FFEQUENCY CLOCK_GetFreq(kCLOCK_SircClk)
#define APP_TIMER_CLOCK_SOURCE    kRTC_SIRCClock

#define APP_WAKEUP_PIN_INDEX   55 /*!< Pin index of wakeup pin PTB23*/
#define APP_WAKEUP_BUTTON_NAME "SW2"

#define APP_LPCMP                                     LPCMP_1
#define APP_LPCMP_IRQ_ID                              CMP1_IRQn
#define APP_LPCMP_IRQ_HANDLER_FUNC                    CMP1_IRQHandler
#define APP_LPCMP_ROUND_ROBIN_FIXED_MUX_PORT          kLPCMP_FixedPlusMuxPort
#define APP_LPCMP_ROUND_ROBIN_FIXED_CHANNEL           0U
#define APP_LPCMP_ROUND_ROBIN_CHANNELS_CHECKER_MASK   0x1U
#define APP_LPCMP_ROUND_ROBIN_CHANNELS_PRE_STATE_MASK 0x0U

#define APP_MC_ME_CORE_INDEX 0U

/*
 * (LPCMPx_RRCR0[RR_NSAM] / roundrobin clock period) should bigger than CMP propagation delay,
 * roundrobin clock comes from RTC_CLK (32.768KHz), the propagation delay specified in reference manual is 5us.
 * Note that the range of LPCMPx_RRCR0[RR_NSAM] is limited, so the user needs to set the appropriate
 * roundrobin clock period.
 */
#define APP_LPCMP_ROUND_ROBIN_SAMPLE_CLOCK_NUMBERS (uint8_t)(5U * APP_TIMER_CLOCK_FFEQUENCY / 1000000UL)

/*
 * (LPCMPx_RRCR0[RR_INITMOD] / roundrobin clock period) should bigger than Initialization time,
 * roundrobin clock comes from RTC_CLK (32.768KHz), the initialization time specified in Reference Manual is 30us.
 * Note that the range of LPCMPx_RRCR0[RR_INITMOD] is limited, so the user needs to set the appropriate
 * roundrobin clock period.
 */
#define APP_LPCMP_ROUND_ROBIN_INIT_DELAY_MODULES (uint8_t)(40U * APP_TIMER_CLOCK_FFEQUENCY / 1000000UL)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void APP_DeinitDebugConsole(void);
void APP_DeinitDEBUG_UARTPins(void);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
