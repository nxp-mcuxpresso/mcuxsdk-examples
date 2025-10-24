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
#include "fsl_device_registers.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

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
 * \brief NBU debug notification callback for fault analysis and warning handling
 *
 * This callback handles NBU debug events and provides different responses based on event type:
 *
 * For NBU warnings (nbu_dbg_warning):
 * - Displays new warning count information
 *
 * For NBU fatal errors (nbu_dbg_fatal_error):
 * - Exception information
 * - Processor register state
 * - Detailed fault status analysis with explanations
 * - Memory addresses involved in faults
 *
 * The analysis is printed to the serial console using PRINTF.
 *
 * \param nbu_dbg_context_t structure pointer with number of warning/ fatal error fron NBU
 *
 * \note This function should be registered via NBUDBG_RegisterNbuDebugNotificationCb()
 *       and will be called automatically when NBUDBG_StateCheck() detects NBU events
 */
static void BOARD_NbuDebugNotifyCb(const nbu_dbg_context_t *nbu_event)
{
    nbu_debug_struct_t debug_info;
    nbu_dbg_info_t *nbu_dbg_info;
    reg_info_t *regs;

    int status;

    if (nbu_event->nbu_warning_count > 0U)
    {
        PRINTF("New NBU Warnings detected: %u warnings\n", nbu_event->nbu_warning_count);
    }

    if (nbu_event->nbu_error_count > 0U)
    {
        status = NBUDBG_StructDump(&debug_info);
        if (status != 0)
        {
            PRINTF("ERROR: Failed to retrieve NBU debug information\n");
        }
        else
        {
            nbu_dbg_info = &debug_info.nbu_dbg_info;
            regs = &debug_info.nbu_dbg_info.reg_info;
            PRINTF("\n=== NBU Fault/Assert Analysis ===\n\n");
            if (nbu_dbg_info->exception_id == NBUDBG_EXCEPTION_ID_FOR_ASSERT_MAGIC)
            {
                /* Assert on NBU side */
                PRINTF("NBU Assert Detected\n");
                PRINTF("  Line: %u\n", nbu_dbg_info->assert_info.line);
                PRINTF("  File name: %s\n", nbu_dbg_info->assert_info.file_name);
            }
            else
            {
                /* Fault on NBU side */
                PRINTF("NBU Fault Detected\n");
                PRINTF("Exception Information:\n");
                PRINTF("  Exception ID: 0x%08X\n", nbu_dbg_info->exception_id);
                PRINTF("  NBU SHA1    : 0x%08X\n", nbu_dbg_info->nbu_sha1);

                PRINTF("\nProcessor State:\n");
                PRINTF("  PC  (Program Counter): 0x%08X\n", regs->pc);
                PRINTF("  LR  (Link Register)  : 0x%08X\n", regs->lr);
                PRINTF("  SP  (Stack Pointer)  : 0x%08X\n", regs->sp);
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

                        if (regs->cfsr & SCB_CFSR_MMARVALID_Msk)
                        {
                            PRINTF("    Faulting Address (MMFAR): 0x%08X\n", regs->xfar.mmfar);
                        }
                    }

                    /* Check Bus Faults */
                    if (regs->cfsr & 0xFF00)
                    {
                        PRINTF("\n  Bus Faults Detected:\n");
                        DBG_PrintBusFaults(regs->cfsr);

                        if (regs->cfsr & SCB_CFSR_BFARVALID_Msk)
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
            }

            PRINTF("\nExecution Context:\n");
            if (NBUDBG_IS_HANDLER_MODE(nbu_dbg_info->execution_context.handler_irq))
            {
                uint32_t irq_number = NBUDBG_GET_IRQ_NUMBER(nbu_dbg_info->execution_context.handler_irq);
                PRINTF("  Mode: Handler Mode (Interrupt Context)\n");
                PRINTF("  IRQ Number: %u\n", irq_number);
                (void) irq_number; /* Supress warning when debug console (PRINTF) is disabled */
            }
            else
            {
                PRINTF("  Mode: Thread Mode\n");
                PRINTF("  Thread Address: 0x%08X\n", nbu_dbg_info->execution_context.thread_info.thread_addr);
                PRINTF("  Thread Name: %.8s\n", nbu_dbg_info->execution_context.thread_info.thread_name);
            }
            /* Raw dump of BLE debug data */
            DBG_PrintRawData("BLE Debug Data", "DBG_BLE_START", "DBG_BLE_END",
                             debug_info.dbg_ble, NBUDBG_BLE_STRUCT_SIZE);

            /* Raw dump of 15.4 debug data */
            DBG_PrintRawData("15.4 Debug Data", "DBG_15_4_START", "DBG_15_4_END",
                             debug_info.dbg_15_4, NBUDBG_15_4_STRUCT_SIZE);

            PRINTF("\n=== End of NBU Fault/Assert Analysis ===\n\n");
        }
    }
}

