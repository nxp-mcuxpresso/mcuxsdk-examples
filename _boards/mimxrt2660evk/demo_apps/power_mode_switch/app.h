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
 * WAKE domain eDMA3 channel 0: drives the DMA-request wakeup demo. Channel 0
 * runs a single-shot memory-to-memory copy inside WAKE_SS_SRAM
 * (0x2600_0000..0x2600_1FFF, 8 KB), triggered by one WAKE_LPTMR_0 compare
 * (Table 19 source index 31, the index encoded in kPOWER_WakeupWakeDma_Lptmr0):
 * the whole APP_DMA_XFER_BYTES transfer is one eDMA minor loop, so that single
 * request moves all of it and the channel signals completion right away. The
 * channel-mux request source and the LPTMR->eDMA request generation are
 * [hw-defer].
 *
 * Two wakeup sources must be armed together: kPOWER_WakeupWakeDma_Lptmr0 wakes
 * CMC2 so WAKE_LPTMR_0 can run and drive the DMA; kPOWER_WakeupIrq_WakeEdma3Ch0
 * wakes CMC0 (CPU) via WAKE_EDMA3_CH0_IRQn once the transfer completes. Arming
 * only one leaves either the DMA never running, or the CPU never woken.
 */
#define APP_WAKE_EDMA_BASE      WAKE__EDMA3
#define APP_WAKE_EDMA_CHANNEL   0U
#define APP_WAKE_EDMA_IRQ       WAKE_EDMA3_CH0_IRQn /* 208 */
#define APP_WAKE_EDMA_IRQ_HANDLER WAKE_EDMA3_CH0_IRQHandler
#define APP_WAKE_EDMA_REQ_LPTMR0  7U              /* WAKE_EDMA3 channel-mux slot for WAKE_LPTMR_0 */

/* m2m buffers inside WAKE_SS_SRAM (src in the first 4 KB, dst in the second). */
#define APP_WAKE_SS_SRAM_BASE   0x26000000U
#define APP_DMA_SRC_ADDR        (APP_WAKE_SS_SRAM_BASE + 0x0000U)
#define APP_DMA_DST_ADDR        (APP_WAKE_SS_SRAM_BASE + 0x1000U)
#define APP_DMA_XFER_BYTES      256U               /* moved in one shot by the single LPTMR request */
#define APP_WAKE_LPTMR_DMA_MS   5000U               /* delay before WAKE_LPTMR_0 fires its one DMA request */

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

/*
 * Bit flags for which low-power modes a wakeup source's hardware domain
 * supports. Mirrors the bit positions of power_mode_switch.h's APP_LP_* codes
 * (1=Sleep, 2=DS1, 3=DS2, 4=DS3, 5=PD, 6=DPD1, 7=DPD2), defined independently
 * here (not via that header's APP_M() macro) because app.h must not depend on
 * the shared example header - only the reverse direction is allowed.
 */
#define APP_WAKEUP_MODE_SLEEP  (1U << 1)
#define APP_WAKEUP_MODE_DS1    (1U << 2)
#define APP_WAKEUP_MODE_DS     ((1U << 2) | (1U << 3) | (1U << 4))
#define APP_WAKEUP_MODE_PD     (1U << 5)
#define APP_WAKEUP_MODE_DPD    ((1U << 6) | (1U << 7))

/*
 * One code per physical wakeup source.  Sync vs async edge detection is NOT
 * enumerated here: it is selected internally by the low-power mode (Sleep =
 * synchronous; Deep Sleep / Power Down = async / clock-less).
 */
#define APP_WAKEUP_UART             0U    /*!< HSP LPUART0 RX (Sleep only: RX-full interrupt; the DS
                                               RX-active-edge wake path produces no wakeup on silicon). */
