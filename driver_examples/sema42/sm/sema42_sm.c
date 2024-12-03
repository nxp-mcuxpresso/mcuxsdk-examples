/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_sema42.h"
#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware.*/
    BOARD_InitHardware();

    PRINTF("\r\n");
    PRINTF("**************************************************************\r\n");
    PRINTF("* Please make sure the SM is started now.                 *\r\n");
    PRINTF("* Use the following commands in SM(M33) for SEMA42 gate access *\r\n");
    PRINTF("* - md.b 0x%08x 1 : Get SEMA42 gate status.              *\r\n", APP_SEMA42_GATE_ADDR);
    PRINTF("*   - 0 - Unlocked;                                          *\r\n");
    PRINTF("*   - %d - Locked by Cortex-M33;                                *\r\n", APP_CORTEX_M33_DID + 1);
    PRINTF("*   - %d - Locked by Cortex-M7;                                *\r\n", APP_CORTEX_M7_DID + 1);
    PRINTF("* - mm.b 0x%08x %d 1 : Lock the SEMA42 gate.              *\r\n", APP_SEMA42_GATE_ADDR,
           APP_CORTEX_M33_DID + 1);
    PRINTF("* - mm.b 0x%08x 0 1 : Unlock the SEMA42 gate.            *\r\n", APP_SEMA42_GATE_ADDR);
    PRINTF("**************************************************************\r\n\r\n");

    PRINTF("Press anykey to start the example...\r\n");

    GETCHAR();

    PRINTF("\r\nSEMA42 example started!\r\n");

    APP_InitDomainConfig();

    /* SEMA42 init */
    SEMA42_Init(APP_SEMA42);
    /* Reset the sema42 gate */
    SEMA42_ResetAllGates(APP_SEMA42);

    /*
     * Part 1. Cortex-M7 locks the SEMA42 gate, Cortex-M33 checks the status in SM.
     */
    PRINTF("\r\nNow the SEMA42 gate is unlocked, checking status in SM returns 0.\r\n");
    PRINTF("Press anykey to lock the SEMA42 gate...\r\n");

    GETCHAR();
    SEMA42_Lock(APP_SEMA42, APP_SEMA42_GATE, APP_CORTEX_M7_DID);

    PRINTF("\r\nNow the SEMA42 gate is locked, checking status in SM returns %d.\r\n", APP_CORTEX_M7_DID + 1);
    PRINTF("Lock or unlock the SEMA42 gate in SM, the status does not change.\r\n");

    PRINTF("\r\nPress anykey to unlock the SEMA42 gate...\r\n");

    GETCHAR();
    SEMA42_Unlock(APP_SEMA42, APP_SEMA42_GATE);

    PRINTF("\r\nNow the SEMA42 gate is unlocked, checking status in SM returns 0.\r\n");

    /*
     * Part 2. Cortex-M33 locks the SEMA42 gate in SM, Cortex-M checks the status.
     */
    do
    {
        SEMA42_Unlock(APP_SEMA42, APP_SEMA42_GATE);

        PRINTF("\r\nLock the SEMA42 gate in SM, after locked, then press anykey...\r\n");

        GETCHAR();

    } while (kStatus_SEMA42_Busy != SEMA42_TryLock(APP_SEMA42, APP_SEMA42_GATE, APP_CORTEX_M7_DID));

    PRINTF("\r\nCortex-M33 has locked the SEMA42 gate in SM, Cortex-M7 could not lock.\r\n");

    /*
     * Part 3. Reset the SEMA42 gate.
     */
    PRINTF("\r\nPress anykey to reset the SEMA42 gate...\r\n");

    GETCHAR();

    SEMA42_ResetGate(APP_SEMA42, APP_SEMA42_GATE);

    if (kSEMA42_Unlocked != SEMA42_GetGateStatus(APP_SEMA42, APP_SEMA42_GATE))
    {
        PRINTF("\r\nSEMA42 gate reset error.\r\n");

        APP_DeinitDomainConfig();
        while (1)
        {
        }
    }

    PRINTF("\r\nNow the SEMA42 gate is unlocked, checking status in SM returns 0.\r\n");

    PRINTF("\r\nPress anykey to finish the example...\r\n");

    GETCHAR();

    APP_DeinitDomainConfig();

    PRINTF("\r\nSEMA42 SM example successed.\r\n");

    while (1)
    {
    }
}
