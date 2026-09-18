/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_vbatcon.h"
#include "fsl_lptmr.h"
#include "fsl_lpuart.h"
#include "fsl_irtc.h"
#include "fsl_edma.h"
#include "fsl_resetcon.h"
#include "fsl_debug_console.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
/*
 * Bring the VBAT retention SRAM (VBAT__SRAM) up to ACTIVE so the application can
 * use it and it is retained across Deep Power Down 1.  Enable read/write access,
 * then follow the RM power-up sequence SHUTDOWN (SD) -> SHUTDOWN_POWER_UP (SD PU)
 * -> ACTIVE (ACT): PWR_MODE (CTRL[5:2]) must not jump straight to ACTIVE.  Lock
 * bits (CTRL[26:16]) are left clear so the mode stays changeable.
 * [hw-defer] pending silicon confirmation of the VBAT SRAM power sequence.
 */
void BOARD_InitVbatSram(void)
{
    uint32_t ctrl;
    VBAT__SRAM->CTRL |= 0x3U; /* RAM_RD_EN (bit 0) | RAM_WR_EN (bit 1) */
    ctrl             = VBAT__SRAM->CTRL & ~(uint32_t)SRAM_CTRL_PWR_MODE_MASK; /* SHUTDOWN (SD = 0) */
    VBAT__SRAM->CTRL = ctrl;
    VBAT__SRAM->CTRL = ctrl | SRAM_CTRL_PWR_MODE(4U);                         /* SHUTDOWN_POWER_UP (SD PU) */
    VBAT__SRAM->CTRL = ctrl | SRAM_CTRL_PWR_MODE(6U);                         /* ACTIVE (ACT)              */
}

/*******************************************************************************
 * VBAT_SRAM retention self-check (DPD1 only)
 *
 * DPD1 wake is a PoR, so retention is verified across the reset: arm a marker +
 * pattern in VBAT_SRAM (0x4630_0000..0x4630_FFFF) just before entering DPD1, then
 * report on the next boot. DPD1 keeps VDD_PMU so the marker survives. DPD2 removes
 * VDD_PMU (POWER_ShutdownVbatSram() powers the SRAM down) so it is never armed or
 * checked for that mode - Sleep/DS/PD do not reset the CPU so main() (and this
 * check) never re-runs after them.
 *
 * Accessed via the absolute address (not a linker section). This assumes the
 * linker maps NO section into VBAT_SRAM; otherwise C startup (.bss zeroing) would
 * clobber the marker and even DPD1 would read as "not retained".
 ******************************************************************************/
#define APP_VBAT_SRAM_BASE  0x46300000UL
#define APP_VBAT_TEST_MAGIC 0x5A5A600DUL

static volatile uint32_t *const s_vbatSram = (volatile uint32_t *)APP_VBAT_SRAM_BASE;

void BOARD_EnableVbatSram(void)
{
    s_vbatSram[0] = APP_VBAT_TEST_MAGIC;
    for (uint32_t i = 1U; i < 32U; i++)
    {
        s_vbatSram[i] = 0xC0DE0000UL + i;
    }
}

void BOARD_VbatSramReport(void)
{
    if (s_vbatSram[0] != APP_VBAT_TEST_MAGIC)
    {
        PRINTF("VBAT_SRAM check: no valid marker -> NOT retained\r\n");
        return;
    }

    bool ok = true;
    for (uint32_t i = 1U; i < 32U; i++)
    {
        if (s_vbatSram[i] != (0xC0DE0000UL + i))
        {
            ok = false;
            break;
        }
    }
    PRINTF("VBAT_SRAM check: marker found -> %s\r\n", ok ? "RETAINED" : "CORRUPTED");
    /* Consume the marker so a later cold boot is not misreported as retained. */
    s_vbatSram[0] = 0U;
}

/*
 * VBATCON GPR[0] / GPR[1] boot-context markers (see app.h). VBATCON_GPR_COUNT
 * on this device is 2, so both markers fit in the array; VBATCON_Init/Deinit
 * never touch GPRs, and the VBAT domain is always powered, so both survive
 * every Power Down / Deep Power Down PoR.
 */
#define BOARD_VBAT_GPR_DPD_VARIANT    0U
#define BOARD_VBAT_GPR_ENABLED_WAKEUP 1U

