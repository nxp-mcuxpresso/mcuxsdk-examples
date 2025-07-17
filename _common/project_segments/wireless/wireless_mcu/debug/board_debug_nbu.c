/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/************************************************************************************
 * Include
 ************************************************************************************/

#include <stdint.h>
#include "fwk_nbu_dbg.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* CFSR (Configurable Fault Status Register) bit definitions */
#define CFSR_IACCVIOL_Pos    0U
#define CFSR_IACCVIOL_Msk    (1UL << CFSR_IACCVIOL_Pos)
#define CFSR_DACCVIOL_Pos    1U
#define CFSR_DACCVIOL_Msk    (1UL << CFSR_DACCVIOL_Pos)
#define CFSR_MUNSTKERR_Pos   3U
#define CFSR_MUNSTKERR_Msk   (1UL << CFSR_MUNSTKERR_Pos)
#define CFSR_MSTKERR_Pos     4U
#define CFSR_MSTKERR_Msk     (1UL << CFSR_MSTKERR_Pos)
#define CFSR_MLSPERR_Pos     5U
#define CFSR_MLSPERR_Msk     (1UL << CFSR_MLSPERR_Pos)
#define CFSR_MMARVALID_Pos   7U
#define CFSR_MMARVALID_Msk   (1UL << CFSR_MMARVALID_Pos)

#define CFSR_IBUSERR_Pos     8U
#define CFSR_IBUSERR_Msk     (1UL << CFSR_IBUSERR_Pos)
#define CFSR_PRECISERR_Pos   9U
#define CFSR_PRECISERR_Msk   (1UL << CFSR_PRECISERR_Pos)
#define CFSR_IMPRECISERR_Pos 10U
#define CFSR_IMPRECISERR_Msk (1UL << CFSR_IMPRECISERR_Pos)
#define CFSR_UNSTKERR_Pos    11U
#define CFSR_UNSTKERR_Msk    (1UL << CFSR_UNSTKERR_Pos)
#define CFSR_STKERR_Pos      12U
#define CFSR_STKERR_Msk      (1UL << CFSR_STKERR_Pos)
#define CFSR_LSPERR_Pos      13U
#define CFSR_LSPERR_Msk      (1UL << CFSR_LSPERR_Pos)
#define CFSR_BFARVALID_Pos   15U
#define CFSR_BFARVALID_Msk   (1UL << CFSR_BFARVALID_Pos)

#define CFSR_UNDEFINSTR_Pos  16U
#define CFSR_UNDEFINSTR_Msk  (1UL << CFSR_UNDEFINSTR_Pos)
#define CFSR_INVSTATE_Pos    17U
#define CFSR_INVSTATE_Msk    (1UL << CFSR_INVSTATE_Pos)
#define CFSR_INVPC_Pos       18U
#define CFSR_INVPC_Msk       (1UL << CFSR_INVPC_Pos)
#define CFSR_NOCP_Pos        19U
#define CFSR_NOCP_Msk        (1UL << CFSR_NOCP_Pos)
#define CFSR_UNALIGNED_Pos   24U
#define CFSR_UNALIGNED_Msk   (1UL << CFSR_UNALIGNED_Pos)
#define CFSR_DIVBYZERO_Pos   25U
#define CFSR_DIVBYZERO_Msk   (1UL << CFSR_DIVBYZERO_Pos)

/************************************************************************************
 * Private memory declarations
 ************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private type definitions and macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
static void DBG_PrintMemoryManagementFaults(uint32_t cfsr);
static void DBG_PrintBusFaults(uint32_t cfsr);
static void DBG_PrintUsageFaults(uint32_t cfsr);
static void DBG_PrintRawData(const char* label, const char* start_marker, const char* end_marker,
                             const uint8_t* data, size_t size);

/**
 * @brief Analyze and display NBU fault information via serial output
 *
 * This callback retrieves the NBU debug structure and provides a comprehensive
 * analysis of the fault condition, including:
 * - Exception information
 * - Processor register state
 * - Detailed fault status analysis with explanations
 * - Memory addresses involved in faults
 *
 * The analysis is printed to the serial console using PRINTF.
 *
 * @note This function should be called after detecting an NBU fault condition
 */
