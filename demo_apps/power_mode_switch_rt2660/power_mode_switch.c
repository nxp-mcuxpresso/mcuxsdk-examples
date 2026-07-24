/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * RT2660 Power Mode Switch Demo
 * ==============================
 * Two-level interactive menu demonstrating all RT2660 power modes.
 *
 * Level 1 - low power mode selection:
 *   Run Mode Switch, Sleep, Deep Sleep 1-3, Power Down, Deep Power Down 1-2.
 *
 * Level 2 - wakeup source selection per mode:
 *   Deep Power Down modes show a VBAT-domain Level 2 menu (VBAT LPTMR or SW5).
 *   Run Mode Switch shows a list of target run modes instead.
 *
 * Mode-specific wakeup source tables, power config presets, and menu strings
 * are defined in power_mode_switch.h.
 *
 * Run mode switches use POWER_EnterHpRun / POWER_EnterNormalRun /
 * POWER_EnterLpRun; the current mode is queried from the power driver via
 * POWER_GetCurrentRunMode() (power_run_mode_t).
 *
 * Sleep gates COMPUTE_SS and NPU_SS root clocks via CMC handshake; clock
 * sources and VDD_CORE are unchanged.  The application does not call
 * POWER_EnableWakeupSource() for Sleep - wakeup is handled by NVIC.
 *
 * Deep Sleep / Power Down require POWERCON wakeup mask configuration via
 * POWER_EnableWakeupSource().  Power Down does not return - the MCU performs
 * a full PoR after wakeup.
 *
 * Deep Power Down wakeup is driven from the always-on VBAT domain, NOT by
 * POWER_EnableWakeupSource() (the WAKE/MAIN CMC masks it writes are in domains
 * that are off in DPD).  The DPD Level 2 menu offers two VBAT-domain sources:
 * the VBAT LPTMR (ULP32K) - armed before entry so the SoC auto-wakes after the
 * programmed interval via the VBATCON wakeup request - or SW5 (the PMIC on/off
 * button), whose pad edge drives the same wakeup request in hardware.  DPD does
 * not return - the MCU powers on (full PoR) after power restore.
 *
 * Boot origin detection after Power Down or Deep Power Down is the
 * application's responsibility (e.g., inspect POWERCON WARM_GPR[0] or
 * VBATCON status registers directly).  The fsl_power driver does not provide
 * a POWER_GetWakeupReason() API.
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_power.h"
#include "fsl_resetcon.h"
#include "fsl_clock.h"
#include "fsl_gpio.h"
#include "fsl_lptmr.h"
#include "fsl_lpuart.h"
#include "fsl_vbatcon.h"
#include "fsl_irtc.h"
#include "fsl_edma.h"
#include "app.h"
#include "board.h"
#include "power_mode_switch.h"


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* Wakeup source control (per-source configure fns + framework, arm flag) */
static void APP_SetWakeupSource(uint8_t src, bool arm);
static void APP_SetWakeup(uint8_t src, bool arm);

/* Run mode helpers */
static void           APP_SwitchRunMode(power_run_mode_t target);
static void           APP_WaitTxDone(void);

/* Low Power Run helpers (Deep Sleep entry/exit only) */
static void APP_EnterLowPowerRun(void);
static void APP_ExitLowPowerRun(void);

/* Flat top-level menu + wakeup-source sub-menu */
static void           APP_PrintTopMenu(void);
static uint8_t        APP_ReadMenuSelection(void);
static void           APP_PrintModeResources(uint8_t lpType, uint8_t wakeupSrc);
static uint8_t        APP_RunLevel2WakeupMenu(uint8_t lpType);
static void           APP_EnterLowPower(const app_target_t *t);

/* Transition-chain logging helpers (shared by APP_SwitchRunMode and APP_EnterLowPower) */
static void        APP_BeginTransition(const char *fromName, const char *toName);
static void        APP_LogStep(const char *fromName, const char *toName, const char *note);
static void        APP_LogWakeEvent(const char *wakeSourceName);

/* LPCG load test helpers (REQ-008) */
static void APP_ApplyLpcgTable(const app_lpcg_config_t *table);

/* Table-driven config builders - parse g_powerModeTable and fill POWER_Enter*() structs */
static void APP_BuildSleepConfig(power_sleep_config_t *cfg);
static void APP_BuildDeepSleepConfig(app_standby_mode_idx_t modeIdx, power_deep_sleep_config_t *cfg);
static void APP_BuildPowerDownConfig(power_down_config_t *cfg);
static void APP_BuildDeepPowerDownConfig(uint8_t lpType, power_deep_power_down_config_t *cfg);

/* Topology builder - extracts hskSel from g_powerModeTable and calls POWER_SetTopology */
static void APP_ApplyTopologyForMode(app_standby_mode_idx_t modeIdx);

/*******************************************************************************
 * IRQ Handlers
 ******************************************************************************/
void APP_UART_IRQ_HANDLER(void)
{
    uint32_t flags = LPUART_GetStatusFlags((LPUART_Type *)APP_UART_BASEADDR);

    /* RX active edge is the Deep Sleep wake trigger (clock-less). The triggering
     * character is typically lost while the RX clock restarts, so just clear the flag. */
    if ((flags & kLPUART_RxActiveEdgeFlag) != 0U)
    {
        LPUART_ClearStatusFlags((LPUART_Type *)APP_UART_BASEADDR, kLPUART_RxActiveEdgeFlag);
    }
    /* RX data register full fires in Sleep (clock still running); consume the byte. */
    if ((flags & kLPUART_RxDataRegFullFlag) != 0U)
    {
        (void)LPUART_ReadByte((LPUART_Type *)APP_UART_BASEADDR);
    }
    SDK_ISR_EXIT_BARRIER;
}

void APP_WAKE_GPIO_IRQ_HANDLER(void)
{
    if (GPIO_PinGetInterruptFlag(APP_WAKE_GPIO, APP_WAKE_GPIO_PIN) != 0U)
    {
        GPIO_SetPinInterruptConfig(APP_WAKE_GPIO, APP_WAKE_GPIO_PIN,
                                   kGPIO_InterruptStatusFlagDisabled);
        GPIO_PinClearInterruptFlag(APP_WAKE_GPIO, APP_WAKE_GPIO_PIN);
    }
    SDK_ISR_EXIT_BARRIER;
}

void APP_AON_GPIO_IRQ_HANDLER(void)
{
    /* SW5 = VBAT/AON GPIO (dsc_rgpio); same channel-routed IRQ model as WAKE GPIO. */
    if (GPIO_PinGetInterruptFlag(APP_AON_GPIO, APP_AON_GPIO_PIN) != 0U)
    {
        GPIO_SetPinInterruptConfig(APP_AON_GPIO, APP_AON_GPIO_PIN,
                                   kGPIO_InterruptStatusFlagDisabled);
        GPIO_PinClearInterruptFlag(APP_AON_GPIO, APP_AON_GPIO_PIN);
    }
    SDK_ISR_EXIT_BARRIER;
}

void APP_VBAT_LPTMR_IRQ_HANDLER(void)
{
    /* Only acknowledge the compare; do NOT stop the timer here. This VBAT LPTMR is an
     * always-on counter that is not reset when TEN is cleared, so StopTimer would FREEZE
     * the count at CMR -- the next arm would then compare immediately (TEN gets cleared
     * before low-power entry -> no wake). Leaving it running lets the counter self-reset
     * to 0 at each compare, so every subsequent arm gets a fresh interval. The arm/disarm
     * path (APP_CfgLptmr) owns starting/stopping the timer. */
    LPTMR_ClearStatusFlags(APP_VBAT_LPTMR_BASE, kLPTMR_TimerCompareFlag);
    SDK_ISR_EXIT_BARRIER;
}

void APP_RTC_IRQ_HANDLER(void)
{
    uint32_t flags = IRTC_GetStatusFlags(APP_RTC_BASE);
    if (flags != 0U)
    {
        IRTC_SetWriteProtection(APP_RTC_BASE, false); /* unlock to allow the W1C */
        IRTC_ClearStatusFlags(APP_RTC_BASE, flags);
    }
    SDK_ISR_EXIT_BARRIER;
}

void APP_WAKE_EDMA_IRQ_HANDLER(void)
{
    /* WAKE_EDMA3 CH0 transfer complete: clear DONE / interrupt flags.  In the
     * "DMA (wake on complete)" case this is the wakeup; in "DMA (run, then SW5)"
     * the NVIC line is left disabled, so this handler does not run until torn
     * down. */
    EDMA_ClearChannelStatusFlags(APP_WAKE_EDMA_BASE, APP_WAKE_EDMA_CHANNEL,
                                 (uint32_t)kEDMA_InterruptFlag | (uint32_t)kEDMA_DoneFlag);
    SDK_ISR_EXIT_BARRIER;
}

/*******************************************************************************
 * Wakeup-after-reset handling
 *
 * Power Down and Deep Power Down wake via a PoR, so the NVIC is reset and carries
 * NO pending-IRQ information. Instead, at boot we read the RESETCON reset status to
 * detect a low-power wake, then acknowledge the latched wakeup source from the
 * peripheral status flag that survived in the still-powered domain (VBAT LPTMR /
 * IRTC). Clearing it here deasserts the VBATCON wakeup request that a stale flag
 * would otherwise keep held -- that stale flag was popping the SoC straight back
 * out of DPD when the VBAT LPTMR / RTC source was reused. Doing it once at boot,
 * keyed off the reset cause, is cleaner than clearing defensively in each source's
 * arm path.
 ******************************************************************************/