#define APP_WAKEUP_WAKE_GPIO        1U    /*!< WAKE GPIO / SW6 (PIO1_0); Sleep, DS 1/2/3, PD.       */
#define APP_WAKEUP_AON_GPIO         2U    /*!< AON GPIO / SW5 (PIO0_4); Sleep, DS 1/2/3, PD, DPD.   */
#define APP_WAKEUP_LPTMR            3U    /*!< VBAT LPTMR; Sleep, DS 1/2/3, PD, DPD.                */
#define APP_WAKEUP_RTC              4U    /*!< VBAT RTC alarm; Sleep, DS 1/2/3, PD, DPD (VBAT domain). */
#define APP_WAKEUP_DMA_WAKE         5U    /*!< WAKE eDMA3 CH0 m2m; completion wakes. Sleep, DS1 only. */
#define APP_WAKEUP_NONE             0xFFU /*!< No wakeup source (DPD auto-wake, or back/cancel).    */

/*!
 * @brief Descriptor for one wakeup source, indexed by APP_WAKEUP_* code.
 *
 * Single source of truth for a source's display name plus its board-level
 * enable/disable functions (defined in hardware_init.c).  Those functions own
 * the NVIC EnableIRQ/DisableIRQ and POWER_Enable/DisableWakeupSource calls for
 * every IRQ/POWERCON-mask pair this source needs - not just the peripheral-
 * specific arm/disarm bits - because some sources (e.g. DMA) need more than
 * one such pair and a single irq/powerSrc field here could not represent that.
 * Which demo low-power modes each source supports is @ref modes, a bitmask of
 * APP_WAKEUP_MODE_*.
 */
typedef struct
{
    const char *name;      /*!< Display name (log line + Level 2 menu).      */
    void (*enable)(void);  /*!< Full arm: peripheral bits + EnableIRQ(s) + POWER_EnableWakeupSource(s). */
    void (*disable)(void); /*!< Full disarm, reverse order of enable().      */
    uint8_t     modes;     /*!< Supported LP modes: bitmask of APP_WAKEUP_MODE_*. */
} app_wakeup_source_t;

/*
 * Per-source enable/disable functions are defined in hardware_init.c; forward-declared
 * here so the descriptor table below can reference them.  BOARD_EnableDma()/
 * BOARD_DisableDma() (the shared eDMA transfer setup) are internal to
 * hardware_init.c - not declared here - since only BOARD_EnableDmaWake() calls them.
 */
void BOARD_EnableWakeGpio(void);
void BOARD_DisableWakeGpio(void);
void BOARD_EnableAonGpio(void);
void BOARD_DisableAonGpio(void);
void BOARD_EnableLptmr(void);
void BOARD_DisableLptmr(void);
void BOARD_EnableUart(void);
void BOARD_DisableUart(void);
void BOARD_EnableRtc(void);
void BOARD_DisableRtc(void);
void BOARD_EnableDmaWake(void);
void BOARD_DisableDmaWake(void);

/*!
 * Per-source wakeup descriptor table, indexed by APP_WAKEUP_* code.
 */