void BOARD_SetDpdVariantMarker(uint32_t variant)
{
    VBATCON_WriteGPR(VBAT__VBATCON, BOARD_VBAT_GPR_DPD_VARIANT, variant);
}

uint32_t BOARD_GetAndClearDpdVariantMarker(void)
{
    uint32_t variant = VBATCON_ReadGPR(VBAT__VBATCON, BOARD_VBAT_GPR_DPD_VARIANT);
    VBATCON_WriteGPR(VBAT__VBATCON, BOARD_VBAT_GPR_DPD_VARIANT, BOARD_DPD_VARIANT_DPD2);
    return variant;
}

void BOARD_SetEnabledWakeupMarker(uint8_t wakeupSrc)
{
    VBATCON_WriteGPR(VBAT__VBATCON, BOARD_VBAT_GPR_ENABLED_WAKEUP, (uint32_t)wakeupSrc);
}

uint8_t BOARD_GetAndClearEnabledWakeupMarker(void)
{
    uint32_t wakeupSrc = VBATCON_ReadGPR(VBAT__VBATCON, BOARD_VBAT_GPR_ENABLED_WAKEUP);
    VBATCON_WriteGPR(VBAT__VBATCON, BOARD_VBAT_GPR_ENABLED_WAKEUP, 0xFFU);
    return (uint8_t)wakeupSrc;
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
 *
 * @return true if this boot was a Deep Power Down wake (DPD1 or DPD2 - the reset
 *         source does not distinguish the two); false otherwise (Power Down wake or
 *         a cold / non-low-power reset). Callers use this to gate DPD1-only recovery
 *         steps such as the VBAT_SRAM retention check.
 ******************************************************************************/
bool BOARD_HandleWakeupAfterReset(void)
{
    /* RSTSTAT holds only the latest reset cause, and a DPD wake is immediately
     * followed by a PIN_RESET that overwrites it. RSTSTAT_STICKY accumulates every
     * cause since it was last cleared, so it still shows WU_VBAT/WU_PD. */
    uint32_t sticky = RESETCON_GetStickyResetStatus(SYSCON__RESETCON);
    bool fromDpd    = (sticky & (uint32_t)kRESETCON_SrcWuVbat) != 0U;
    bool fromPd     = (sticky & (uint32_t)kRESETCON_SrcWuPd) != 0U;

    if (!fromDpd && !fromPd)
    {
        return false; /* Cold / non-low-power reset: nothing latched to acknowledge. */
    }

    PRINTF("Boot from %s wakeup (RSTSTAT_STICKY=0x%08X)\r\n", fromDpd ? "Deep Power Down" : "Power Down",
           (unsigned int)sticky);

    /* Which source was enabled for this entry (recorded in a VBATCON GPR before
     * POWER_Enter*() - see power_mode_switch.c's APP_EnableWakeup()). This is the
     * authoritative wakeup reason; the peripheral status flags below are not
     * used to gate it, since the LPTMR's flag does not reliably survive a DPD wake. */
    uint8_t enabledSrc = BOARD_GetAndClearEnabledWakeupMarker();

    PRINTF("Wakeup Reason: %s\r\n", APP_WAKEUP_SOURCES[enabledSrc].name);

    /* VBAT LPTMR compare flag. Ungate its clock first so the register read is safe
     * even if this wake was not the LPTMR (VBATCON is retained, so this is cheap). */
    VBATCON_EnableLPTMRClockGate(VBAT__VBATCON, false);
    if ((LPTMR_GetStatusFlags(APP_VBAT_LPTMR_BASE) & (uint32_t)kLPTMR_TimerCompareFlag) != 0U)
    {
        LPTMR_ClearStatusFlags(APP_VBAT_LPTMR_BASE, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(APP_VBAT_LPTMR_BASE);
    }

    /* IRTC alarm flag (W1C; unlock write protection as the RTC ISR does). */
    uint32_t rtcFlags = IRTC_GetStatusFlags(APP_RTC_BASE);
    if (rtcFlags != 0U)
    {
        IRTC_SetWriteProtection(APP_RTC_BASE, false);
        IRTC_ClearStatusFlags(APP_RTC_BASE, rtcFlags);
    }

    /* Clear the latched VBAT power-event flags and the sticky reset status so a
     * stale latch does not block the next low-power entry. */
    PRINTF("  VBATCON EVENTS=0x%08X\r\n", (unsigned int)VBAT__VBATCON->EVENTS);
    VBATCON_ClearEventFlags(VBAT__VBATCON, (uint32_t)kVBATCON_EventAll);
    RESETCON_ClearStickyResetStatus(SYSCON__RESETCON, (uint32_t)kRESETCON_SrcAll);
    return fromDpd;
}

void BOARD_InitHardware(void)
{
    BOARD_CommonSetting();
    BOARD_ResetMPU();

    power_policy_config_t powerPolicyCfg;
    POWER_GetDefaultPolicyConfig(&powerPolicyCfg);
    powerPolicyCfg.handshakeRouting = &g_handshakeRoutingCfg;
    POWER_SetPolicy(&powerPolicyCfg);
}

/*******************************************************************************
 * Per-source wakeup enable/disable functions
 *
 * Each pair owns the FULL arm/disarm for its source: peripheral-specific bits
 * plus EnableIRQ/DisableIRQ + POWER_Enable/DisableWakeupSource. Kept together
 * here (rather than split with a generic dispatcher applying the NVIC/POWERCON
 * half from a single irq/powerSrc table field) because some sources - the DMA
 * variants below - need more than one IRQ/POWERCON registration, which a
 * single-pair table field cannot represent; a per-source function has no such
 * limit.
 ******************************************************************************/

/*
 * GPIO wakeup config shared by both buttons.  SW6 = WAKE GPIO (WAKE domain) and
 * SW5 = VBAT/AON GPIO use the same dsc_rgpio channel-routed model: assign the pin
 * to CH0 and arm either edge (robust to the pad's idle level).
 */
static void BOARD_EnableGpioWakeup(GPIO_Type *base, uint8_t pin)
{
    GPIO_PinClearInterruptFlag(base, pin);
    GPIO_SetPinInterruptChannel(base, pin, kGPIO_InterruptOutput0);
    GPIO_SetPinInterruptConfig(base, pin, kGPIO_InterruptEitherEdge);
}

static void BOARD_DisableGpioWakeup(GPIO_Type *base, uint8_t pin)
{
    GPIO_SetPinInterruptConfig(base, pin, kGPIO_InterruptStatusFlagDisabled);
}

void BOARD_EnableWakeGpio(void)
{
    IOMUXC_SetPin_Mux_Config(IOMUXC_PIO1_0_WAKE_GPIO0_GPIO0, 0xA0U);

    /* Configure both wakeup buttons as GPIO inputs (SW6 = WAKE pin 0).
      GPIO_PinInit also enables the WAKE GPIO port clock. */
    gpio_pin_config_t btnConfig = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic  = 0U,
    };
    GPIO_PinInit(BOARD_USER_BUTTON_6_GPIO, BOARD_USER_BUTTON_6_GPIO_PIN, &btnConfig);

    BOARD_EnableGpioWakeup(APP_WAKE_GPIO, APP_WAKE_GPIO_PIN);
    EnableIRQ(APP_WAKE_GPIO_IRQ);
    POWER_EnableWakeupSource(kPOWER_WakeupIrq_WakeGpioCh0);
}

void BOARD_DisableWakeGpio(void)
{
    DisableIRQ(APP_WAKE_GPIO_IRQ);
    POWER_DisableWakeupSource(kPOWER_WakeupIrq_WakeGpioCh0);
    BOARD_DisableGpioWakeup(APP_WAKE_GPIO, APP_WAKE_GPIO_PIN);

    GPIO_PortDenit(BOARD_USER_BUTTON_6_GPIO);
    IOMUXC_SetPin_Mux_Config(IOMUXC_PIO1_0_WAKE_GPIO0_GPIO0, 0x00);
}

void BOARD_EnableAonGpio(void)
{
    IOMUXC_SetPin_Mux_Config(IOMUXC_PIO0_4_VBAT_GPIO0_GPIO4, 0xA0U);

    /* Configure both wakeup buttons as GPIO inputs (SW5 = VBAT pin 4).
     * GPIO_PinInit also enables the WAKE GPIO port clock. */
    gpio_pin_config_t btnConfig = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic  = 0U,
    };
    GPIO_PinInit(BOARD_USER_BUTTON_GPIO, BOARD_USER_BUTTON_GPIO_PIN, &btnConfig);

    BOARD_EnableGpioWakeup(APP_AON_GPIO, APP_AON_GPIO_PIN);
    EnableIRQ(APP_AON_GPIO_IRQ);
    POWER_EnableWakeupSource(kPOWER_WakeupIrq_VbatGpioCh0);
}