static void BOARD_NbuSystemNotifyCb(nbu_dbg_event_id_t event_id)
{
    nbu_debug_struct_t debug_info;
    regs_status_t *regs;
    int status;

    status = NBUDBG_StructDump(&debug_info);
    if (status != 0)
    {
        PRINTF("ERROR: Failed to retrieve NBU debug information\n");
        return;
    }

    regs = &debug_info.reg_dump;

    PRINTF("\n=== NBU Fault Analysis ===\n");
    PRINTF("Exception Information:\n");
    PRINTF("  NBU event ID: 0x%08X\n", event_id);
    PRINTF("  Exception ID: 0x%08X\n", regs->exception_id);
    PRINTF("  NBU SHA1    : 0x%08X\n", regs->nbu_sha1);

    PRINTF("\nProcessor State:\n");
    PRINTF("  PC  (Program Counter): 0x%08X\n", regs->pc);
    PRINTF("  LR  (Link Register)  : 0x%08X\n", regs->lr);
    PRINTF("  PSP (Process Stack)  : 0x%08X\n", regs->psp);
    PRINTF("  PSR (Program Status) : 0x%08X\n", regs->psr);

    PRINTF("\nGeneral Purpose Registers:\n");
    PRINTF("  R0:  0x%08X  R1:  0x%08X  R2:  0x%08X  R3:  0x%08X\n",
           regs->r0, regs->r1, regs->r2, regs->r3);
    PRINTF("  R4:  0x%08X  R5:  0x%08X  R6:  0x%08X  R7:  0x%08X\n",
           regs->r4, regs->r5, regs->r6, regs->r7);
    PRINTF("  R8:  0x%08X  R9:  0x%08X  R10: 0x%08X  R11: 0x%08X\n",
           regs->r8, regs->r9, regs->r10, regs->r11);
    PRINTF("  R12: 0x%08X\n", regs->r12);

    PRINTF("\nFault Status Analysis:\n");
    PRINTF("  CFSR (Configurable Fault Status): 0x%08X\n", regs->cfsr);

    if (regs->cfsr != 0)
    {
        /* Check Memory Management Faults */
        if (regs->cfsr & 0xFF)
        {
            PRINTF("\n  Memory Management Faults Detected:\n");
            DBG_PrintMemoryManagementFaults(regs->cfsr);

            if (regs->cfsr & CFSR_MMARVALID_Msk)
            {
                PRINTF("    Faulting Address (MMFAR): 0x%08X\n", regs->xfar.mmfar);
            }
        }

        /* Check Bus Faults */
        if (regs->cfsr & 0xFF00)
        {
            PRINTF("\n  Bus Faults Detected:\n");
            DBG_PrintBusFaults(regs->cfsr);

            if (regs->cfsr & CFSR_BFARVALID_Msk)
            {
                PRINTF("    Faulting Address (BFAR): 0x%08X\n", regs->xfar.bfar);
            }
        }

        /* Check Usage Faults */
        if (regs->cfsr & 0xFFFF0000)
        {
            PRINTF("\n  Usage Faults Detected:\n");
            DBG_PrintUsageFaults(regs->cfsr);
        }
    }
    else
    {
        PRINTF("  No fault status flags set in CFSR\n");
    }

    /* Raw dump of BLE debug data */
    DBG_PrintRawData("BLE Debug Data", "DBG_BLE_START", "DBG_BLE_END",
                     debug_info.dbg_ble, NBUDBG_BLE_STRUCT_SIZE);

    /* Raw dump of 15.4 debug data */
    DBG_PrintRawData("15.4 Debug Data", "DBG_15_4_START", "DBG_15_4_END",
                     debug_info.dbg_15_4, NBUDBG_15_4_STRUCT_SIZE);

    PRINTF("\n=== End of NBU Fault Analysis ===\n\n");
}

