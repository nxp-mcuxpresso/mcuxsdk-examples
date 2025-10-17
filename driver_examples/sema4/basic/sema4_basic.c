/*
 * Copyright 2019,2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_sema4.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
static inline void demo_check(bool condition, int line)
{

    if (!condition)
    {
        (void)PRINTF("Example error, line: %d\r\n", line);
        for (;;)
        {
        }
    }
}
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void DEMO_SEMA4_Lock(void);
static void DEMO_SEMA4_ResetGate(void);
static void DEMO_SEMA4_ResetAllGate(void);
static void DEMO_SEMA4_TestLockUnlock(uint8_t gateNum, bool isBlocking);
static void DEMO_SEMA4_ResetSingleGate(uint8_t gateNum);
static void DEMO_SEMA4_LockAllAndResetOne(uint8_t gateToReset);

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    (void)PRINTF("SEMA4 basic example start\r\n");
    (void)PRINTF("Proc number is %d\r\n", DEMO_PROC_NUM);

    SEMA4_Init(DEMO_SEMA4);

    DEMO_SEMA4_Lock();
    DEMO_SEMA4_ResetGate();
    DEMO_SEMA4_ResetAllGate();

    SEMA4_Deinit(DEMO_SEMA4);

    (void)PRINTF("\r\n\r\nSEMA4 basic example finished successfully\r\n");

    for (;;)
    {
    }
}

static void DEMO_SEMA4_TestLockUnlock(uint8_t gateNum, bool isBlocking) {
    uint8_t i = 0U;
    const char* method = isBlocking ? "blocking" : "non-blocking";
    
    (void)PRINTF("Lock with %s method\r\n", method);

    if (isBlocking) {
        (void)SEMA4_Lock(DEMO_SEMA4, gateNum, DEMO_PROC_NUM);
    } else {
        demo_check(SEMA4_TryLock(DEMO_SEMA4, gateNum, DEMO_PROC_NUM) == kStatus_Success, __LINE__);
    }

    /* Verify gate is locked */
    demo_check(SEMA4_GetLockProc(DEMO_SEMA4, gateNum) == DEMO_PROC_NUM, __LINE__);

    /* Check other gates are not locked */
    for (i = 0U; i < gateNum; i++) {
        demo_check(SEMA4_GetLockProc(DEMO_SEMA4, i) == -1, __LINE__);
    }
    for (i = gateNum + 1U; i < FSL_FEATURE_SEMA4_GATE_COUNT; i++) {
        demo_check(SEMA4_GetLockProc(DEMO_SEMA4, i) == -1, __LINE__);
    }

    SEMA4_Unlock(DEMO_SEMA4, gateNum);

    /* Verify gate is unlocked */
    demo_check(SEMA4_GetLockProc(DEMO_SEMA4, gateNum) == -1, __LINE__);

    (void)PRINTF("Gate lock&unlock success\r\n");
}

static void DEMO_SEMA4_Lock(void)
{
    uint8_t gateNum = 0U;

    (void)PRINTF("\r\n\r\nSEMA42 GATE LOCK AND UNLOCK\r\n");

    demo_check(SEMA4_ResetAllGates(DEMO_SEMA4) == kStatus_Success, __LINE__);

    for (gateNum = 0U; gateNum < FSL_FEATURE_SEMA4_GATE_COUNT; gateNum++)
    {
        (void)PRINTF("Gate %d/%d\r\n", gateNum + 1u, FSL_FEATURE_SEMA4_GATE_COUNT);

        /*
         * Non-blocking lock.
         */
        DEMO_SEMA4_TestLockUnlock(gateNum, false);

        /*
         * Blocking lock.
         */
        DEMO_SEMA4_TestLockUnlock(gateNum, true);
    }
}

static void DEMO_SEMA4_ResetSingleGate(uint8_t gateNum) 
{
    (void)PRINTF("Lock gate %d then reset it\r\n", gateNum);
    (void)SEMA4_Lock(DEMO_SEMA4, gateNum, DEMO_PROC_NUM);
    
    /* Now SEMA4 gate is locked */
    demo_check(SEMA4_GetLockProc(DEMO_SEMA4, gateNum) == DEMO_PROC_NUM, __LINE__);
    
    /* Reset the gate */
    demo_check(SEMA4_ResetGate(DEMO_SEMA4, gateNum) == kStatus_Success, __LINE__);
    
    /* Now SEMA4 gate is unlocked */
    demo_check(SEMA4_GetLockProc(DEMO_SEMA4, gateNum) == -1, __LINE__);
}