void BOARD_DisableAonGpio(void)
{
    DisableIRQ(APP_AON_GPIO_IRQ);
    POWER_DisableWakeupSource(kPOWER_WakeupIrq_VbatGpioCh0);
    BOARD_DisableGpioWakeup(APP_AON_GPIO, APP_AON_GPIO_PIN);

    GPIO_PortDenit(BOARD_USER_BUTTON_GPIO);
    IOMUXC_SetPin_Mux_Config(IOMUXC_PIO0_4_VBAT_GPIO0_GPIO4, 0x00);
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
void BOARD_EnableLptmr(void)
{
    lptmr_config_t cfg;

    VBATCON_EnableLPTMRClockGate(VBAT__VBATCON, false);
    (void)VBATCON_SwitchUlp32kSource(VBAT__VBATCON, kVBATCON_Ulp32kSourceFro32k, 0xFFFFU);

    /* Full reset before re-arming: after a DPD wake (a PoR) the normal Sleep/DS
     * tear-down (BOARD_DisableLptmr) never runs, leaving this VBAT LPTMR in a "used"
     * state where a later LPTMR_Init/StartTimer fails to re-set CSR.TEN (observed:
     * 2nd DPD entry LPTMR CSR=0x40, TEN=0 -> never fires -> no wake). Deinit first to
     * force a clean start every arm, then re-ungate (Deinit gates the clock). */
    LPTMR_Deinit(APP_VBAT_LPTMR_BASE);
    VBATCON_EnableLPTMRClockGate(VBAT__VBATCON, false);

    LPTMR_GetDefaultConfig(&cfg);
    cfg.timerMode = kLPTMR_TimerModeTimeCounter;
    /* Prescaler bypassed (driver default), so one count == one ULP32K tick. */
    LPTMR_Init(APP_VBAT_LPTMR_BASE, &cfg);
    LPTMR_SetTimerPeriod(APP_VBAT_LPTMR_BASE, MSEC_TO_COUNT(APP_LPTMR_TIMEOUT_MS, APP_LPTMR_CLK_HZ));
    LPTMR_EnableInterrupts(APP_VBAT_LPTMR_BASE, kLPTMR_TimerInterruptEnable);
    LPTMR_StartTimer(APP_VBAT_LPTMR_BASE);

    EnableIRQ(APP_VBAT_LPTMR_IRQ);
    POWER_EnableWakeupSource(kPOWER_WakeupIrq_VbatLptmr);
}

void BOARD_DisableLptmr(void)
{
    DisableIRQ(APP_VBAT_LPTMR_IRQ);
    POWER_DisableWakeupSource(kPOWER_WakeupIrq_VbatLptmr);
    LPTMR_StopTimer(APP_VBAT_LPTMR_BASE);
    LPTMR_Deinit(APP_VBAT_LPTMR_BASE);
}

/*
 * HSP LPUART0 RX wakeup config.  RX-data-full fires in Sleep (clock on); the RX
 * active edge is the clock-less path that wakes Deep Sleep (functional clock
 * gated) - both are armed so the same source works in Sleep and Deep Sleep.  On
 * disarm the wake character(s) are drained so the menu does not consume them.
 */
void BOARD_EnableUart(void)
{
    /* Clear any stale RX active-edge flag before arming. */
    LPUART_ClearStatusFlags((LPUART_Type *)APP_UART_BASEADDR, kLPUART_RxActiveEdgeFlag);
    LPUART_EnableInterrupts((LPUART_Type *)APP_UART_BASEADDR, kLPUART_RxActiveEdgeInterruptEnable);
    EnableIRQ(APP_UART_IRQ);
    POWER_EnableWakeupSource(kPOWER_WakeupIrq_HspLpuart0);
}

void BOARD_DisableUart(void)
{
    DisableIRQ(APP_UART_IRQ);
    POWER_DisableWakeupSource(kPOWER_WakeupIrq_HspLpuart0);
    LPUART_DisableInterrupts((LPUART_Type *)APP_UART_BASEADDR, kLPUART_RxActiveEdgeInterruptEnable);

    if ((LPUART_GetStatusFlags((LPUART_Type *)APP_UART_BASEADDR) & (uint32_t)kLPUART_RxDataRegFullFlag) != 0U)
    {
        (void)LPUART_ReadByte((LPUART_Type *)APP_UART_BASEADDR);
    }
    LPUART_ClearStatusFlags((LPUART_Type *)APP_UART_BASEADDR, kLPUART_RxActiveEdgeFlag | kLPUART_RxOverrunFlag);
}

/*
 * VBAT IRTC alarm wakeup.  On arm: init the IRTC, set a fixed base datetime
 * (midnight) and an alarm APP_RTC_ALARM_OFFSET_S seconds later, and enable the
 * alarm interrupt.  On disarm: disable it.  The alarm asserts VBAT_RTC_IRQn (an
 * NVIC IRQ for Sleep/DS/PD; the VBATCON wakeup request for DPD).
 * [hw-defer] pending silicon confirmation of the VBAT IRTC clock source.
 */
void BOARD_EnableRtc(void)
{
    irtc_config_t cfg;
    irtc_datetime_t base = {
        .year = 2026U, .month = 1U, .day = 1U, .weekDay = 1U, .hour = 0U, .minute = 0U, .second = 0U};
    irtc_datetime_t alarm = {.year    = 2026U,
                             .month   = 1U,
                             .day     = 1U,
                             .weekDay = 1U,
                             .hour    = (uint8_t)(APP_RTC_ALARM_OFFSET_S / 3600U),
                             .minute  = (uint8_t)((APP_RTC_ALARM_OFFSET_S / 60U) % 60U),
                             .second  = (uint8_t)(APP_RTC_ALARM_OFFSET_S % 60U)};

    IRTC_GetDefaultConfig(&cfg);
    (void)IRTC_Init(APP_RTC_BASE, &cfg);
    IRTC_SetDatetime(APP_RTC_BASE, &base);
    IRTC_SetAlarm(APP_RTC_BASE, &alarm);
    IRTC_EnableInterrupts(APP_RTC_BASE, kIRTC_AlarmInterruptEnable);

    EnableIRQ(APP_RTC_IRQ);
    POWER_EnableWakeupSource(kPOWER_WakeupIrq_VbatRtc);
}

void BOARD_DisableRtc(void)
{
    DisableIRQ(APP_RTC_IRQ);
    POWER_DisableWakeupSource(kPOWER_WakeupIrq_VbatRtc);
    IRTC_DisableInterrupts(APP_RTC_BASE, kIRTC_AlarmInterruptEnable);
}

/*
 * "DMA (wake on complete)": run the transfer and arm both wakeup stages -
 * kPOWER_WakeupWakeDma_Lptmr0 (runs the DMA) and kPOWER_WakeupIrq_WakeEdma3Ch0
 * (wakes the CPU on completion). See app.h for why both are needed.
 */
void BOARD_EnableDmaWake(void)
{
    edma_config_t edmaCfg;
    edma_transfer_config_t xfer;
    lptmr_config_t lpCfg;
    uint8_t *src = (uint8_t *)APP_DMA_SRC_ADDR;

    clock_root_config_t wakeBusRootCfg;
    wakeBusRootCfg.mux = kCLOCK_WAKEBUS_ClockRoot_FRO_192M;
    wakeBusRootCfg.div = 10U;
    CLOCK_SetRootClock(kCLOCK_Root_CGU_WAKEBUS_ROOTCLK, &wakeBusRootCfg);

    EnableIRQ(APP_WAKE_EDMA_IRQ);
    POWER_EnableWakeupSource(kPOWER_WakeupWakeDma_Lptmr0);
    POWER_EnableWakeupSource(kPOWER_WakeupIrq_WakeEdma3Ch0);

    /* Seed the source buffer so the copy moves observable data. */
    for (uint32_t i = 0U; i < APP_DMA_XFER_BYTES; i++)
    {
        src[i] = (uint8_t)(i + 1U);
    }

    EDMA_GetDefaultConfig(&edmaCfg);
    EDMA_Init(APP_WAKE_EDMA_BASE, &edmaCfg);
    EDMA_SetChannelMux(APP_WAKE_EDMA_BASE, APP_WAKE_EDMA_CHANNEL, APP_WAKE_EDMA_REQ_LPTMR0);

    EDMA_PrepareTransfer(&xfer, (void *)APP_DMA_SRC_ADDR, sizeof(uint32_t), (void *)APP_DMA_DST_ADDR, sizeof(uint32_t),
                         APP_DMA_XFER_BYTES, APP_DMA_XFER_BYTES, kEDMA_MemoryToMemory);
    EDMA_SetTransferConfig(APP_WAKE_EDMA_BASE, APP_WAKE_EDMA_CHANNEL, &xfer, NULL);
    EDMA_EnableChannelInterrupts(APP_WAKE_EDMA_BASE, APP_WAKE_EDMA_CHANNEL, (uint32_t)kEDMA_MajorInterruptEnable);
    /* LPTMR clock is async to the eDMA bus clock -> open the async gate too. */
    EDMA_EnableAsyncRequest(APP_WAKE_EDMA_BASE, APP_WAKE_EDMA_CHANNEL, true);
    EDMA_EnableChannelRequest(APP_WAKE_EDMA_BASE, APP_WAKE_EDMA_CHANNEL);

    /* WAKE_LPTMR_0's first compare raises the one DMA request that moves the
     * whole transfer (bytesEachRequest == transferBytes below), completing the
     * channel immediately - no need for the timer to keep running afterward. */
    CLOCK_SetRootClockMux(kCLOCK_Root_WAKE_lptmr0_fclk, kCLOCK_LPTMR0_ClockRoot_LP1M_WAKE);
    LPTMR_GetDefaultConfig(&lpCfg);
    lpCfg.timerMode         = kLPTMR_TimerModeTimeCounter;
    lpCfg.enableFreeRunning = false;
    LPTMR_Init(APP_WAKE_LPTMR_BASE, &lpCfg);
    LPTMR_SetTimerPeriod(APP_WAKE_LPTMR_BASE, MSEC_TO_COUNT(APP_WAKE_LPTMR_DMA_MS, 1000000));
    LPTMR_EnableTimerDMA(APP_WAKE_LPTMR_BASE, true);
    LPTMR_StartTimer(APP_WAKE_LPTMR_BASE);
}

void BOARD_DisableDmaWake(void)
{
    DisableIRQ(APP_WAKE_EDMA_IRQ);
    POWER_DisableWakeupSource(kPOWER_WakeupIrq_WakeEdma3Ch0);
    POWER_DisableWakeupSource(kPOWER_WakeupWakeDma_Lptmr0);

    LPTMR_StopTimer(APP_WAKE_LPTMR_BASE);
    LPTMR_EnableTimerDMA(APP_WAKE_LPTMR_BASE, false);
    LPTMR_Deinit(APP_WAKE_LPTMR_BASE);
    EDMA_ClearChannelStatusFlags(APP_WAKE_EDMA_BASE, APP_WAKE_EDMA_CHANNEL,
                                 (uint32_t)kEDMA_InterruptFlag | (uint32_t)kEDMA_DoneFlag);
    EDMA_Deinit(APP_WAKE_EDMA_BASE);
}

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
        GPIO_SetPinInterruptConfig(APP_WAKE_GPIO, APP_WAKE_GPIO_PIN, kGPIO_InterruptStatusFlagDisabled);
        GPIO_PinClearInterruptFlag(APP_WAKE_GPIO, APP_WAKE_GPIO_PIN);
    }
    SDK_ISR_EXIT_BARRIER;
}

void APP_AON_GPIO_IRQ_HANDLER(void)
{
    /* SW5 = VBAT/AON GPIO (dsc_rgpio); same channel-routed IRQ model as WAKE GPIO. */
    if (GPIO_PinGetInterruptFlag(APP_AON_GPIO, APP_AON_GPIO_PIN) != 0U)
    {
        GPIO_SetPinInterruptConfig(APP_AON_GPIO, APP_AON_GPIO_PIN, kGPIO_InterruptStatusFlagDisabled);
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
     * path (BOARD_EnableLptmr/BOARD_DisableLptmr) owns starting/stopping the timer. */
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
    /* WAKE_EDMA3 CH0 transfer complete: this is the wakeup - clear DONE / interrupt flags. */
    EDMA_ClearChannelStatusFlags(APP_WAKE_EDMA_BASE, APP_WAKE_EDMA_CHANNEL,
                                 (uint32_t)kEDMA_InterruptFlag | (uint32_t)kEDMA_DoneFlag);
    SDK_ISR_EXIT_BARRIER;
}
/*${function:end}*/
