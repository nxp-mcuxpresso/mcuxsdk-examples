/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_sema4.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_ASSERT(x)                \
    if (!(x))                        \
    {                                \
        PRINTF("Example error\r\n"); \
        for (;;)                     \
            ;                        \
    }

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void SEMA4_IRQHandler(void)
{
    /* If gate notification IRQ received, then lock the gate. */
    if (((1U << APP_SEMA4_GATE) & SEMA4_GetGateNotifyStatus(APP_SEMA4, APP_PROC_NUM)))
    {
        SEMA4_TryLock(APP_SEMA4, APP_SEMA4_GATE, APP_PROC_NUM);
    }
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("SEMA4 uboot example start\r\n");

    SEMA4_Init(APP_SEMA4);

    SEMA4_EnableGateNotifyInterrupt(APP_SEMA4, APP_PROC_NUM, (1 << APP_SEMA4_GATE));

    SOC_EnableSEMA4Intterrupt();

    /* Step 1: The other core lock the sema4 gate. */
    PRINTF("Lock sema4 gate in uboot using:\r\n");
    PRINTF(" > mw.b 0x%08x %d 1 \r\n", APP_SEMA4_GATE_ADDR, APP_OTHER_PROC_NUM + 1);

    /* Wait SEMA4 gate is locked by the other core. */
    while (SEMA4_GetLockProc(APP_SEMA4, APP_SEMA4_GATE) != APP_OTHER_PROC_NUM)
    {
    }

    /* Step 2: Current core try to lock the sema4 gate and lock failed. */
    APP_ASSERT(kStatus_Fail == SEMA4_TryLock(APP_SEMA4, APP_SEMA4_GATE, APP_PROC_NUM));

    /* Step 3: The other core unlock the sema4 gate. */
    PRINTF("Unlock sema4 gate in uboot using:\r\n");
    PRINTF(" > mw.b 0x%08x 0 1 \r\n", APP_SEMA4_GATE_ADDR);

    /* Wait SEMA4 gate is locked by the other core. */
    while (SEMA4_GetLockProc(APP_SEMA4, APP_SEMA4_GATE) == APP_OTHER_PROC_NUM)
    {
    }

    /*
     * When the other core unlocked the gate, current core receives the notification
     * interrupt and lock the gate in ISR.
     */
    APP_ASSERT(SEMA4_GetLockProc(APP_SEMA4, APP_SEMA4_GATE) == APP_PROC_NUM);

    SEMA4_Unlock(APP_SEMA4, APP_SEMA4_GATE);

    SOC_DisableSEMA4Intterrupt();

    SEMA4_DisableGateNotifyInterrupt(APP_SEMA4, APP_PROC_NUM, (1 << APP_SEMA4_GATE));

    SEMA4_Deinit(APP_SEMA4);

    PRINTF("SEMA4 uboot example success\r\n");

    while (1)
    {
    }
}
