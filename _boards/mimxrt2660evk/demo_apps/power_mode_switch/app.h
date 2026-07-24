/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#include "fsl_power.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*
 * Debug UART: HSP domain LPUART0.
 * IRQ = HSP_LPUART0_IRQn (184). Used as UART RX wakeup source.
 */
#define APP_UART_BASEADDR       HSP__LPUART_0
#define APP_UART_IRQ            HSP_LPUART0_IRQn
#define APP_UART_IRQ_HANDLER    HSP_LPUART0_IRQHandler

/*
 * WAKE domain GPIO wakeup button - SW6 = PIO1_0 (WAKE domain, VDD_0V8/VDDIO1).
 * Muxed to the WAKE GPIO function; pin 0 -> channel 0 -> WAKE_GPIO_CH0_IRQn (240).
 * The WAKE domain stays powered when PD_MAIN is OFF (Power Down), so SW6 wakes
 * Sleep (sync) and Deep Sleep / Power Down (async).  Routes through the CMC0
 * IRQ_WAKEUP_MASK, applied by POWER_Enter*().  Not available in Deep Power Down
 * (WAKE domain off) - use SW5 (AON GPIO) for DPD.
 */
#define APP_WAKE_GPIO           WAKE__GPIO
#define APP_WAKE_GPIO_PIN       0U
#define APP_WAKE_GPIO_IRQ       WAKE_GPIO_CH0_IRQn  /* 240 */
#define APP_WAKE_GPIO_IRQ_HANDLER WAKE_GPIO_CH0_IRQHandler
#define APP_WAKE_GPIO_NAME      "SW6"

/*
 * VBAT domain LPTMR: the demo's single LPTMR timer wakeup source across every
 * low power mode (Sleep, Deep Sleep, Power Down, Deep Power Down).  It lives in
 * the always-on VBAT domain and is clocked from ULP32K (FRO32K / OSC32K, ~32768
 * Hz) selected/gated via VBATCON CLOCK_CTRL, so it keeps counting even in DPD
 * where VDD_CORE and the WAKE domain are off.  On compare it drives the VBATCON
 * battery-domain wakeup request (PMIC power-on) for DPD, and a normal NVIC IRQ
 * for the returning / PoR modes.  See REQ-006.
 * VBAT_LPTMR_IRQn = 250.  Wakeup timeout: APP_LPTMR_TIMEOUT_MS milliseconds.
 * APP_LPTMR_CLK_HZ is the ULP32K count rate used to convert the timeout to LPTMR
 * counts (prescaler bypassed).
 */
#define APP_VBAT_LPTMR_BASE        VBAT__LPTMR
#define APP_VBAT_LPTMR_IRQ         VBAT_LPTMR_IRQn  /* 250 */
#define APP_VBAT_LPTMR_IRQ_HANDLER VBAT_LPTMR_IRQHandler
#define APP_LPTMR_CLK_HZ           32768U           /* ULP32K count rate */
#define APP_LPTMR_TIMEOUT_MS       5000U            /* 5 second wakeup timeout */

/*
 * WAKE domain LPTMR0: retained only for the WAKE-domain eDMA-request wakeup
 * demo (kPOWER_WakeupWakeDma_Lptmr0), which inherently needs a WAKE-domain
 * timer.  It is NOT used as a CPU/PMIC timer wakeup source - that role moved to
 * the VBAT LPTMR above.
 * WAKE_LPTMR0_IRQn = 222.
 */
#define APP_WAKE_LPTMR_BASE     WAKE__LPTMR_0
#define APP_WAKE_LPTMR_IRQ      WAKE_LPTMR0_IRQn    /* 222 */
#define APP_WAKE_LPTMR_IRQ_HANDLER WAKE_LPTMR0_IRQHandler

/*
 * VBAT RTC: IRTC (fsl_irtc) in the VBAT domain, used as wakeup source for
 * Sleep/DS/PD via NVIC IRQ (kPOWER_WakeupIrq_VbatRtc with APP_RTC_IRQ).  For Deep
 * Power Down, the RTC is a VBAT-domain peripheral whose alarm drives the VBATCON
 * wakeup request; no per-source enable bit exists (see REQ-006). VBAT_RTC_IRQn = 251.
 * The demo arms an IRTC alarm APP_RTC_ALARM_OFFSET_S seconds ahead.
 */