static void DEMO_SEMA4_LockAllAndResetOne(uint8_t gateToReset) 
{
    uint8_t i = 0U;
    (void)PRINTF("Lock all gates then reset gate %d\r\n", gateToReset);

    /* Lock all gates */
    for (i = 0U; i < FSL_FEATURE_SEMA4_GATE_COUNT; i++)
    {
        (void)SEMA4_Lock(DEMO_SEMA4, i, DEMO_PROC_NUM);
        demo_check(SEMA4_GetLockProc(DEMO_SEMA4, i) == DEMO_PROC_NUM, __LINE__);
    }

    /* Reset the specified gate */
    demo_check(SEMA4_ResetGate(DEMO_SEMA4, gateToReset) == kStatus_Success, __LINE__);
    demo_check(SEMA4_GetLockProc(DEMO_SEMA4, gateToReset) == -1, __LINE__);

    /* Verify other gates remain locked */
    for (i = 0U; i < gateToReset; i++)
    {
        demo_check(SEMA4_GetLockProc(DEMO_SEMA4, i) == DEMO_PROC_NUM, __LINE__);
    }
    for (i = gateToReset + 1U; i < FSL_FEATURE_SEMA4_GATE_COUNT; i++)
    {
        demo_check(SEMA4_GetLockProc(DEMO_SEMA4, i) == DEMO_PROC_NUM, __LINE__);
    }
}

static void DEMO_SEMA4_ResetGate(void)
{
    uint8_t gateNum = 0U;

    (void)PRINTF("\r\n\r\nSEMA42 GATE RESET\r\n");
    demo_check(SEMA4_ResetAllGates(DEMO_SEMA4) == kStatus_Success, __LINE__);

    /* Test resetting individual gates */
    for (gateNum = 0U; gateNum < FSL_FEATURE_SEMA4_GATE_COUNT; gateNum++)
    {
        DEMO_SEMA4_ResetSingleGate(gateNum);
    }

    /* Test locking all gates and resetting one at a time */
    for (gateNum = 0U; gateNum < FSL_FEATURE_SEMA4_GATE_COUNT; gateNum++)
    {
        DEMO_SEMA4_LockAllAndResetOne(gateNum);
        demo_check(SEMA4_ResetAllGates(DEMO_SEMA4) == kStatus_Success, __LINE__);
    }
}
static void DEMO_SEMA4_ResetAllGate(void)
{
    uint8_t gateNum = 0U;

    (void)PRINTF("\r\n\r\nSEMA42 ALL GATE RESET\r\n");

    demo_check(SEMA4_ResetAllGates(DEMO_SEMA4) == kStatus_Success, __LINE__);

    for (gateNum = 0U; gateNum < FSL_FEATURE_SEMA4_GATE_COUNT; gateNum++)
    {
        (void)PRINTF("Lock gate %d and reset all\r\n", gateNum);

        (void)SEMA4_Lock(DEMO_SEMA4, gateNum, DEMO_PROC_NUM);

        /* Now SEMA4 gate is locked. */
        demo_check(SEMA4_GetLockProc(DEMO_SEMA4, gateNum) == DEMO_PROC_NUM, __LINE__);

        /* Reset all gates. */
        demo_check(SEMA4_ResetAllGates(DEMO_SEMA4) == kStatus_Success, __LINE__);

        /* Now SEMA4 gate is unlocked. */
        demo_check(SEMA4_GetLockProc(DEMO_SEMA4, gateNum) == -1, __LINE__);
    }

    (void)PRINTF("Lock all gates and reset all\r\n");

    for (gateNum = 0U; gateNum < FSL_FEATURE_SEMA4_GATE_COUNT; gateNum++)
    {
        (void)SEMA4_Lock(DEMO_SEMA4, gateNum, DEMO_PROC_NUM);

        /* Now SEMA4 gate is locked. */
        demo_check(SEMA4_GetLockProc(DEMO_SEMA4, gateNum) == DEMO_PROC_NUM, __LINE__);
    }

    /* Reset all the gates. */
    demo_check(SEMA4_ResetAllGates(DEMO_SEMA4) == kStatus_Success, __LINE__);

    /* Check the gate status, should be unlocked. */
    for (gateNum = 0U; gateNum < FSL_FEATURE_SEMA4_GATE_COUNT; gateNum++)
    {
        demo_check(SEMA4_GetLockProc(DEMO_SEMA4, gateNum) == -1, __LINE__);
    }
}
