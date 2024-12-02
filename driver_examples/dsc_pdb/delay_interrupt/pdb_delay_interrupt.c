/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_pdb.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint16_t g_PdbDelayInterruptCounter;
volatile bool g_PdbDelayInterruptFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief ISR for PDB interrupt function
 */
#pragma interrupt alignsp saveall
void DEMO_PDB_IRQHandler(void)
{
    PDB_ClearStatusFlags(DEMO_PDB_BASE, kPDB_DelayAStatusFlag);
    g_PdbDelayInterruptCounter++;
    g_PdbDelayInterruptFlag = true;
}
#pragma interrupt off

/*!
 * @brief Main function
 */
int main(void)
{
    pdb_config_t pdbConfigStruct;

    BOARD_InitHardware();

    EnableIRQ(DEMO_PDB_IRQ_ID);
    PRINTF("\r\nPDB Delay Interrupt Example.\r\n");

    /* Configure the PDB counter. */
    memset(&pdbConfigStruct, 0, sizeof(pdb_config_t));
    PDB_GetDefaultConfig(&pdbConfigStruct);
    pdbConfigStruct.sPdbTriggerABOutputConfig.bEnableTrigger1 = true;
    pdbConfigStruct.u16EnableInterruptMask                    = kPDB_EnableDelayAInterrupt;
    pdbConfigStruct.bEnablePDB                                = true;
    PDB_Init(DEMO_PDB_BASE, &pdbConfigStruct);

    while (1)
    {
        PRINTF("\r\nType any key into terminal to trigger the PDB counter ...\r\n");
        GETCHAR();
        g_PdbDelayInterruptFlag = false;
        PDB_DoSoftwareTrigger(DEMO_PDB_BASE);
        while (!g_PdbDelayInterruptFlag)
        {
        }
        PRINTF("PDB Delay Interrupt Counter: %d\r\n", g_PdbDelayInterruptCounter);
    }
}