static const app_wakeup_source_t APP_WAKEUP_SOURCES[] = {
    [APP_WAKEUP_UART]      = {
      "UART RX",
      BOARD_EnableUart,
      BOARD_DisableUart,
      APP_WAKEUP_MODE_SLEEP
    },
    [APP_WAKEUP_WAKE_GPIO] = {
      "WAKE GPIO / SW6",
      BOARD_EnableWakeGpio,
      BOARD_DisableWakeGpio,
      APP_WAKEUP_MODE_SLEEP | APP_WAKEUP_MODE_DS | APP_WAKEUP_MODE_PD
    },
    [APP_WAKEUP_AON_GPIO]  = {
      "AON GPIO / SW5",
      BOARD_EnableAonGpio,
      BOARD_DisableAonGpio,
      APP_WAKEUP_MODE_SLEEP | APP_WAKEUP_MODE_DS | APP_WAKEUP_MODE_PD | APP_WAKEUP_MODE_DPD
    },
    [APP_WAKEUP_LPTMR]     = {
      "VBAT LPTMR",
      BOARD_EnableLptmr,
      BOARD_DisableLptmr,
      APP_WAKEUP_MODE_SLEEP | APP_WAKEUP_MODE_DS | APP_WAKEUP_MODE_PD | APP_WAKEUP_MODE_DPD
    },
    [APP_WAKEUP_RTC]       = {
      "RTC alarm",
      BOARD_EnableRtc,
      BOARD_DisableRtc,
      APP_WAKEUP_MODE_SLEEP | APP_WAKEUP_MODE_DS | APP_WAKEUP_MODE_PD | APP_WAKEUP_MODE_DPD
    },
    /* DMA (wake on complete): eDMA CH0 completion is the armed wakeup. Sleep and
     * Deep Sleep 1 only - both keep the WAKE domain (and its eDMA3/LPTMR) alive. */
    [APP_WAKEUP_DMA_WAKE]  = {
      "DMA (wake on complete)",
      BOARD_EnableDmaWake,
      BOARD_DisableDmaWake,
      APP_WAKEUP_MODE_SLEEP
    },
};
#define APP_WAKEUP_SOURCE_COUNT \
    ((uint8_t)(sizeof(APP_WAKEUP_SOURCES) / sizeof(APP_WAKEUP_SOURCES[0])))

#define APP_PRINT_DEBUG_INFO 0
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitHardware(void);

/* Brings VBAT__SRAM to ACTIVE (RAM_RD/WR_EN + SD -> SD_PU -> ACT). Defined in
 * hardware_init.c; called from the DPD1 entry path in power_mode_switch.c just
 * before arming the retention marker. */
void BOARD_InitVbatSram(void);

/*! @brief Arm the VBAT_SRAM marker + pattern before a DPD1 entry. */
void BOARD_EnableVbatSram(void);

/*! @brief On boot after a DPD1 wake, report whether the VBAT_SRAM marker + pattern survived. */
void BOARD_VbatSramReport(void);

/*
 * Persistent boot-context markers stored in VBATCON GPRs. VBATCON is in the
 * always-on VBAT domain, so its GPRs survive every Power Down / Deep Power Down
 * PoR - including DPD2, where VBAT__SRAM itself is powered down and unsafe to
 * touch. RESETCON's reset-status bits cannot tell DPD1 apart from DPD2, or which
 * wakeup source was actually enabled for a given entry; these markers close that
 * gap. Written just before a non-returning POWER_Enter*() call in
 * power_mode_switch.c and consumed (read-and-cleared) on the next boot by
 * BOARD_HandleWakeupAfterReset() / main(). Defined in hardware_init.c.
 */
#define BOARD_DPD_VARIANT_DPD2 0U
#define BOARD_DPD_VARIANT_DPD1 1U

void     BOARD_SetDpdVariantMarker(uint32_t variant);
uint32_t BOARD_GetAndClearDpdVariantMarker(void);
void     BOARD_SetEnabledWakeupMarker(uint8_t wakeupSrc);
uint8_t  BOARD_GetAndClearEnabledWakeupMarker(void);

/*!
 * @brief Acknowledge a Power Down / Deep Power Down wake at boot.
 *
 * @return true if this boot was a Deep Power Down wake (DPD1 or DPD2); false
 *         otherwise (Power Down wake or a cold / non-low-power reset).
 */
bool BOARD_HandleWakeupAfterReset(void);

/* Handshake routing config defined in the shared example (power_mode_switch_tables.c).
 * BOARD_InitHardware() applies this via POWER_SetHandshakeRouting() as the last bring-up step
 * (after BOARD_CommonSetting() already applied POWER_SetPolicy() with POR-default routing). */
extern power_handshake_routing_config_t g_handshakeRoutingCfg;

#endif /* _APP_H_ */