static void DBG_PrintMemoryManagementFaults(uint32_t cfsr)
{
    if (cfsr & SCB_CFSR_IACCVIOL_Msk)
    {
        PRINTF("    - Instruction access violation\n");
        PRINTF("      Cause: Attempted to execute from a region marked as non-executable\n");
    }

    if (cfsr & SCB_CFSR_DACCVIOL_Msk)
    {
        PRINTF("    - Data access violation\n");
        PRINTF("      Cause: Attempted to access a memory region without proper permissions\n");
    }

    if (cfsr & SCB_CFSR_MUNSTKERR_Msk)
    {
        PRINTF("    - Memory management fault on unstacking\n");
        PRINTF("      Cause: Error during exception return stack pop operation\n");
    }

    if (cfsr & SCB_CFSR_MSTKERR_Msk)
    {
        PRINTF("    - Memory management fault on stacking\n");
        PRINTF("      Cause: Error during exception entry stack push operation\n");
    }

    if (cfsr & SCB_CFSR_MLSPERR_Msk)
    {
        PRINTF("    - Memory management fault during lazy FP state preservation\n");
        PRINTF("      Cause: Error during floating-point context save\n");
    }
}

static void DBG_PrintBusFaults(uint32_t cfsr)
{
    if (cfsr & SCB_CFSR_IBUSERR_Msk)
    {
        PRINTF("    - Instruction bus error\n");
        PRINTF("      Cause: Bus fault on instruction fetch\n");
    }

    if (cfsr & SCB_CFSR_PRECISERR_Msk)
    {
        PRINTF("    - Precise data bus error\n");
        PRINTF("      Cause: Bus fault on data access (address in BFAR is valid)\n");
    }

    if (cfsr & SCB_CFSR_IMPRECISERR_Msk)
    {
        PRINTF("    - Imprecise data bus error\n");
        PRINTF("      Cause: Bus fault on data access (address in BFAR may not be valid)\n");
    }

    if (cfsr & SCB_CFSR_UNSTKERR_Msk)
    {
        PRINTF("    - Bus fault on unstacking\n");
        PRINTF("      Cause: Bus error during exception return\n");
    }

    if (cfsr & SCB_CFSR_STKERR_Msk)
    {
        PRINTF("    - Bus fault on stacking\n");
        PRINTF("      Cause: Bus error during exception entry\n");
    }

    if (cfsr & SCB_CFSR_LSPERR_Msk)
    {
        PRINTF("    - Bus fault during lazy FP state preservation\n");
        PRINTF("      Cause: Bus error during floating-point context save\n");
    }
}

static void DBG_PrintUsageFaults(uint32_t cfsr)
{
    if (cfsr & SCB_CFSR_UNDEFINSTR_Msk)
    {
        PRINTF("    - Undefined instruction\n");
        PRINTF("      Cause: Attempted to execute an undefined instruction\n");
    }

    if (cfsr & SCB_CFSR_INVSTATE_Msk)
    {
        PRINTF("    - Invalid state\n");
        PRINTF("      Cause: Attempted to execute instruction with invalid EPSR state\n");
    }

    if (cfsr & SCB_CFSR_INVPC_Msk)
    {
        PRINTF("    - Invalid PC load\n");
        PRINTF("      Cause: Attempted to load invalid PC value\n");
    }

    if (cfsr & SCB_CFSR_NOCP_Msk)
    {
        PRINTF("    - No coprocessor\n");
        PRINTF("      Cause: Attempted to access unavailable coprocessor\n");
    }

    if (cfsr & SCB_CFSR_UNALIGNED_Msk)
    {
        PRINTF("    - Unaligned access\n");
        PRINTF("      Cause: Unaligned memory access with UNALIGN_TRP set\n");
    }

    if (cfsr & SCB_CFSR_DIVBYZERO_Msk)
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
    NBUDBG_RegisterNbuDebugNotificationCb(BOARD_NbuDebugNotifyCb);

    return 0;
}

void BOARD_DbgNbuProcess(void)
{
    NBUDBG_StateCheck();
}

int fsl_assert_hook(const char *failedExpr, const char *file, int line)
{
    BOARD_DbgNbuProcess();
    return 0;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
