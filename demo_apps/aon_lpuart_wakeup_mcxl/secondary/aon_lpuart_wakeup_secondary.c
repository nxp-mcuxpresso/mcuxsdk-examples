/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_clock.h"
#include "fsl_debug_console.h"
#include "fsl_mu.h"
#include "fsl_power.h"
#include "fsl_smm.h"

#include "fsl_lpuart.h"

#include "pin_mux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
static const char *const g_modeNameArray[] = {
    "Sleep",
    "Deep Sleep",
    "Power Down 1",
    "Power Down 2",
    "Deep Power Down 1",
    "Deep Power Down 2",
    "Deep Power Down 3",
    "Shut Down",
    "Active",
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static bool APP_SecondaryCoreCallback(power_low_power_mode_t targetPowerMode, void *ptrPowerConfig, void *userData);
static void APP_ActiveOps(void);
static void APP_DeepPowerDown1Ops(void);
static void APP_DPD1ToActive(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_MuBRxIsrHit    = false;
volatile bool g_DualCoreSynced = false;
uint32_t g_MuBRxMsg            = 0UL;

/*******************************************************************************
 * Code
 ******************************************************************************/

void APP_AON_LPUART_IRQ_HANDLER(void)
{
    /* Disable NVIC IRQ to prevent re-entry.  Do NOT read the byte —
     * RDRF must stay asserted so the SMM wakeup controller can detect
     * the LPUART interrupt and complete the main-domain power-up. */
    DisableIRQ(APP_AON_LPUART_IRQn);
}

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

static bool APP_SecondaryCoreCallback(power_low_power_mode_t targetPowerMode, void *ptrPowerConfig, void *userData)
{
    PRINTF("CM33 requests power mode: %s\r\n", g_modeNameArray[(uint8_t)targetPowerMode]);

    if ((targetPowerMode == kPower_PowerDown2) || (targetPowerMode == kPower_DeepPowerDown2))
    {
        /* PD2/DPD2: CM0+ enters WFI in InterpretRequest.  Ensure LPUART
         * peripheral RX interrupt and NVIC IRQ are enabled so incoming
         * character wakes CM0+ from WFI. */
        LPUART_EnableInterrupts(APP_AON_LPUART, kLPUART_RxDataRegFullInterruptEnable);
        EnableIRQ(APP_AON_LPUART_IRQn);
        MU_DisableInterrupts(APP_MU, kMU_Rx0FullInterruptEnable);
    }

    (void)ptrPowerConfig;
    (void)userData;
    return true;
}

int main(void)
{
    power_low_power_mode_t curPowerMode = kPower_Active;
    power_low_power_mode_t prePowerMode = kPower_DeepPowerDown2;

    BOARD_InitHardware();

    PRINTF(
        "\r\n###########################  AON LPUART Wakeup Demo (CM0+)  "
        "###########################\r\n");
    PRINTF("Core Clock Frequency: %d\r\n", CLOCK_GetAonCoreSysClkFreq());
    EnableIRQ(MU_B_RX_IRQn);
    MU_EnableInterrupts(APP_MU, kMU_Rx0FullInterruptEnable);

    while (g_DualCoreSynced == false)
    {
    }
    g_DualCoreSynced = false;

    Power_ClearLpPowerSettings();

    while (1)
    {
        if (Power_GetCurrentPowerMode(&curPowerMode) == kStatus_Success)
        {
            if (curPowerMode == kPower_Active)
            {
                PRINTF("System Is In Active Mode!\r\n");
                APP_ActiveOps();
            }
            else if (curPowerMode == kPower_DeepPowerDown1)
            {
                PRINTF("System Is In Deep Power Down1 Mode!\r\n");
                APP_DeepPowerDown1Ops();
            }
            else
            {
                if (prePowerMode != curPowerMode)
                {
                    PRINTF("System Is In %s Mode!\r\n", g_modeNameArray[(uint8_t)curPowerMode]);
                }
                prePowerMode = curPowerMode;
            }
        }
    }
}

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

    PRINTF("Start Interpret Request\r\n");
    DisableIRQ(MU_B_RX_IRQn);
    uint32_t key = DisableGlobalIRQ();
    Power_InterpretRequest(g_MuBRxMsg);
    g_MuBRxMsg    = 0UL;
    g_MuBRxIsrHit = false;
    EnableGlobalIRQ(key);
    EnableIRQ(MU_B_RX_IRQn);
}

static void APP_DeepPowerDown1Ops(void)
{
    power_dpd1_transition_t nextTrans = Power_GetDeepPowerDown1NextTransition();
    MU_DisableInterrupts(APP_MU, kMU_Rx0FullInterruptEnable);
    DisableIRQ(MU_B_RX_IRQn);

    if (nextTrans == kPower_Dpd1ToActive)
    {
        APP_DPD1ToActive();
    }
    else
    {
        PRINTF("Unsupported DPD1 transition: %d\r\n", (int)nextTrans);
        assert(false);
    }
}

static void APP_DPD1ToActive(void)
{
    /* In DPD1, the AON LPUART stays alive — we intentionally do NOT deinit
     * the debug console so it can serve as the wakeup source. */
    PRINTF("Waiting for AON LPUART character to wakeup from DPD1...\r\n");
    LPUART_EnableInterrupts(APP_AON_LPUART, kLPUART_RxDataRegFullInterruptEnable);
    EnableIRQ(APP_AON_LPUART_IRQn);
    __WFI();

    PRINTF("Woken from DPD1!\r\n");
}
