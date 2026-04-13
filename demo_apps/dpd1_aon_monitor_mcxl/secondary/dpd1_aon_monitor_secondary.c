/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * DPD1 AON Monitor — CM0+ (secondary core) application.
 *
 * Demonstrates the ROM CGU-clobber workaround for DPD1 mode:
 *   1. CM0+ stays active during DPD1, printing periodic heartbeat via AON UART.
 *   2. On AON UART RX (any character), CM0+ manually triggers CM33 wakeup
 *      while protecting its own clock tree from ROM's System Init.
 *   3. The workaround: save CGU → SMM_WakeupMainDomain → wait for CM33 ready
 *      token → restore CGU.
 */

#include "app.h"
#include "board.h"
#include "fsl_clock.h"
#include "fsl_debug_console.h"
#include "fsl_lpuart.h"
#include "fsl_mu.h"
#include "fsl_power.h"
#include "fsl_smm.h"

#include "pin_mux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Snapshot of AON CGU registers preserved across CM33 ROM boot. */
typedef struct
{
    uint32_t clkConfig;
    uint32_t clockDiv;
    uint32_t perClkConfig;
    uint32_t perClkEn;
} app_cgu_snapshot_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static bool APP_SecondaryCoreCallback(power_low_power_mode_t targetPowerMode, void *ptrPowerConfig, void *userData);
static void APP_ActiveOps(void);
static void APP_DeepPowerDown1Ops(void);
static void APP_SaveCguState(app_cgu_snapshot_t *snapshot);
static void APP_RestoreCguState(const app_cgu_snapshot_t *snapshot);
static void APP_WakeupMainDomainSafely(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_MuBRxIsrHit    = false;
volatile bool g_DualCoreSynced = false;
volatile bool g_UartRxReceived = false;
uint32_t g_MuBRxMsg            = 0UL;

/*******************************************************************************
 * Code
 ******************************************************************************/

void MU_B_RX_IRQHandler(void)
{
    uint32_t msg = MU_ReceiveMsgNonBlocking(APP_MU, APP_MU_CHANNEL);

    MU_ClearStatusFlags(APP_MU, kMU_Rx0FullFlag);
    if (msg == 0UL)
    {
        __ISB();
        __DSB();
        return;
    }
    if (Power_GetMuMessageType(msg) == kPower_MsgTypeSync)
    {
        PRINTF("Syncing with CM33\r\n");
        if (Power_MuSyncCallback(msg, APP_MU_CHANNEL) == kStatus_Success)
        {
            Power_RegisterUserCallback(APP_SecondaryCoreCallback, NULL);
            g_DualCoreSynced = true;
        }
        else
        {
            PRINTF("Fail to sync with CM33\r\n");
            assert(false);
        }
    }
    else
    {
        g_MuBRxMsg    = msg;
        g_MuBRxIsrHit = true;
    }
    __ISB();
    __DSB();
}

/*
 * Callback invoked by Power_InterpretRequest BEFORE the ACK is sent to CM33.
 *
 * Note: DPD1 on CM33 does NOT use MU handshake (no Power_CM33RequestLowPowerMode
 * call in Power_EnterDeepPowerDown1).  CM0+ detects DPD1 via SMM hardware state
 * polling in APP_ActiveOps.  Therefore DPD1-specific setup is done in
 * APP_DeepPowerDown1Ops, not here.
 */
static bool APP_SecondaryCoreCallback(power_low_power_mode_t targetPowerMode, void *ptrPowerConfig, void *userData)
{
    (void)ptrPowerConfig;
    (void)userData;
    (void)targetPowerMode;

    return true;
}

/* AON LPUART RX interrupt: set flag and disable further interrupts. */
void APP_AON_LPUART_IRQHandler(void)
{
    if ((kLPUART_RxDataRegFullFlag & LPUART_GetStatusFlags(APP_AON_LPUART)) != 0U)
    {
        /* Consume the received byte. */
        (void)LPUART_ReadByte(APP_AON_LPUART);
        /* Disable RX interrupt — one character is enough to trigger wakeup. */
        LPUART_DisableInterrupts(APP_AON_LPUART, kLPUART_RxDataRegFullInterruptEnable);
        DisableIRQ(APP_AON_LPUART_IRQn);
        g_UartRxReceived = true;
    }
    SDK_ISR_EXIT_BARRIER;
}

/* ---- CGU save / restore ------------------------------------------------- */

static void APP_SaveCguState(app_cgu_snapshot_t *snapshot)
{
    snapshot->clkConfig    = AON__CGU->CLK_CONFIG;
    snapshot->clockDiv     = AON__CGU->CLOCK_DIV;
    snapshot->perClkConfig = AON__CGU->PER_CLK_CONFIG;
    snapshot->perClkEn     = AON__CGU->PER_CLK_EN;
}

static void APP_RestoreCguState(const app_cgu_snapshot_t *snapshot)
{
    /* Re-enable FROs first (ROM may have disabled them). */
    AON__CGU->CLK_CONFIG = snapshot->clkConfig;
    AON__CGU->CLOCK_DIV  = snapshot->clockDiv;

    /* Reconfigure FRO to the correct frequency before restoring
     * peripheral clocks that depend on it. */
    CLOCK_SetupFROAonClocking(3000000U);
    SystemCoreClock = 3000000U;

    AON__CGU->PER_CLK_CONFIG = snapshot->perClkConfig;
    AON__CGU->PER_CLK_EN     = snapshot->perClkEn;
}

/*
 * Safely wake the Main domain while protecting the AON clock tree.
 *
 * Flow:
 *   1. Save CGU registers.
 *   2. Pulse WKUP_CPU_M → Main domain powers on → CM33 ROM runs
 *      (ROM System Init clobbers AON CGU here).
 *   3. Power_NotifyCM33ToRun() — waits for the fsl_power.c BCKP1
 *      sync signal, then writes dualCoreSynced to unblock CM33.
 *   4. Restore CGU → UART and other AON peripherals resume normally.
 */
static void APP_WakeupMainDomainSafely(void)
{
    app_cgu_snapshot_t cguSnapshot;

    /* Step 1: save CGU state before ROM can touch it. */
    APP_SaveCguState(&cguSnapshot);

    /* Step 2: trigger Main domain power-up. */
    PRINTF("Triggering CM33 wakeup...\r\n");
    SMM_WakeupMainDomain(AON__SMM);

    /* Step 3: synchronize with CM33 via the power driver.
     * Power_NotifyCM33ToRun spins until CM33 writes the BCKP1
     * sync signal (after ROM System Init + context restore),
     * then sets dualCoreSynced to let CM33 proceed. */
    Power_NotifyCM33ToRun();

    /* Step 4: restore CGU — UART output resumes correctly. */
    APP_RestoreCguState(&cguSnapshot);

    PRINTF("CGU restored — CM33 is ready\r\n");
}

/* ---- Main ---------------------------------------------------------------- */

int main(void)
{
    power_low_power_mode_t curPowerMode = kPower_Active;

    BOARD_InitHardware();
    PRINTF("\r\n### DPD1 AON Monitor — CM0+ Boot ###\r\n");

    EnableIRQ(MU_B_RX_IRQn);
    MU_EnableInterrupts(APP_MU, kMU_Rx0FullInterruptEnable);

    /* Wait for dual-core sync with CM33. */
    while (g_DualCoreSynced == false)
    {
    }
    g_DualCoreSynced = false;

    while (1)
    {
        if (Power_GetCurrentPowerMode(&curPowerMode) == kStatus_Success)
        {
            if (curPowerMode == kPower_Active)
            {
                APP_ActiveOps();
            }
            else if (curPowerMode == kPower_DeepPowerDown1)
            {
                APP_DeepPowerDown1Ops();
            }
        }
    }
}

/* ---- Active mode: wait for CM33 power request --------------------------- */

static void APP_ActiveOps(void)
{
    power_low_power_mode_t curLpMode;
    MU_EnableInterrupts(APP_MU, kMU_Rx0FullInterruptEnable);
    EnableIRQ(MU_B_RX_IRQn);

    while (g_MuBRxIsrHit == false)
    {
        (void)Power_GetCurrentPowerMode(&curLpMode);
        if (curLpMode != kPower_Active)
        {
            return;
        }
    }

    PRINTF("Interpreting CM33 request\r\n");
    DisableIRQ(MU_B_RX_IRQn);
    uint32_t key = DisableGlobalIRQ();
    Power_InterpretRequest(g_MuBRxMsg);
    g_MuBRxMsg    = 0UL;
    g_MuBRxIsrHit = false;
    EnableGlobalIRQ(key);
    EnableIRQ(MU_B_RX_IRQn);
}

/* ---- DPD1 mode: periodic log + UART RX interrupt wakeup ----------------- */

static void APP_DeepPowerDown1Ops(void)
{
    power_dpd1_transition_t nextTrans = Power_GetDeepPowerDown1NextTransition();
    MU_DisableInterrupts(APP_MU, kMU_Rx0FullInterruptEnable);
    DisableIRQ(MU_B_RX_IRQn);

    if (nextTrans != kPower_Dpd1ToActive)
    {
        PRINTF("Unsupported DPD1 transition\r\n");
        assert(false);
    }

    /* Enable AON LPUART RX IRQ in CM0+ NVIC.
     * CM33 already enabled the peripheral-level RX interrupt before DPD1 entry;
     * CM0+ now arms its own NVIC to receive it. */
    g_UartRxReceived = false;
    EnableIRQ(APP_AON_LPUART_IRQn);

    PRINTF("CM0+ active in DPD1 — monitoring AON UART RX\r\n");
    PRINTF("Send any character to wake CM33...\r\n\r\n");

    uint32_t heartbeat = 0U;

    while (1)
    {
        PRINTF("[DPD1] Heartbeat #%d  (CM0+ alive, CM33 Powered Off)\r\n", heartbeat++);

        /* Wait ~APP_LOG_INTERVAL_MS in small steps, checking RX flag. */
        for (uint32_t ms = 0U; ms < APP_LOG_INTERVAL_MS; ms += 10U)
        {
            if (g_UartRxReceived)
            {
                break;
            }
            SDK_DelayAtLeastUs(10000U, SystemCoreClock);
        }

        if (g_UartRxReceived)
        {
            PRINTF("\r\n[DPD1] UART RX detected — waking CM33 (with CGU workaround)\r\n");

            /* === ROM CGU-clobber workaround === */
            APP_WakeupMainDomainSafely();

            PRINTF("[DPD1] CM33 wakeup complete — returning to Active mode\r\n");
            return;
        }
    }
}