static void APP_HandleWakeupAfterReset(void)
{
    uint32_t rst     = RESETCON_GetResetStatus(SYSCON__RESETCON);
    bool     fromDpd = (rst & (uint32_t)kRESETCON_SrcWuVbat) != 0U;
    bool     fromPd  = (rst & (uint32_t)kRESETCON_SrcWuPd) != 0U;

    if (!fromDpd && !fromPd)
    {
        return; /* Cold / non-low-power reset: nothing latched to acknowledge. */
    }

    PRINTF("Boot from %s wakeup (RSTSTAT=0x%08X)\r\n",
           fromDpd ? "Deep Power Down" : "Power Down", (unsigned int)rst);

    /* VBAT LPTMR compare flag. Ungate its clock first so the register read is safe
     * even if this wake was not the LPTMR (VBATCON is retained, so this is cheap). */
    VBATCON_EnableLPTMRClockGate(VBAT__VBATCON, false);
    if ((LPTMR_GetStatusFlags(APP_VBAT_LPTMR_BASE) & (uint32_t)kLPTMR_TimerCompareFlag) != 0U)
    {
        PRINTF("  wakeup reason: VBAT LPTMR\r\n");
        LPTMR_ClearStatusFlags(APP_VBAT_LPTMR_BASE, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(APP_VBAT_LPTMR_BASE);
    }

    /* IRTC alarm flag (W1C; unlock write protection as the RTC ISR does). */
    uint32_t rtcFlags = IRTC_GetStatusFlags(APP_RTC_BASE);
    if (rtcFlags != 0U)
    {
        PRINTF("  wakeup reason: RTC alarm\r\n");
        IRTC_SetWriteProtection(APP_RTC_BASE, false);
        IRTC_ClearStatusFlags(APP_RTC_BASE, rtcFlags);
    }

    /* Also clear the latched VBAT power-event flags and the sticky reset status.
     * A wakeup event left latched here (consumed by the first wake but never
     * acknowledged) is the prime suspect for "first wake OK, second wake never
     * fires" -- the VBAT wakeup logic will not re-trigger while a stale latch is
     * held. The EVENTS print is a diagnostic to confirm this on the board. */
    PRINTF("  VBATCON EVENTS=0x%08X\r\n", (unsigned int)VBAT__VBATCON->EVENTS);
    VBATCON_ClearEventFlags(VBAT__VBATCON, (uint32_t)kVBATCON_EventAll);
    RESETCON_ClearStickyResetStatus(SYSCON__RESETCON, (uint32_t)kRESETCON_SrcAll);
}

/*******************************************************************************
 * VBAT_SRAM retention self-check (DPD1 retains, DPD2 does not)
 *
 * DPD wake is a PoR, so retention is verified across the reset: arm a marker +
 * pattern in VBAT_SRAM (0x4630_0000..0x4630_FFFF) just before entering DPD, then
 * report on the next boot. DPD1 keeps VDD_PMU so the marker survives; DPD2 removes
 * VDD_PMU (and POWER_ShutdownVbatSram() powers the SRAM down), so it is lost.
 *
 * Accessed via the absolute address (not a linker section). This assumes the
 * linker maps NO section into VBAT_SRAM; otherwise C startup (.bss zeroing) would
 * clobber the marker and even DPD1 would read as "not retained".
 ******************************************************************************/
#define APP_VBAT_SRAM_BASE  0x46300000UL
#define APP_VBAT_TEST_MAGIC 0x5A5A600DUL

static volatile uint32_t *const s_vbatSram = (volatile uint32_t *)APP_VBAT_SRAM_BASE;

/*! @brief Arm the VBAT_SRAM marker + pattern before a DPD entry (variant 1 or 2). */
static void APP_VbatSramArm(uint8_t dpdVariant)
{
    s_vbatSram[0] = APP_VBAT_TEST_MAGIC;
    s_vbatSram[1] = (uint32_t)dpdVariant;
    for (uint32_t i = 2U; i < 32U; i++)
    {
        s_vbatSram[i] = 0xC0DE0000UL + i;
    }
}

/*! @brief On boot, report whether the VBAT_SRAM marker + pattern survived. */
static void APP_VbatSramReport(void)
{
    if (s_vbatSram[0] != APP_VBAT_TEST_MAGIC)
    {
        PRINTF("VBAT_SRAM check: no valid marker -> cold boot or NOT retained (expected after DPD2)\r\n");
        return;
    }

    bool ok = true;
    for (uint32_t i = 2U; i < 32U; i++)
    {
        if (s_vbatSram[i] != (0xC0DE0000UL + i))
        {
            ok = false;
            break;
        }
    }
    PRINTF("VBAT_SRAM check: marker found (armed before DPD%u) -> %s\r\n",
           (unsigned int)s_vbatSram[1], ok ? "RETAINED (expected after DPD1)" : "CORRUPTED");
    /* Consume the marker so a later cold boot is not misreported as retained. */
    s_vbatSram[0] = 0U;
}

/*******************************************************************************
 * Main
 ******************************************************************************/
int main(void)
{
    uint8_t idx;

    /*
     * BOARD_InitHardware() brings up the board and, as its final step, runs POWER_Init() on the
     * shared-example config literals (s_powerInitCfg / s_topologyCfg in power_mode_switch.h). When
     * the PF9453 PMIC supply is enabled it also initialises the LPI2C1 transport and fills
     * s_powerInitCfg.extSupply before POWER_Init(). To customise for a non-reference board, edit
     * those literals in power_mode_switch.h; also keep the hskSel field of every affected
     * APP_LPCG_TABLE entry in sync with s_topologyCfg so CCM SLICE_CONTROL.HSK_SEL matches.
     */
    BOARD_InitHardware();

    /*
     * Boot origin detection after Power Down or Deep Power Down is the
     * application's responsibility.  The fsl_power driver does not provide a
     * POWER_GetWakeupReason() API.  Applications may inspect POWERCON
     * WARM_GPR[0] or VBATCON status registers to determine boot origin.
     */
    PRINTF("\r\nPower Mode Switch Example Started...\r\n");

    /* If this boot is a Power Down / Deep Power Down wake (a PoR), acknowledge and clear
     * the latched VBAT wakeup source so a stale flag does not immediately re-trigger the
     * next low-power entry. */
    APP_HandleWakeupAfterReset();

    /* VBAT_SRAM retention self-check: report the result of the marker armed before
     * the previous DPD entry (RETAINED after DPD1, lost after DPD2). */
    APP_VbatSramReport();

    /* Enable all peripheral LPCGs per the load-test table (REQ-008).
     * In production, replace kAPP_LpcgModeOn with the desired gating policy
     * per peripheral.  Keep always-on entries (CPU clock, power sequencer)
     * at kAPP_LpcgModeOn regardless of the active mode under test. */
    APP_ApplyLpcgTable(APP_LPCG_TABLE);

    while (true)
    {
        APP_PrintTopMenu();

        idx = APP_ReadMenuSelection();
        if (idx == 0xFFU)
        {
            continue;
        }

        const app_target_t *t = &APP_TARGETS[idx];
        if (idx <= (uint8_t)kAPP_TargetLpRun)
        {
            /* Run target: switch run mode immediately (no wakeup sub-menu). */
            APP_BeginTransition(APP_RUN_MODE_NAMES[POWER_GetCurrentRunMode()], t->shortName);
            APP_SwitchRunMode((power_run_mode_t)t->payload);
            PRINTF("\r\nCurrent mode: %s\r\n", APP_RUN_MODE_NAMES[POWER_GetCurrentRunMode()]);
        }
        else
        {
            /* Low-power target: wakeup menu + entry sequence. */
            APP_EnterLowPower(t);
        }

        PRINTF("Next Loop...\r\n");
    }
}

/*******************************************************************************
 * Per-source wakeup control functions
 *
 * Each configure(bool on) does ONLY the peripheral-specific arm/disarm.  The
 * generic EnableIRQ(irq) + POWER_Enable/DisableWakeupSource(powerSrc) is applied
 * by APP_SetWakeupSource() from the APP_WAKEUP_SOURCES[] descriptor table below.
 ******************************************************************************/

/*
 * GPIO wakeup config shared by both buttons.  SW6 = WAKE GPIO (WAKE domain) and
 * SW5 = VBAT/AON GPIO use the same dsc_rgpio channel-routed model: assign the pin
 * to CH0 and arm either edge (robust to the pad's idle level).
 */
static void APP_CfgGpio(GPIO_Type *base, uint8_t pin, bool on)
{
    if (on)
    {
        GPIO_PinClearInterruptFlag(base, pin);
        GPIO_SetPinInterruptChannel(base, pin, kGPIO_InterruptOutput0);
        GPIO_SetPinInterruptConfig(base, pin, kGPIO_InterruptEitherEdge);
    }
    else
    {
        GPIO_SetPinInterruptConfig(base, pin, kGPIO_InterruptStatusFlagDisabled);
    }
}

static void APP_CfgWakeGpio(bool on)
{
    APP_CfgGpio(APP_WAKE_GPIO, APP_WAKE_GPIO_PIN, on);
}

static void APP_CfgAonGpio(bool on)
{
    APP_CfgGpio(APP_AON_GPIO, APP_AON_GPIO_PIN, on);
}

/*
 * VBAT LPTMR wakeup config.  The LPTMR lives in the always-on VBAT domain and is
 * clocked from ULP32K (FRO32K / OSC32K), not a CGU root clock.  ULP32K keeps
 * running without VDD_CORE, so the same timer covers every low power mode
 * including Deep Power Down; there is no separate sync/async clock path.
 *
 * NOTE: VBATCON_EnableLPTMRClockGate(base, true) *gates* (stops) the clock per
 * the driver contract, so pass false to let it run.  If the timer does not count
 * on silicon, the CLOCK_CTRL.VBAT_LPTMR_CG_EN polarity is the first thing to
 * re-check (marked [hw-defer] in the spec).
 */
static void APP_CfgLptmr(bool on)
{
    if (on)
    {
        lptmr_config_t cfg;

        VBATCON_EnableLPTMRClockGate(VBAT__VBATCON, false);
        (void)VBATCON_SwitchUlp32kSource(VBAT__VBATCON, kVBATCON_Ulp32kSourceFro32k, 0xFFFFU);

        /* Full reset before re-arming: after a DPD wake (a PoR) the normal Sleep/DS
         * tear-down (LPTMR_Deinit) never runs, leaving this VBAT LPTMR in a "used" state
         * where a later LPTMR_Init/StartTimer fails to re-set CSR.TEN (observed: 2nd DPD
         * entry LPTMR CSR=0x40, TEN=0 -> never fires -> no wake). Deinit first to force a
         * clean start every arm, then re-ungate (Deinit gates the clock). */
        LPTMR_Deinit(APP_VBAT_LPTMR_BASE);
        VBATCON_EnableLPTMRClockGate(VBAT__VBATCON, false);

        LPTMR_GetDefaultConfig(&cfg);
        cfg.timerMode = kLPTMR_TimerModeTimeCounter;
        /* Prescaler bypassed (driver default), so one count == one ULP32K tick. */
        LPTMR_Init(APP_VBAT_LPTMR_BASE, &cfg);
        LPTMR_SetTimerPeriod(APP_VBAT_LPTMR_BASE,
                             MSEC_TO_COUNT(APP_LPTMR_TIMEOUT_MS, APP_LPTMR_CLK_HZ));
        LPTMR_EnableInterrupts(APP_VBAT_LPTMR_BASE, kLPTMR_TimerInterruptEnable);
        LPTMR_StartTimer(APP_VBAT_LPTMR_BASE);
    }
    else
    {
        LPTMR_StopTimer(APP_VBAT_LPTMR_BASE);
        LPTMR_Deinit(APP_VBAT_LPTMR_BASE);
    }
}

/*
 * HSP LPUART0 RX wakeup config.  RX-data-full fires in Sleep (clock on); the RX
 * active edge is the clock-less path that wakes Deep Sleep (functional clock
 * gated) - both are armed so the same source works in Sleep and Deep Sleep.  On
 * disarm the wake character(s) are drained so the menu does not consume them.
 */
static void APP_CfgUart(bool on)
{
    if (on)
    {
        /* Clear any stale RX active-edge flag before arming. */
        LPUART_ClearStatusFlags((LPUART_Type *)APP_UART_BASEADDR, kLPUART_RxActiveEdgeFlag);
        LPUART_EnableInterrupts((LPUART_Type *)APP_UART_BASEADDR,
                                kLPUART_RxDataRegFullInterruptEnable |
                                kLPUART_RxActiveEdgeInterruptEnable);
    }
    else
    {
        LPUART_DisableInterrupts((LPUART_Type *)APP_UART_BASEADDR,
                                 kLPUART_RxDataRegFullInterruptEnable |
                                 kLPUART_RxActiveEdgeInterruptEnable);
        /* The wake trigger is the RX start edge, so the triggering byte is
         * usually still shifting in when we get here; spin long enough (>> one
         * 115200-baud character time) for it to finish, draining the RX register
         * on each pass.  Then clear the RX active-edge / overrun flags. */
        for (volatile uint32_t i = 0U; i < 2000000U; i++)
        {
            if ((LPUART_GetStatusFlags((LPUART_Type *)APP_UART_BASEADDR) &
                 (uint32_t)kLPUART_RxDataRegFullFlag) != 0U)
            {
                (void)LPUART_ReadByte((LPUART_Type *)APP_UART_BASEADDR);
            }
        }
        LPUART_ClearStatusFlags((LPUART_Type *)APP_UART_BASEADDR,
                                kLPUART_RxActiveEdgeFlag | kLPUART_RxOverrunFlag);
    }
}

/*
 * VBAT IRTC alarm wakeup.  On arm: init the IRTC, set a fixed base datetime
 * (midnight) and an alarm APP_RTC_ALARM_OFFSET_S seconds later, and enable the
 * alarm interrupt.  On disarm: disable it.  The alarm asserts VBAT_RTC_IRQn (an
 * NVIC IRQ for Sleep/DS/PD; the VBATCON wakeup request for DPD).
 * [hw-defer] pending silicon confirmation of the VBAT IRTC clock source.
 */
static void APP_CfgRtc(bool on)
{
    if (on)
    {
        irtc_config_t   cfg;
        irtc_datetime_t base  = { .year = 2026U, .month = 1U, .day = 1U, .weekDay = 1U,
                                  .hour = 0U, .minute = 0U, .second = 0U };
        irtc_datetime_t alarm = { .year = 2026U, .month = 1U, .day = 1U, .weekDay = 1U,
                                  .hour   = (uint8_t)(APP_RTC_ALARM_OFFSET_S / 3600U),
                                  .minute = (uint8_t)((APP_RTC_ALARM_OFFSET_S / 60U) % 60U),
                                  .second = (uint8_t)(APP_RTC_ALARM_OFFSET_S % 60U) };

        IRTC_GetDefaultConfig(&cfg);
        (void)IRTC_Init(APP_RTC_BASE, &cfg);
        IRTC_SetDatetime(APP_RTC_BASE, &base);
        IRTC_SetAlarm(APP_RTC_BASE, &alarm);
        IRTC_EnableInterrupts(APP_RTC_BASE, kIRTC_AlarmInterruptEnable);
    }
    else
    {
        IRTC_DisableInterrupts(APP_RTC_BASE, kIRTC_AlarmInterruptEnable);
    }
}

/*
 * WAKE eDMA3 CH0 memory-to-memory wakeup, paced by WAKE_LPTMR_0.
 *
 * On arm: seed the source buffer, set up a CH0 m2m transfer inside WAKE_SS_SRAM
 * (APP_DMA_XFER_BYTES in APP_DMA_MAJOR_LOOPS minor loops), point the channel
 * request mux at the WAKE_LPTMR_0 request line, open both the normal and the
 * async request gates (the LPTMR's low-power clock is async to the eDMA bus
 * clock), enable the CH0 major-loop interrupt, then start WAKE_LPTMR_0 raising a
 * periodic DMA request.  Each request advances one minor loop, so the copy runs
 * across the sleep interval and CH0 completes after APP_DMA_MAJOR_LOOPS ticks.
 *
 * On disarm: stop the timer, disable the request, and clear the channel.  Whether
 * the channel-mux request index and the LPTMR->eDMA request path are correct is
 * [hw-defer] pending silicon.
 */
static void APP_CfgDma(bool on)
{
    if (on)
    {
        edma_config_t         edmaCfg;
        edma_transfer_config_t xfer;
        lptmr_config_t         lpCfg;
        uint8_t               *src = (uint8_t *)APP_DMA_SRC_ADDR;

        /* Seed the source buffer so the copy moves observable data. */
        for (uint32_t i = 0U; i < APP_DMA_XFER_BYTES; i++)
        {
            src[i] = (uint8_t)(i + 1U);
        }

        EDMA_GetDefaultConfig(&edmaCfg);
        EDMA_Init(APP_WAKE_EDMA_BASE, &edmaCfg);
        EDMA_SetChannelMux(APP_WAKE_EDMA_BASE, APP_WAKE_EDMA_CHANNEL, APP_WAKE_EDMA_REQ_LPTMR0);

        EDMA_PrepareTransfer(&xfer,
                             (void *)APP_DMA_SRC_ADDR, sizeof(uint32_t),
                             (void *)APP_DMA_DST_ADDR, sizeof(uint32_t),
                             APP_DMA_MINOR_BYTES, APP_DMA_XFER_BYTES,
                             kEDMA_MemoryToMemory);
        EDMA_SetTransferConfig(APP_WAKE_EDMA_BASE, APP_WAKE_EDMA_CHANNEL, &xfer, NULL);
        EDMA_EnableChannelInterrupts(APP_WAKE_EDMA_BASE, APP_WAKE_EDMA_CHANNEL,
                                     (uint32_t)kEDMA_MajorInterruptEnable);
        /* LPTMR clock is async to the eDMA bus clock -> open the async gate too. */
        EDMA_EnableAsyncRequest(APP_WAKE_EDMA_BASE, APP_WAKE_EDMA_CHANNEL, true);
        EDMA_EnableChannelRequest(APP_WAKE_EDMA_BASE, APP_WAKE_EDMA_CHANNEL);

        /* WAKE_LPTMR_0 free-runs, raising a DMA request on each compare. */
        LPTMR_GetDefaultConfig(&lpCfg);
        lpCfg.timerMode = kLPTMR_TimerModeTimeCounter;
        LPTMR_Init(APP_WAKE_LPTMR_BASE, &lpCfg);
        LPTMR_SetTimerPeriod(APP_WAKE_LPTMR_BASE,
                             MSEC_TO_COUNT(APP_WAKE_LPTMR_DMA_MS, APP_LPTMR_CLK_HZ));
        LPTMR_EnableTimerDMA(APP_WAKE_LPTMR_BASE, true);
        LPTMR_StartTimer(APP_WAKE_LPTMR_BASE);
    }
    else
    {
        LPTMR_StopTimer(APP_WAKE_LPTMR_BASE);
        LPTMR_EnableTimerDMA(APP_WAKE_LPTMR_BASE, false);
        LPTMR_Deinit(APP_WAKE_LPTMR_BASE);
        EDMA_ClearChannelStatusFlags(APP_WAKE_EDMA_BASE, APP_WAKE_EDMA_CHANNEL,
                                     (uint32_t)kEDMA_InterruptFlag | (uint32_t)kEDMA_DoneFlag);
        EDMA_Deinit(APP_WAKE_EDMA_BASE);
    }
}

/*
 * "DMA (run, then SW5)" variant: arm the same eDMA transfer, plus SW5 / AON GPIO
 * as the actual wakeup.  APP_SetWakeupSource() uses this entry's irq/powerSrc
 * (SW5), so the eDMA CH0 NVIC line is NOT enabled here - the copy runs and
 * completes while the core stays asleep, and SW5 is what wakes it.
 */
static void APP_CfgDmaSw5(bool on)
{
    APP_CfgDma(on);
    APP_CfgAonGpio(on);
}

/*!
 * @brief Arm (arm=true) or disarm a wakeup source from its descriptor.
 *
 * Runs the source's configure() for the peripheral-specific bits, then the
 * generic EnableIRQ(irq) + POWER_EnableWakeupSource(powerSrc) (reverse order on
 * disarm).  Sync vs async edge mode is a hardware property of the low-power mode,
 * not a separate source, so a single code per source is armed here.
 *
 * Bring-up note: the POWERCON mask is programmed for every mode, including Sleep
 * (NVIC still handles the actual wakeup - the mask is harmless) and Deep Power
 * Down (armed alongside the VBATCON latch).  This is intentionally broader than
 * REQ-006's VBATCON-only DPD wakeup; see the task log.
 */
static void APP_SetWakeupSource(uint8_t src, bool arm)
{
    const app_wakeup_source_t *ws = &APP_WAKEUP_SOURCES[src];

    if (arm)
    {
        if (ws->configure != NULL) { ws->configure(true); }
        EnableIRQ(ws->irq);
        POWER_EnableWakeupSource(ws->powerSrc);
    }
    else
    {
        DisableIRQ(ws->irq);
        POWER_DisableWakeupSource(ws->powerSrc);
        if (ws->configure != NULL) { ws->configure(false); }
    }
}

/*******************************************************************************
 * Wakeup configuration
 ******************************************************************************/

/*!
 * @brief Arm (arm=true) or tear down a wakeup source, bracketed by
 *        POWER_ClearAllWakeupSources().
 *
 * Configures the full source (peripheral + POWERCON mask) via
 * APP_SetWakeupSource() with a clean-slate clear on either side.  Used at every
 * low-power entry (Sleep / Deep Sleep / Power Down / Deep Power Down).
 */
static void APP_SetWakeup(uint8_t src, bool arm)
{
    if (arm)
    {
        POWER_ClearAllWakeupSources();
        APP_SetWakeupSource(src, true);
    }
    else
    {
        APP_SetWakeupSource(src, false);
        POWER_ClearAllWakeupSources();
    }
}

/*******************************************************************************
 * CorePLL reconfiguration helper
 *
 * Run-mode clock switching for the three active run modes is provided by the
 * common board clock config (BOARD_BootClockHPRUN / NPRUN / LPRUN), which is
 * passed as the clock callback to POWER_Enter*Run() in APP_SwitchRunMode().
 * The helper below is used only by the Low Power Run exit path to restore the
 * CorePLL frequency around Deep Sleep.
 ******************************************************************************/

/*!
 * @brief Reinitialize COREPLL with the given integer loop-division factor.
 *
 * Used by APP_ExitLowPowerRun() to restore the CorePLL frequency
 * (loopDivNint=25 -> 24 MHz x 25 = 600 MHz) after Deep Sleep.  Safe to call
 * while the CPU is parked on a non-CorePLL source.
 *
 * TODO: add CLOCK_SetRootClockMux() mux steps when that API is available:
 *   CLOCK_SetRootClockMux(kCLOCK_Root_CGU_MAIN_ROOTCLK, kCLOCK_CGU_MAIN_ClockRoot_BASE);
 *   ... (COREPLL reinit below) ...
 *   CLOCK_SetRootClockMux(kCLOCK_Root_CGU_MAIN_ROOTCLK, kCLOCK_CGU_MAIN_ClockRoot_COREPLL_OUT);
 */
static void APP_ReconfigureCorePll(uint32_t loopDivNint)
{
    const clock_cguana_core_pll_config_t cfg = {
        .startMode   = kCLOCK_CguanaPllStartFull,
        .vcoSelHf    = false,   /* LF VCO: 600-1258 MHz */
        .refFreq     = kCLOCK_CguanaRefFreq24M,
        .loopDivNint = loopDivNint,
        .postDivBy2  = false,
    };
    CLOCK_DeinitCorePll();
    CLOCK_InitCorePll(&cfg);
}

/*******************************************************************************
 * Low Power Run helpers
 *
 * APP_EnterLowPowerRun() / APP_ExitLowPowerRun() bracket POWER_EnterDeepSleep()
 * in the DS case body.  They are NOT tracked run modes and are NOT routed
 * through APP_SwitchRunMode().
 *
 * Motivation: POWERCON's MainPLL/SysPLL CGUANA enable is a combined PLL+DIV4
 * bit.  POWERCON restores PLL+DIV4 as a unit on wake.  The other CGU root
 * clocks (DIV5/8/10/20, SYSPLLDIV5/X) are separate SW-managed gates that are
 * not touched by POWERCON and must be explicitly gated before deep sleep entry
 * and ungated after wake.
 ******************************************************************************/

/*!
 * @brief Gate non-DIV4 PLL root clocks and switch buses to FRO192 before
 * deep sleep entry.
 *
 * Called immediately before POWER_EnterDeepSleep().  On entry the system is
 * already in LP run mode (CorePLL at 600 MHz, buses on LP PLL sources).
 * SYSPLLDIV4_ROOTCLK is intentionally left ON because XSPI Flash uses
 * SYSPLL_DIV4/2 = 250 MHz.
 */
static void APP_EnterLowPowerRun(void)
{
    /* Switch all bus clocks to FRO192 / LP2M before gating PLLs. */
    CLOCK_SetRootClockMux(kCLOCK_Root_CGU_MAIN_ROOTCLK,      kCLOCK_CGU_MAIN_ClockRoot_BASE);
    CLOCK_SetRootClockMux(kCLOCK_Root_CGU_NPU_ROOTCLK,      kCLOCK_NPU_ClockRoot_BASE);
    CLOCK_SetRootClockMux(kCLOCK_Root_CGU_MEDIABUS_ROOTCLK, kCLOCK_MEDIABUS_ClockRoot_BASE);
    CLOCK_SetRootClockMux(kCLOCK_Root_CGU_AUDIOBUS_ROOTCLK, kCLOCK_AUDIOBUS_ClockRoot_BASE);
    CLOCK_SetRootClockMux(kCLOCK_Root_CGU_COMMBUS_ROOTCLK,  kCLOCK_COMMBUS_ClockRoot_BASE);
    CLOCK_SetRootClockMux(kCLOCK_Root_CGU_WAKEBUS_ROOTCLK,  kCLOCK_WAKEBUS_ClockRoot_LOW);
    /* Gate CorePLL CPU and buses no longer need it. */
    CLOCK_DeinitCorePll();
    /* Gate non-DIV4 PLL root clocks.  POWERCON restores the PLLs and their
     * DIV4 outputs on wake via the combined CGUANA enable; the roots below
     * are separately SW-managed and must be re-enabled by APP_ExitLowPowerRun. */
    CLOCK_PowerOffRootClock(kCLOCK_Root_CGU_MAINPLLDIVX_ROOTCLK);
    CLOCK_PowerOffRootClock(kCLOCK_Root_CGU_MAINPLLDIV8_ROOTCLK);
    CLOCK_PowerOffRootClock(kCLOCK_Root_CGU_MAINPLLDIV10_ROOTCLK);
    CLOCK_PowerOffRootClock(kCLOCK_Root_CGU_MAINPLLDIV20_ROOTCLK);
    CLOCK_PowerOffRootClock(kCLOCK_Root_CGU_SYSPLLDIV5_ROOTCLK);
    CLOCK_PowerOffRootClock(kCLOCK_Root_CGU_SYSPLLDIVX_ROOTCLK);
    /* SYSPLLDIV4_ROOTCLK stays ON XSPI Flash: SYSPLL_DIV4/2 = 250 MHz. */
}

/*!
 * @brief Restore non-DIV4 PLL root clocks and bus muxes to LP state after
 * POWER_EnterDeepSleep() returns.
 *
 * POWERCON has already re-enabled MAINPLL+DIV4 and SYSPLL+DIV4 via their
 * combined CGUANA bits.  This function re-enables the separately-gated roots,
 * restores CorePLL to the LP frequency (600 MHz), and switches buses back to
 * their LP PLL sources so that POWER_GetCurrentRunMode() reports LP.
 * The subsequent s_lpRunCfg post-wake call to APP_SwitchRunMode(kPOWER_RunModeNormal)
 * then only needs to bump CorePLL to 792 MHz.
 */
static void APP_ExitLowPowerRun(void)
{
    /* Re-enable non-DIV4 PLL root clocks (POWERCON restored PLL+DIV4). */
    CLOCK_PowerOnRootClock(kCLOCK_Root_CGU_MAINPLLDIVX_ROOTCLK);
    CLOCK_PowerOnRootClock(kCLOCK_Root_CGU_MAINPLLDIV8_ROOTCLK);
    CLOCK_PowerOnRootClock(kCLOCK_Root_CGU_MAINPLLDIV10_ROOTCLK);
    CLOCK_PowerOnRootClock(kCLOCK_Root_CGU_MAINPLLDIV20_ROOTCLK);
    CLOCK_PowerOnRootClock(kCLOCK_Root_CGU_SYSPLLDIV5_ROOTCLK);
    CLOCK_PowerOnRootClock(kCLOCK_Root_CGU_SYSPLLDIVX_ROOTCLK);
    /* Restore CorePLL to LP frequency (run mode is LP). */
    APP_ReconfigureCorePll(25U); /* 24 * 25 = 600 MHz */
    /* Restore bus mux from FRO192 back to LP PLL sources. */
    CLOCK_SetRootClockMux(kCLOCK_Root_CGU_MAIN_ROOTCLK,      kCLOCK_CGU_MAIN_ClockRoot_COREPLL_OUT);
    CLOCK_SetRootClockMux(kCLOCK_Root_CGU_NPU_ROOTCLK,      kCLOCK_NPU_ClockRoot_MAINPLL_DIVOUT1);
    CLOCK_SetRootClockMux(kCLOCK_Root_CGU_MEDIABUS_ROOTCLK, kCLOCK_MEDIABUS_ClockRoot_SYSPLL_DIVOUT2);
    CLOCK_SetRootClockMux(kCLOCK_Root_CGU_AUDIOBUS_ROOTCLK, kCLOCK_AUDIOBUS_ClockRoot_MAINPLL_DIVX);
    CLOCK_SetRootClockMux(kCLOCK_Root_CGU_COMMBUS_ROOTCLK,  kCLOCK_COMMBUS_ClockRoot_MAINPLL_DIVX);
    CLOCK_SetRootClockMux(kCLOCK_Root_CGU_WAKEBUS_ROOTCLK,  kCLOCK_WAKEBUS_ClockRoot_MAINPLL_DIVX);
}

/*******************************************************************************
 * Run mode helpers
 *
 * The demo queries the current run mode directly via POWER_GetCurrentRunMode()
 * (power_run_mode_t); there is no example-level run-mode enum or cached copy.
 ******************************************************************************/
/*!
 * @brief Per-user-pick step counter for the transition-chain printer.
 *
 * Reset at the start of every top-level mode dispatch.  Each call to
 * APP_LogStep() increments it and prefixes the printed line with the count.
 */
static uint8_t s_stepNum;

static void APP_BeginTransition(const char *fromName, const char *toName)
{
    s_stepNum = 0U;
    PRINTF("\r\nTransition: %s -> %s\r\n", fromName, toName);
}

static void APP_LogStep(const char *fromName, const char *toName, const char *note)
{
    s_stepNum++;
    if ((note != NULL) && (note[0] != '\0'))
    {
        PRINTF("  [step %u] %s -> %s  (%s)\r\n",
               (uint32_t)s_stepNum, fromName, toName, note);
    }
    else
    {
        PRINTF("  [step %u] %s -> %s\r\n",
               (uint32_t)s_stepNum, fromName, toName);
    }
}

static void APP_LogWakeEvent(const char *wakeSourceName)
{
    if ((wakeSourceName != NULL) && (wakeSourceName[0] != '\0'))
    {
        PRINTF("  [wake event: %s]\r\n", wakeSourceName);
    }
    else
    {
        PRINTF("  [wake event]\r\n");
    }
}

static void APP_SwitchRunMode(power_run_mode_t target)
{
    const char *fromName = APP_RUN_MODE_NAMES[POWER_GetCurrentRunMode()];
    const char *toName   = APP_RUN_MODE_NAMES[target];

    if (POWER_GetCurrentRunMode() == target)
    {
        APP_LogStep(fromName, toName, "already in target run mode");
        return;
    }
    switch (target)
    {
        case kPOWER_RunModeHp:
            POWER_EnterHpRun(BOARD_BootClockHPRUN);
            break;
        case kPOWER_RunModeNormal:
            POWER_EnterNormalRun(BOARD_BootClockNPRUN);
            break;
        case kPOWER_RunModeLp:
            POWER_EnterLpRun(BOARD_BootClockLPRUN);
            break;
        default:
            break;
    }
    APP_LogStep(fromName, toName, "run mode switched");
}

static void APP_WaitTxDone(void)
{
    while ((LPUART_GetStatusFlags((LPUART_Type *)APP_UART_BASEADDR) &
            kLPUART_TransmissionCompleteFlag) == 0U)
    {
    }
}

/*******************************************************************************
 * LPCG load test helpers ******************************************************************************/

/*!
 * @brief Apply a sentinel-terminated LPCG configuration table.
 *
 * Iterates APP_LPCG_TABLE[] (or any conforming table) and calls
 * CLOCK_SetClockGateMode() for each entry until the sentinel
 * { kCLOCK_IpInvalid, kAPP_LpcgModeOff } is reached.
 *
 * Call once after POWER_Init() and the initial run-mode selection so that
 * all peripheral clocks are gated according to the table before the first
 * menu iteration.  For active-mode load characterisation, the default table
 * sets every LPCG to kAPP_LpcgModeOn (mode 3).
 *
 * @param table  Pointer to the first element of the LPCG config table.
 */
static void APP_ApplyLpcgTable(const app_lpcg_config_t *table)
{
    assert(table != NULL);
    while (table->lpcg != kCLOCK_IpInvalid)
    {
        CLOCK_SetClockGateMode(table->lpcg, (clock_gate_value_t)table->mode,
                               table->hskSel, table->bypassHandshake);
        table++;
    }
}

/*******************************************************************************
 * Table-driven config builders
 *
 * Each builder starts from a zero-initialised struct and iterates the
 * g_powerModeTable column for its mode, applying every valid cell to the
 * matching struct field.  Cells marked _NC_ (valid=false) are skipped so
 * the field stays 0 (driver default).
 *
 * To change a mode's resource configuration, edit the corresponding cell
 * in g_powerModeTable inside power_mode_switch.h - no code change required.
 ******************************************************************************/

/*
 * Data-driven Sleep / Deep Sleep field applicator.
 *
 * power_sleep_config_t and power_deep_sleep_config_t carry the same field
 * names with the same types and the same layout.  A single offsetof-based
 * lookup table (s_slpFieldMap) therefore serves both structs.
 *
 * Resources that have no field in these structs (PD/DPD-only rows) have
 * type kFT_Skip.  Their cells are always _NC_ (valid=false) so the outer
 * loop filters them before the table is even consulted.
 */
typedef enum
{
    kFT_Skip = 0, /*!< PD/DPD-only resource - not in Sleep/DeepSleep struct */
    kFT_Bool,     /*!< bool field: written as (cell->value != 0U)            */
    kFT_U8,       /*!< uint8_t field                                         */
    kFT_U32,      /*!< 32-bit field (all enum types: pdcon_event_t, pmu_dcdc_mode_t, etc.) */
} app_field_type_t;

typedef struct
{
    uint16_t offset; /*!< Byte offset of the field (from offsetof). */
    uint8_t  type;   /*!< app_field_type_t tag. */
} app_slp_field_t;

#define _SF(field, ft) { (uint16_t)offsetof(power_sleep_config_t, field), (uint8_t)(ft) }

static const app_slp_field_t s_slpFieldMap[kPOWER_Resource_COUNT] = {
    [kPOWER_Resource_CpuDomainEvent]   = _SF(setCpuDomainEvent,   kFT_U32),
    [kPOWER_Resource_NpuDomainEvent]   = _SF(setNpuDomainEvent,   kFT_U32),
    [kPOWER_Resource_CommDomainEvent]  = _SF(setCommDomainEvent,  kFT_U32),
    [kPOWER_Resource_MediaDomainEvent] = _SF(setMediaDomainEvent, kFT_U32),
    [kPOWER_Resource_PmuMode]          = _SF(setPmuMode,          kFT_U8),
    [kPOWER_Resource_DcdcMode]         = _SF(setDcdcMode,         kFT_U32),
    [kPOWER_Resource_CoreLvl]          = _SF(setCoreLevel,          kFT_U8),
    [kPOWER_Resource_Ldo0V8Mode]       = _SF(setLdo0V8Mode,       kFT_U32),
    [kPOWER_Resource_Ldo1V8Mode]       = _SF(setLdo1V8Mode,       kFT_U32),
    [kPOWER_Resource_LdoVdda1V8Mode]   = _SF(setLdoVdda1V8Mode,   kFT_U8),
    [kPOWER_Resource_Hqref]            = _SF(enableHqref,            kFT_Bool),
    [kPOWER_Resource_Sensors]          = _SF(enableSensors,          kFT_U8),
    [kPOWER_Resource_ClkLdoa0V8]       = _SF(enableLdoa0V8ClockSource,          kFT_Bool),
    [kPOWER_Resource_ClkFro192M]       = _SF(enableFro192MClockSource,          kFT_Bool),
    [kPOWER_Resource_ClkFro12M]        = _SF(enableFro12MClockSource,           kFT_Bool),
    [kPOWER_Resource_ClkMainPll]       = _SF(enableMainPllClockSource,          kFT_Bool),
    [kPOWER_Resource_ClkCorePll]       = _SF(enableCorePllClockSource,          kFT_Bool),
    [kPOWER_Resource_ClkSysPll]        = _SF(enableSysPllClockSource,           kFT_Bool),
    [kPOWER_Resource_ClkLdoq0V8]       = _SF(enableLdoq0V8ClockSource,          kFT_Bool),
    [kPOWER_Resource_ClkSxosc]         = _SF(enableSxoscClockSource,            kFT_Bool),
    [kPOWER_Resource_ClkFro12MLp]      = _SF(enableFro12MLpClockSource,         kFT_Bool),
    [kPOWER_Resource_WakeSram]         = _SF(setWakeSramMode,      kFT_U32),
    [kPOWER_Resource_Ocram]            = _SF(setOcramMode,         kFT_U32),
    [kPOWER_Resource_LlcCache]         = _SF(setLlcCacheMode,      kFT_U32),
    [kPOWER_Resource_Audio8Kb]         = _SF(setAudio8KbMode,      kFT_U32),
    [kPOWER_Resource_M85Tcm]           = _SF(setM85TcmMode,        kFT_U32),
    [kPOWER_Resource_NpuTcm]           = _SF(setNpuTcmMode,        kFT_U32),
    /* Root-clock enables map to power_deep_sleep_config_t bool fields. */
    [kPOWER_Resource_RcgCompute]       = _SF(enableComputeRootClock,        kFT_Bool),
    [kPOWER_Resource_RcgMain]          = _SF(enableMainRootClock,           kFT_Bool),
    [kPOWER_Resource_RcgWake]          = _SF(enableWakeRootClock,           kFT_Bool),
    [kPOWER_Resource_RcgComm]          = _SF(enableCommRootClock,           kFT_Bool),
    [kPOWER_Resource_RcgMedia]         = _SF(enableMediaRootClock,          kFT_Bool),
    [kPOWER_Resource_RcgAudio]         = _SF(enableAudioRootClock,          kFT_Bool),
    [kPOWER_Resource_RcgWake1M]        = _SF(enableWake1MRootClock,         kFT_Bool),
    [kPOWER_Resource_RcgWake2M]        = _SF(enableWake2MRootClock,         kFT_Bool),
};
#undef _SF

/*!
 * @brief Apply g_powerModeTable cells to a Sleep or Deep Sleep config struct.
 *
 * power_sleep_config_t and power_deep_sleep_config_t share identical field
 * layouts, so s_slpFieldMap offsets work for both.  The caller passes the
 * config struct as uint8_t* for byte-offset access; the type tag in the map
 * drives each field write.
 *
 * @param modeIdx  Column index in g_powerModeTable.
 * @param base     Pointer to the first byte of a zero-initialised config struct.
 */
static void APP_ApplySleepFields(app_standby_mode_idx_t modeIdx, uint8_t *base)
{
    uint32_t r;
    assert(base != NULL);
    for (r = 0U; r < (uint32_t)kPOWER_Resource_COUNT; r++)
    {
        const power_mode_cell_t *cell = &g_powerModeTable[r][(uint32_t)modeIdx];
        if (!cell->valid)
        {
            continue;
        }
        const app_slp_field_t *f = &s_slpFieldMap[r];
        if (f->type == (uint8_t)kFT_Skip)
        {
            continue;
        }
        switch ((app_field_type_t)f->type)
        {
            case kFT_Bool: *(bool     *)(base + f->offset) = (cell->value != 0U); break;
            case kFT_U8:   *(uint8_t  *)(base + f->offset) = (uint8_t)cell->value; break;
            case kFT_U32:  *(uint32_t *)(base + f->offset) = cell->value;          break;
            default:       break;
        }
    }
}

/*!
 * @brief Build a power_sleep_config_t from the Sleep column of g_powerModeTable.
 */
static void APP_BuildSleepConfig(power_sleep_config_t *cfg)
{
    assert(cfg != NULL);
    (void)memset(cfg, 0, sizeof(*cfg));
    APP_ApplySleepFields(kAPP_StandbyModeIdx_Sleep, (uint8_t *)cfg);
}

/*!
 * @brief Build a power_deep_sleep_config_t from the specified DS column of g_powerModeTable.
 *
 * All column values are read directly from the table with no runtime overrides.
 * DMA-specific clock-source differences are applied later by APP_BuildConfig().
 *
 * @param modeIdx  DS column to use (one of kAPP_StandbyModeIdx_DS*).
 * @param cfg      Output config struct.  Must not be NULL.
 */
static void APP_BuildDeepSleepConfig(app_standby_mode_idx_t modeIdx, power_deep_sleep_config_t *cfg)
{
    assert(cfg != NULL);
    (void)memset(cfg, 0, sizeof(*cfg));
    APP_ApplySleepFields(modeIdx, (uint8_t *)cfg);
}

/* Preserve the legacy DMA-only Deep Sleep clock-source deltas after removing
 * the dedicated DSx_Dma table columns.
 */
static void APP_ApplyDmaDeepSleepOverrides(uint8_t lpType, power_deep_sleep_config_t *cfg)
{
    assert(cfg != NULL);

    switch (lpType)
    {
        case APP_LP_DS1:
            cfg->enableFro12MClockSource = false;
            cfg->enableMainPllClockSource = true;
            cfg->enableLdoq0V8ClockSource = true;
            cfg->enableSxoscClockSource = true;
            cfg->enableFro12MLpClockSource = false;
            break;

        case APP_LP_DS2:
        case APP_LP_DS3:
            cfg->enableLdoa0V8ClockSource = true;
            cfg->enableFro192MClockSource = true;
            cfg->enableMainPllClockSource = true;
            cfg->enableLdoq0V8ClockSource = true;
            cfg->enableSxoscClockSource = true;
            break;

        default:
            break;
    }
}

/*!
 * Maps a kPOWER_Resource_Clk* or kPOWER_Resource_Rcg* resource to the
 * POWERCON HSK_SEL register word and 3-bit field position it controls.
 *
 * regIdx:  0 = rcgcfgHskSel
 *          1 = csrccfgHskSel
 *          2 = csrccfgHskSel1
 * shift:   nibble-aligned bit offset of the 3-bit HSK_SEL field (0,4,8,...28).
 *
 * Non-topology resources have {0,0} (zero-initialized); they are never
 * accessed because their power_mode_cell_t.hskSel is always 0.
 */
typedef struct
{
    uint8_t regIdx; /*!< POWERCON HSK_SEL register index: 0=rcgcfg, 1=csrccfg, 2=csrccfg1 */
    uint8_t shift;  /*!< Nibble-aligned bit offset of the 3-bit HSK_SEL field (0,4,...28). */
} app_topo_field_t;

static const app_topo_field_t s_topoFieldMap[kPOWER_Resource_COUNT] = {
    /* CSRCCFG_HSK_SEL (regIdx=1) */
    [kPOWER_Resource_ClkLdoa0V8]  = { 1U,  0U },
    [kPOWER_Resource_ClkFro192M]  = { 1U,  4U },
    [kPOWER_Resource_ClkFro12M]   = { 1U,  8U },
    [kPOWER_Resource_ClkMainPll]  = { 1U, 12U },
    [kPOWER_Resource_ClkCorePll]  = { 1U, 16U },
    [kPOWER_Resource_ClkSysPll]   = { 1U, 20U },
    [kPOWER_Resource_ClkLdoq0V8]  = { 1U, 24U },
    [kPOWER_Resource_ClkSxosc]    = { 1U, 28U },
    /* CSRCCFG_HSK_SEL1 (regIdx=2) */
    [kPOWER_Resource_ClkFro12MLp] = { 2U,  0U },
    /* RCGCFG_HSK_SEL (regIdx=0) */
    [kPOWER_Resource_RcgCompute]  = { 0U,  0U },
    [kPOWER_Resource_RcgMain]     = { 0U,  4U },
    [kPOWER_Resource_RcgWake]     = { 0U,  8U },
    [kPOWER_Resource_RcgComm]     = { 0U, 12U },
    [kPOWER_Resource_RcgMedia]    = { 0U, 16U },
    [kPOWER_Resource_RcgAudio]    = { 0U, 20U },
    [kPOWER_Resource_RcgWake1M]   = { 0U, 24U },
    [kPOWER_Resource_RcgWake2M]   = { 0U, 28U },
};

/*!
 * @brief Build a power_topology_config_t from the hskSel fields of g_powerModeTable.
 *
 * @param modeIdx  Column index in g_powerModeTable (Sleep, DS1_Irq, DS1_Dma, ..., PD, DPD).
 * @param topo     Output topology config struct.  Must not be NULL.
 */
static void APP_BuildTopologyConfig(app_standby_mode_idx_t modeIdx,
                                    power_topology_config_t *topo)
{
    uint32_t r;
    uint32_t *regs[3];
    assert(topo != NULL);
    POWER_GetDefaultTopologyConfig(topo);

    regs[0] = &topo->rcgcfgHskSel;
    regs[1] = &topo->csrccfgHskSel;
    regs[2] = &topo->csrccfgHskSel1;

    for (r = 0U; r < (uint32_t)kPOWER_Resource_COUNT; r++)
    {
        const power_mode_cell_t *cell = &g_powerModeTable[r][(uint32_t)modeIdx];
        if (!cell->valid || (cell->hskSel == 0U))
        {
            continue; /* not a topology resource or not configured for this mode */
        }
        const app_topo_field_t *f = &s_topoFieldMap[r];
        *regs[f->regIdx] = (*regs[f->regIdx] & ~(0x7UL << f->shift))
                         | ((uint32_t)cell->hskSel << f->shift);
    }
}

/*!
 * @brief Apply POWERCON topology for the given mode before calling POWER_Enter*().
 *
 * Builds a power_topology_config_t from the hskSel fields of g_powerModeTable
 * for @p modeIdx and writes it to hardware via POWER_SetTopology().  Must be
 * called after APP_Build*Config() and before POWER_Enter*().
 *
 * @param modeIdx  Standby mode column (kAPP_StandbyModeIdx_Sleep, etc.).
 */
static void APP_ApplyTopologyForMode(app_standby_mode_idx_t modeIdx)
{
    power_topology_config_t topo;
    APP_BuildTopologyConfig(modeIdx, &topo);
    POWER_SetTopology(&topo);
}

/*!
 * @brief Build a power_down_config_t from the PD column of g_powerModeTable.
 */
static void APP_BuildPowerDownConfig(power_down_config_t *cfg)
{
    assert(cfg != NULL);
    (void)memset(cfg, 0, sizeof(*cfg));
    /* Power Down retention (fixed for this demo; previously the PD column of
     * g_powerModeTable). memset already cleared the false/0 fields. */
    cfg->retainOcram0KB = APP_PD_OCRAM0_KB; /* OCRAM0 KB to retain */
    cfg->retainPkcMem   = false;
    cfg->retainLlcCache = false;            /* [EXP] */
    cfg->retainAudio8Kb = false;            /* [EXP] */
}

/*!
 * @brief Build a power_deep_power_down_config_t from the DPD column.
 *
 * DPD1 (retainVbatSram=true):  table value used as-is.
 * DPD2 (retainVbatSram=false): override applied after table read.
 *
 * @param lpType  APP_LP_DPD1 or APP_LP_DPD2.
 */
static void APP_BuildDeepPowerDownConfig(uint8_t lpType,
                                         power_deep_power_down_config_t *cfg)
{
    assert(cfg != NULL);
    (void)memset(cfg, 0, sizeof(*cfg));
    /* DPD1 retains the VBAT SRAM (VDD_PMU kept); DPD2 does not (PMIC_CTRL asserted,
     * VDD_PMU removed). Previously the DPD column of g_powerModeTable + a DPD2 override. */
    cfg->retainVbatSram = (lpType != APP_LP_DPD2);
}

/*******************************************************************************
 * Sequence execution
 ******************************************************************************/

/* Per-mode run-mode switch policy used by APP_EnterLowPower. */
typedef struct
{
    bool             preSwitch;  /*!< Switch run mode before low power entry. */
    power_run_mode_t preMode;    /*!< Target run mode before entry. */
    bool             postSwitch; /*!< Switch run mode after wake (only for modes that return). */
    power_run_mode_t postMode;   /*!< Target run mode after wake. */
} app_lp_run_cfg_t;

/* Indexed by APP_LP_* code.  Index 0 is unused (APP_LP_* values start at 1). */
static const app_lp_run_cfg_t s_lpRunCfg[] = {
    /* [0] reserved */    {false, kPOWER_RunModeNormal, false, kPOWER_RunModeNormal},
    /* [APP_LP_SLEEP]  */ {false, kPOWER_RunModeNormal, true,  kPOWER_RunModeNormal},
    /* [APP_LP_DS1]    */ {true,  kPOWER_RunModeLp,     true,  kPOWER_RunModeNormal},
    /* [APP_LP_DS2]    */ {true,  kPOWER_RunModeLp,     true,  kPOWER_RunModeNormal},
    /* [APP_LP_DS3]    */ {true,  kPOWER_RunModeLp,     true,  kPOWER_RunModeNormal},
    /* [APP_LP_PD]     */ {true,  kPOWER_RunModeLp,     false, kPOWER_RunModeNormal},
    /* [APP_LP_DPD1]   */ {true,  kPOWER_RunModeLp,     false, kPOWER_RunModeNormal},
    /* [APP_LP_DPD2]   */ {true,  kPOWER_RunModeLp,     false, kPOWER_RunModeNormal},
};

/*!
 * @brief Resolve the g_powerModeTable / topology column for (lpType, wakeupSrc).
 *
 * Only Sleep and the Deep Sleep variants are table columns. Power Down / Deep Power
 * Down are not columns and must not be resolved through this helper.
 */
static uint8_t APP_StandbyIdx(uint8_t lpType, uint8_t wakeupSrc)
{
    (void)wakeupSrc;
    switch (lpType)
    {
        case APP_LP_DS1:  return kAPP_StandbyModeIdx_DS1;
        case APP_LP_DS2:  return kAPP_StandbyModeIdx_DS2;
        case APP_LP_DS3:  return kAPP_StandbyModeIdx_DS3;
        case APP_LP_SLEEP:
        default:          return kAPP_StandbyModeIdx_Sleep;
    }
}

/*
 * Build the entry config for lpType into the union (resolving the Deep Sleep
 * standby column via APP_StandbyIdx).  Run before the wakeup is armed, since
 * iterating g_powerModeTable is relatively slow.
 */
static void APP_BuildConfig(uint8_t lpType, uint8_t wakeupSrc, app_lp_cfg_t *cfg)
{
    switch (lpType)
    {
        case APP_LP_SLEEP:
            APP_BuildSleepConfig(&cfg->sleep);
            break;
        case APP_LP_DS1:
        case APP_LP_DS2:
        case APP_LP_DS3:
            APP_BuildDeepSleepConfig((app_standby_mode_idx_t)APP_StandbyIdx(lpType, wakeupSrc),
                                     &cfg->deepSleep);
            if ((wakeupSrc == APP_WAKEUP_DMA_WAKE) || (wakeupSrc == APP_WAKEUP_DMA_SW5))
            {
                APP_ApplyDmaDeepSleepOverrides(lpType, &cfg->deepSleep);
            }
            break;
        case APP_LP_PD:
            APP_BuildPowerDownConfig(&cfg->powerDown);
            break;
        case APP_LP_DPD1:
        case APP_LP_DPD2:
            APP_BuildDeepPowerDownConfig(lpType, &cfg->deepPowerDown);
            break;
        default:
            break;
    }
}

/*
 * Enter the low power mode for lpType (the matching POWER_Enter*() only, on the
 * pre-built config).  Power Down and Deep Power Down do not return.
 */
static void APP_EnterConfig(uint8_t lpType, app_lp_cfg_t *cfg)
{
    switch (lpType)
    {
        case APP_LP_SLEEP:
            (void)POWER_EnterSleep(&cfg->sleep);
            break;
        case APP_LP_DS1:
        case APP_LP_DS2:
        case APP_LP_DS3:
            (void)POWER_EnterDeepSleep(&cfg->deepSleep);
            break;
        case APP_LP_PD:
            POWER_EnterPowerDown(&cfg->powerDown); /* does not return */
            break;
        case APP_LP_DPD1:
        case APP_LP_DPD2:
            /* Arm the VBAT_SRAM retention marker just before entry; the next boot
             * reports whether it survived (RETAINED after DPD1, lost after DPD2). */
            APP_VbatSramArm((lpType == APP_LP_DPD2) ? 2U : 1U);
            POWER_EnterDeepPowerDown(&cfg->deepPowerDown); /* does not return */
            break;
        default:
            break;
    }
}

/*!
 * @brief Low-power target handler (Sleep / DS / PD / DPD).
 *
 * The whole low-power path in one linear function: preview the mode's resources,
 * pick a wakeup source, then the common entry flow - apply topology (setup) ->
 * log -> APP_BuildConfig(&cfg) -> arm wakeup (last) -> APP_EnterConfig(&cfg) -> on
 * wake, tear down and restore.  APP_BuildConfig() fills the config; APP_EnterConfig()
 * does ONLY POWER_Enter*(); the arm sits between them (build before arm, arm
 * immediately before entry).  Per-mode flags/notes come from APP_LP_SEQ[lpType];
 * run mode switches from s_lpRunCfg[].  Power Down / Deep Power Down do not return,
 * so the post-entry tail is never reached.
 */
static void APP_EnterLowPower(const app_target_t *t)
{
    uint8_t             lpType = t->payload;
    const app_lp_seq_t *seq    = &APP_LP_SEQ[lpType];
    const char         *lpName = APP_LP_MODE_NAMES[lpType];
    uint8_t             wakeupSrc;
    app_lp_cfg_t        cfg;

    /* Level 2: pick a wakeup source. The resource preview is printed later (after the
     * config is built) so it reflects the actual resolved state. */
    PRINTF("\r\n>>> %s\r\n", t->name);
    wakeupSrc = APP_RunLevel2WakeupMenu(lpType);
    if (wakeupSrc == APP_WAKEUP_NONE)
    {
        return; /* user backed out */
    }
    APP_BeginTransition(APP_RUN_MODE_NAMES[POWER_GetCurrentRunMode()], t->shortName);

    /* Pre-entry run mode switch (LP for deep modes; none for Sleep). */
    if (s_lpRunCfg[lpType].preSwitch)
    {
        APP_SwitchRunMode(s_lpRunCfg[lpType].preMode);
    }

    /* Topology: Sleep / Deep Sleep derive routing from their g_powerModeTable column;
     * Power Down / Deep Power Down have no standby routing, so apply the default topology. */
    if ((lpType == APP_LP_PD) || (lpType == APP_LP_DPD1) || (lpType == APP_LP_DPD2))
    {
        power_topology_config_t topo;
        POWER_GetDefaultTopologyConfig(&topo);
        POWER_SetTopology(&topo);
    }
    else
    {
        APP_ApplyTopologyForMode((app_standby_mode_idx_t)APP_StandbyIdx(lpType, wakeupSrc));
    }
    /* Build the entry config now (fills cfg from g_powerModeTable + inlined PD/DPD
     * retention), then preview the ACTUAL resolved resource state while the console is
     * still alive. Build stays before the wakeup arm below (the relatively slow build
     * must not run with a timed VBAT LPTMR already counting). */
    APP_BuildConfig(lpType, wakeupSrc, &cfg);
    APP_PrintModeResources(lpType, wakeupSrc);

    if (seq->lpRunBracket)
    {
        APP_EnterLowPowerRun();
    }
    APP_LogStep(APP_RUN_MODE_NAMES[POWER_GetCurrentRunMode()], lpName, seq->enterNote);
    APP_WaitTxDone();

    if (seq->deinitConsole)
    {
        DbgConsole_Deinit();
    }
    /* Arm the wakeup source last, immediately before entry. */
    APP_SetWakeup(wakeupSrc, true);
    APP_EnterConfig(lpType, &cfg); /* Power Down / Deep Power Down do NOT return */

    /* --- Returning modes (Sleep / Deep Sleep) resume here. --- */
    if (seq->deinitConsole)
    {
        BOARD_InitDebugConsole();
    }
    if (seq->lpRunBracket)
    {
        APP_ExitLowPowerRun();
    }
    APP_SetWakeup(wakeupSrc, false);
    APP_LogWakeEvent(APP_WAKEUP_SOURCES[wakeupSrc].name);
    APP_LogStep(lpName, APP_RUN_MODE_NAMES[POWER_GetCurrentRunMode()], seq->wakeNote);

    if (s_lpRunCfg[lpType].postSwitch)
    {
        APP_SwitchRunMode(s_lpRunCfg[lpType].postMode);
    }

    PRINTF("\r\nCurrent mode: %s\r\n", APP_RUN_MODE_NAMES[POWER_GetCurrentRunMode()]);
}

/*******************************************************************************
 * Flat top-level menu
 ******************************************************************************/

/*!
 * @brief Print the flat top-level menu - one entry per supported mode.
 */
static void APP_PrintTopMenu(void)
{
    uint8_t i;
    PRINTF("\r\n");
    PRINTF("==============================================\r\n");
    PRINTF("   RT2660 Power Mode Switch Demo\r\n");
    PRINTF("   Current mode: %s\r\n", APP_RUN_MODE_NAMES[POWER_GetCurrentRunMode()]);
    PRINTF("==============================================\r\n");
    for (i = 0U; i < APP_TARGET_COUNT; i++)
    {
        if (i < 9U)
        {
            PRINTF("   %c. %s\r\n", (char)('1' + i), APP_TARGETS[i].name);
        }
        else
        {
            /* Entries 10+ shown as A, B, ... */
            PRINTF("   %c. %s\r\n", (char)('A' + (i - 9U)), APP_TARGETS[i].name);
        }
    }
    PRINTF("==============================================\r\n");
    PRINTF("  Select (1-9, A): ");
}

/*!
 * @brief Read one top-level menu keypress and map it to a mode-table index.
 *
 * Accepts '1'..'9' for entries 0..8 and 'A'/'a' for entry 9 (case-insensitive),
 * echoes the accepted key, and returns the entry index.  Returns 0xFF on any
 * invalid or out-of-range key (after printing "Invalid selection").
 */
static uint8_t APP_ReadMenuSelection(void)
{
    uint32_t ch = (uint32_t)GETCHAR();
    uint8_t  idx;

    /* Normalize lowercase to uppercase. */
    if ((ch >= (uint32_t)'a') && (ch <= (uint32_t)'z'))
    {
        ch -= (uint32_t)('a' - 'A');
    }

    /* Flat selection: '1'..'9' for entries 0..8, 'A' for entry 9. */
    if ((ch >= (uint32_t)'1') && (ch <= (uint32_t)'9'))
    {
        idx = (uint8_t)(ch - (uint32_t)'1');
    }
    else if (ch == (uint32_t)'A')
    {
        idx = 9U;
    }
    else
    {
        PRINTF("\r\n  Invalid selection.\r\n");
        return 0xFFU;
    }

    if (idx >= APP_TARGET_COUNT)
    {
        PRINTF("\r\n  Invalid selection.\r\n");
        return 0xFFU;
    }

    PRINTF("%c\r\n", (char)ch);
    return idx;
}

/*!
 * @brief Print the resolved standby-resource configuration for a mode.
 *
 * Called AFTER the config is built, so it reflects the ACTUAL resource state that will
 * be applied: for Sleep / Deep Sleep it shows the single g_powerModeTable column that
 * APP_StandbyIdx() resolved for the chosen @p wakeupSrc (IRQ vs DMA); Power Down / Deep
 * Power Down print their inlined retention config. Each cell prints the configured value
 * (raw code) or "NC"; rows that are NC are omitted. Row labels come from
 * g_powerResourceNames, the header from g_standbyModeNames. Informational only.
 */
static void APP_PrintModeResources(uint8_t lpType, uint8_t wakeupSrc)
{
    app_standby_mode_idx_t cols[1];
    uint8_t                colCount = 0U;
    uint8_t                c;
    uint32_t               r;

    /* Power Down / Deep Power Down are not g_powerModeTable columns: print the inlined
     * retention config (see APP_BuildPowerDownConfig / APP_BuildDeepPowerDownConfig). */
    if (lpType == APP_LP_PD)
    {
        PRINTF("\r\n  Configured resources for %s:\r\n", APP_LP_MODE_NAMES[lpType]);
        PRINTF("    %-26s | %u KB\r\n", "PD OCRAM0 retain", (unsigned int)APP_PD_OCRAM0_KB);
        PRINTF("    %-26s | %s\r\n",    "PD PKC RAM retain", "no");
        return;
    }
    if ((lpType == APP_LP_DPD1) || (lpType == APP_LP_DPD2))
    {
        PRINTF("\r\n  Configured resources for %s:\r\n", APP_LP_MODE_NAMES[lpType]);
        PRINTF("    %-26s | %s\r\n", "DPD VBAT SRAM retain",
               (lpType == APP_LP_DPD2) ? "no (DPD2)" : "yes (DPD1)");
        return;
    }

    switch (lpType)
    {
        case APP_LP_SLEEP:
        case APP_LP_DS1:
        case APP_LP_DS2:
        case APP_LP_DS3:
            /* Single column actually resolved for this wakeup source (IRQ vs DMA). */
            cols[colCount++] = (app_standby_mode_idx_t)APP_StandbyIdx(lpType, wakeupSrc);
            break;
        default:
            return; /* Run targets have no standby column. */
    }

    PRINTF("\r\n  Configured resources for %s:\r\n", APP_LP_MODE_NAMES[lpType]);
    PRINTF("    %-26s", "resource");
    for (c = 0U; c < colCount; c++)
    {
        PRINTF(" | %s", g_standbyModeNames[cols[c]]);
    }
    PRINTF("\r\n");

    for (r = 0U; r < (uint32_t)kPOWER_Resource_COUNT; r++)
    {
        bool anyValid = false;
        for (c = 0U; c < colCount; c++)
        {
            anyValid = anyValid || g_powerModeTable[r][cols[c]].valid;
        }
        if (!anyValid)
        {
            continue; /* Not configurable in this mode - skip. */
        }

        PRINTF("    %-26s", g_powerResourceNames[r]);
        for (c = 0U; c < colCount; c++)
        {
            const power_mode_cell_t *cell = &g_powerModeTable[r][cols[c]];
            if (cell->valid)
            {
                PRINTF(" | 0x%08X", (unsigned int)cell->value);
            }
            else
            {
                PRINTF(" | %10s", "NC");
            }
        }
        PRINTF("\r\n");
    }
}

/*!
 * @brief Run Level 2 wakeup-source sub-menu for the given low power mode.
 *
 * Displays the wakeup source list for @p lpType, reads the user's selection,
 * and returns the chosen APP_WAKEUP_* code.
 *
 * @return Selected APP_WAKEUP_* code, or APP_WAKEUP_NONE if the user cancels.
 */
static uint8_t APP_RunLevel2WakeupMenu(uint8_t lpType)
{
    uint8_t  codes[APP_WAKEUP_SOURCE_COUNT]; /* menu index -> APP_WAKEUP_* code */
    uint8_t  count = 0U;
    uint8_t  i;
    uint32_t ch;

    /* Build the list from every source whose modes bitmask includes this mode. */
    for (i = 0U; i < APP_WAKEUP_SOURCE_COUNT; i++)
    {
        if ((APP_WAKEUP_SOURCES[i].modes & APP_M(lpType)) != 0U)
        {
            codes[count++] = i; /* table is indexed by code, so i == APP_WAKEUP_* code */
        }
    }
    if (count == 0U)
    {
        return APP_WAKEUP_NONE; /* no selectable source for this mode */
    }

    PRINTF("\r\n  Wakeup source:\r\n");
    for (i = 0U; i < count; i++)
    {
        PRINTF("   %c. %s\r\n", (char)('1' + i), APP_WAKEUP_SOURCES[codes[i]].name);
    }
    PRINTF("   0. Back\r\n");
    PRINTF("  Select: ");

    ch = (uint32_t)GETCHAR();
    PRINTF("%c\r\n", (char)ch);

    if (ch == (uint32_t)'0')
    {
        return APP_WAKEUP_NONE; /* back to Level 1 */
    }

    if ((ch >= (uint32_t)'1') && (ch < (uint32_t)'1' + (uint32_t)count))
    {
        return codes[(uint8_t)(ch - (uint32_t)'1')];
    }

    PRINTF("  Invalid selection.\r\n");
    return APP_WAKEUP_NONE;
}