static void DBG_PrintMemoryManagementFaults(uint32_t cfsr)
{
    if (cfsr & CFSR_IACCVIOL_Msk)
    {
        PRINTF("    - Instruction access violation\n");
        PRINTF("      Cause: Attempted to execute from a region marked as non-executable\n");
    }

    if (cfsr & CFSR_DACCVIOL_Msk)
    {
        PRINTF("    - Data access violation\n");
        PRINTF("      Cause: Attempted to access a memory region without proper permissions\n");
    }

    if (cfsr & CFSR_MUNSTKERR_Msk)
    {
        PRINTF("    - Memory management fault on unstacking\n");
        PRINTF("      Cause: Error during exception return stack pop operation\n");
    }

    if (cfsr & CFSR_MSTKERR_Msk)
    {
        PRINTF("    - Memory management fault on stacking\n");
        PRINTF("      Cause: Error during exception entry stack push operation\n");
    }

    if (cfsr & CFSR_MLSPERR_Msk)
    {
        PRINTF("    - Memory management fault during lazy FP state preservation\n");
        PRINTF("      Cause: Error during floating-point context save\n");
    }
}

static void DBG_PrintBusFaults(uint32_t cfsr)
{
    if (cfsr & CFSR_IBUSERR_Msk)
    {
        PRINTF("    - Instruction bus error\n");
        PRINTF("      Cause: Bus fault on instruction fetch\n");
    }

    if (cfsr & CFSR_PRECISERR_Msk)
    {
        PRINTF("    - Precise data bus error\n");
        PRINTF("      Cause: Bus fault on data access (address in BFAR is valid)\n");
    }

    if (cfsr & CFSR_IMPRECISERR_Msk)
    {
        PRINTF("    - Imprecise data bus error\n");
        PRINTF("      Cause: Bus fault on data access (address in BFAR may not be valid)\n");
    }

    if (cfsr & CFSR_UNSTKERR_Msk)
    {
        PRINTF("    - Bus fault on unstacking\n");
        PRINTF("      Cause: Bus error during exception return\n");
    }

    if (cfsr & CFSR_STKERR_Msk)
    {
        PRINTF("    - Bus fault on stacking\n");
        PRINTF("      Cause: Bus error during exception entry\n");
    }

    if (cfsr & CFSR_LSPERR_Msk)
    {
        PRINTF("    - Bus fault during lazy FP state preservation\n");
        PRINTF("      Cause: Bus error during floating-point context save\n");
    }
}

static void DBG_PrintUsageFaults(uint32_t cfsr)
{
    if (cfsr & CFSR_UNDEFINSTR_Msk)
    {
        PRINTF("    - Undefined instruction\n");
        PRINTF("      Cause: Attempted to execute an undefined instruction\n");
    }

    if (cfsr & CFSR_INVSTATE_Msk)
    {
        PRINTF("    - Invalid state\n");
        PRINTF("      Cause: Attempted to execute instruction with invalid EPSR state\n");
    }

    if (cfsr & CFSR_INVPC_Msk)
    {
        PRINTF("    - Invalid PC load\n");
        PRINTF("      Cause: Attempted to load invalid PC value\n");
    }

    if (cfsr & CFSR_NOCP_Msk)
    {
        PRINTF("    - No coprocessor\n");
        PRINTF("      Cause: Attempted to access unavailable coprocessor\n");
    }

    if (cfsr & CFSR_UNALIGNED_Msk)
    {
        PRINTF("    - Unaligned access\n");
        PRINTF("      Cause: Unaligned memory access with UNALIGN_TRP set\n");
    }

    if (cfsr & CFSR_DIVBYZERO_Msk)
    {
        PRINTF("    - Division by zero\n");
        PRINTF("      Cause: Division by zero with DIV_0_TRP set\n");
    }
}

static void DBG_PrintRawData(const char* label, const char* start_marker, const char* end_marker,
                            const uint8_t* data, size_t size)
{
    PRINTF("\n%s (Raw - %u bytes):\n", label, size);
    PRINTF("%s\n", start_marker);

    for (size_t i = 0; i < size; i++)
    {
        if (i % 16 == 0)
        {
            PRINTF("%04X: ", (unsigned int)i);
        }
        PRINTF("%02X ", data[i]);
        if ((i + 1) % 16 == 0)
        {
            PRINTF("\n");
        }
    }
    if (size % 16 != 0)
    {
        PRINTF("\n");
    }

    PRINTF("%s\n", end_marker);
}

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

int BOARD_DbgNbuInit(void)
{
    NBUDBG_RegisterSystemErrorNotify(BOARD_NbuSystemNotifyCb);

    return 0;
}

void BOARD_DbgNbuProcess(void)
{
    NBUDBG_StateCheck();
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