#define APP_RTC_BASE            VBAT__RTC
#define APP_RTC_IRQ             VBAT_RTC_IRQn       /* 251 */
#define APP_RTC_IRQ_HANDLER     VBAT_RTC_IRQHandler
#define APP_RTC_ALARM_OFFSET_S  10U                 /* 10 second alarm offset */

/*
 * WAKE domain eDMA3 channel 0: drives the DMA-request wakeup demo.  Channel 0
 * runs a memory-to-memory copy inside WAKE_SS_SRAM (0x2600_0000..0x2600_1FFF,
 * 8 KB) paced by WAKE_LPTMR_0: each LPTMR compare raises a DMA request (Table 19
 * source index 31, the index encoded in kPOWER_WakeupWakeDma_Lptmr0) that serves
 * one eDMA minor loop, so the transfer advances across the sleep interval and the
 * channel signals completion after APP_DMA_MAJOR_LOOPS ticks.  The channel-mux
 * request source and the LPTMR->eDMA request generation are [hw-defer].
 * WAKE_EDMA3_CH0_IRQn = 208 (channel completion IRQ / "wake on complete" source).
 */
#define APP_WAKE_EDMA_BASE      WAKE__EDMA3
#define APP_WAKE_EDMA_CHANNEL   0U
#define APP_WAKE_EDMA_IRQ       WAKE_EDMA3_CH0_IRQn /* 208 */
#define APP_WAKE_EDMA_IRQ_HANDLER WAKE_EDMA3_CH0_IRQHandler
#define APP_WAKE_EDMA_REQ_LPTMR0  31U              /* channel-mux request source [hw-defer] */

/* m2m buffers inside WAKE_SS_SRAM (src in the first 4 KB, dst in the second). */
#define APP_WAKE_SS_SRAM_BASE   0x26000000U
#define APP_DMA_SRC_ADDR        (APP_WAKE_SS_SRAM_BASE + 0x0000U)
#define APP_DMA_DST_ADDR        (APP_WAKE_SS_SRAM_BASE + 0x1000U)
#define APP_DMA_MINOR_BYTES     32U                /* bytes moved per LPTMR request */
#define APP_DMA_MAJOR_LOOPS     8U                 /* minor loops (= LPTMR ticks) */
#define APP_DMA_XFER_BYTES      (APP_DMA_MINOR_BYTES * APP_DMA_MAJOR_LOOPS)
#define APP_WAKE_LPTMR_DMA_MS   200U               /* WAKE_LPTMR_0 DMA-request period */

/*
 * AON GPIO wakeup button - SW5 = PIO0_4 (VBAT/AON domain, VDD_CORE_AON/VDD_1V8_AON).
 * Muxed to the VBAT GPIO function; pin 4 -> channel 0 -> VBAT_GPIO_CH0_IRQn (252).
 * PIO0_4 also carries the PMIC ON_OFF function, so SW5 doubles as the DPD wake
 * button.
 *   Sleep / Deep Sleep / Power Down: a normal NVIC IRQ (arm the VBAT GPIO pin +
 *   EnableIRQ(APP_AON_GPIO_IRQ)); the CMC0 IRQ_WAKEUP_MASK is applied by
 *   POWER_Enter*() via kPOWER_WakeupIrq_VbatGpioCh0.
 *   Deep Power Down: the VBAT-domain pad edge drives the VBATCON wakeup request
 *   directly. There is no per-source AON-GPIO wakeup-latch enable in VBATCON
 *   (see REQ-006); no POWERCON mask / NVIC arm applies in DPD.
 */
#define APP_AON_GPIO           VBAT__GPIO
#define APP_AON_GPIO_PIN       4U
#define APP_AON_GPIO_IRQ        VBAT_GPIO_CH0_IRQn  /* 252 */
#define APP_AON_GPIO_IRQ_HANDLER VBAT_GPIO_CH0_IRQHandler
#define APP_AON_GPIO_NAME       "SW5"

/* OCRAM0 retention size for Power Down (kilobytes, 0-64). */
#define APP_PD_OCRAM0_KB        64U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitHardware(void);

/* Power init config defined in the shared example (power_mode_switch.h/.c). BOARD_InitHardware()
 * runs POWER_Init() on this instance as the last bring-up step. */
extern power_topology_config_t s_topologyCfg;
extern power_init_config_t s_powerInitCfg;

#endif /* _APP_H_ */
