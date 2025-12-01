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
static nbu_debug_struct_t debug_info;

/************************************************************************************
*************************************************************************************
* Private type definitions and macros
*************************************************************************************
************************************************************************************/
#define BOARD_DBG_PRINTF (void)PRINTF

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
    nbu_dbg_info_t *nbu_dbg_info;
    reg_info_t *regs;
    int status;

    do
    {
        if (nbu_event->nbu_warning_count > 0U)
        {
            BOARD_DBG_PRINTF("WARNING: %u New NBU Warnings detected\n", nbu_event->nbu_warning_count);
        }

        if (nbu_event->nbu_is_halted != 0U)
        {
            BOARD_DBG_PRINTF("ERROR: NBU is stuck - possible stall or deadlock detected\n");
        }

        status = NBUDBG_StructDump(&debug_info);
        if (status != 0)
        {
            BOARD_DBG_PRINTF("ERROR: Failed to retrieve NBU debug information\n");
            break;
        }

        BOARD_DBG_PRINTF("NBU Debug version: 0x%04X\n", debug_info.version);
        if (debug_info.version != (uint16_t)NBUDBG_VERSION)
        {
            BOARD_DBG_PRINTF("!! Host Debug version 0x%04X != NBU debug version 0x%04X !!\n", (uint16_t)NBUDBG_VERSION, debug_info.version);
            BOARD_DBG_PRINTF("!! The following analysis may be incorrect !!\n");
        }

        if ((nbu_event->nbu_error_count > 0U))
        {
            nbu_dbg_info = &debug_info.nbu_dbg_info;
            regs = &debug_info.nbu_dbg_info.reg_info;
            BOARD_DBG_PRINTF("\n=== NBU Fault/Assert Analysis ===\n\n");
            if (nbu_dbg_info->exception_id == NBUDBG_EXCEPTION_ID_FOR_ASSERT_MAGIC)
            {
                /* Assert on NBU side */
                BOARD_DBG_PRINTF("NBU Assert Detected\n");
                BOARD_DBG_PRINTF("  Line: %u\n", nbu_dbg_info->assert_info.line);
                BOARD_DBG_PRINTF("  File name: %s\n", nbu_dbg_info->assert_info.file_name);
            }
            else
            {
                /* Fault on NBU side */
                BOARD_DBG_PRINTF("NBU Fault Detected\n");
                BOARD_DBG_PRINTF("Exception Information:\n");
                BOARD_DBG_PRINTF("  Exception ID: 0x%08X\n", nbu_dbg_info->exception_id);
                BOARD_DBG_PRINTF("  NBU SHA1    : 0x%08X\n", nbu_dbg_info->nbu_sha1);

                BOARD_DBG_PRINTF("\nProcessor State:\n");
                BOARD_DBG_PRINTF("  PC  (Program Counter): 0x%08X\n", regs->pc);
                BOARD_DBG_PRINTF("  LR  (Link Register)  : 0x%08X\n", regs->lr);
                BOARD_DBG_PRINTF("  SP  (Stack Pointer)  : 0x%08X\n", regs->sp);
                BOARD_DBG_PRINTF("  PSR (Program Status) : 0x%08X\n", regs->psr);

                BOARD_DBG_PRINTF("\nGeneral Purpose Registers:\n");
                BOARD_DBG_PRINTF("  R0:  0x%08X  R1:  0x%08X  R2:  0x%08X  R3:  0x%08X\n",
                       regs->r0, regs->r1, regs->r2, regs->r3);
                BOARD_DBG_PRINTF("  R4:  0x%08X  R5:  0x%08X  R6:  0x%08X  R7:  0x%08X\n",
                       regs->r4, regs->r5, regs->r6, regs->r7);
                BOARD_DBG_PRINTF("  R8:  0x%08X  R9:  0x%08X  R10: 0x%08X  R11: 0x%08X\n",
                       regs->r8, regs->r9, regs->r10, regs->r11);
                BOARD_DBG_PRINTF("  R12: 0x%08X\n", regs->r12);

                BOARD_DBG_PRINTF("\nFault Status Analysis:\n");
                BOARD_DBG_PRINTF("  CFSR (Configurable Fault Status): 0x%08X\n", regs->cfsr);

                if (regs->cfsr != 0U)
                {
                    /* Check Memory Management Faults */
                    if ((regs->cfsr & 0xFFU) != 0U)
                    {
                        BOARD_DBG_PRINTF("\n  Memory Management Faults Detected:\n");
                        DBG_PrintMemoryManagementFaults(regs->cfsr);

                        if ((regs->cfsr & SCB_CFSR_MMARVALID_Msk) != 0U)
                        {
                            BOARD_DBG_PRINTF("    Faulting Address (MMFAR): 0x%08X\n", regs->xfar.mmfar);
                        }
                    }

                    /* Check Bus Faults */
                    if ((regs->cfsr & 0xFF00U) != 0U)
                    {
                        BOARD_DBG_PRINTF("\n  Bus Faults Detected:\n");
                        DBG_PrintBusFaults(regs->cfsr);

                        if ((regs->cfsr & SCB_CFSR_BFARVALID_Msk) != 0U)
                        {
                            BOARD_DBG_PRINTF("    Faulting Address (BFAR): 0x%08X\n", regs->xfar.bfar);
                        }
                    }

                    /* Check Usage Faults */
                    if ((regs->cfsr & 0xFFFF0000U) != 0U)
                    {
                        BOARD_DBG_PRINTF("\n  Usage Faults Detected:\n");
                        DBG_PrintUsageFaults(regs->cfsr);
                    }
                }
                else
                {
                    BOARD_DBG_PRINTF("  No fault status flags set in CFSR\n");
                }
            }

            BOARD_DBG_PRINTF("\nExecution Context:\n");
            if (NBUDBG_IS_HANDLER_MODE(nbu_dbg_info->execution_context.handler_irq))
            {
                uint32_t irq_number = NBUDBG_GET_IRQ_NUMBER(nbu_dbg_info->execution_context.handler_irq);
                BOARD_DBG_PRINTF("  Mode: Handler Mode (Interrupt Context)\n");
                BOARD_DBG_PRINTF("  IRQ Number: %u\n", irq_number);
                (void) irq_number; /* Supress warning when debug console (PRINTF) is disabled */
            }
            else
            {
                BOARD_DBG_PRINTF("  Mode: Thread Mode\n");
                BOARD_DBG_PRINTF("  Thread Address: 0x%08X\n", nbu_dbg_info->execution_context.thread_info.thread_addr);
                BOARD_DBG_PRINTF("  Thread Name: %.8s\n", nbu_dbg_info->execution_context.thread_info.thread_name);
            }

            BOARD_DBG_PRINTF("\n=== End of NBU Fault/Assert Analysis ===\n\n");
        }
        /* Raw dump of BLE debug data */
        DBG_PrintRawData("BLE Debug Data", "DBG_BLE_START", "DBG_BLE_END",
                         debug_info.dbg_ble, NBUDBG_BLE_STRUCT_SIZE);
    } while (false);
}

static void DBG_PrintMemoryManagementFaults(uint32_t cfsr)
{
    if ((cfsr & SCB_CFSR_IACCVIOL_Msk) != 0U)
    {
        BOARD_DBG_PRINTF("    - Instruction access violation\n");
        BOARD_DBG_PRINTF("      Cause: Attempted to execute from a region marked as non-executable\n");
    }

    if ((cfsr & SCB_CFSR_DACCVIOL_Msk) != 0U)
    {
        BOARD_DBG_PRINTF("    - Data access violation\n");
        BOARD_DBG_PRINTF("      Cause: Attempted to access a memory region without proper permissions\n");
    }

    if ((cfsr & SCB_CFSR_MUNSTKERR_Msk) != 0U)
    {
        BOARD_DBG_PRINTF("    - Memory management fault on unstacking\n");
        BOARD_DBG_PRINTF("      Cause: Error during exception return stack pop operation\n");
    }

    if ((cfsr & SCB_CFSR_MSTKERR_Msk) != 0U)
    {
        BOARD_DBG_PRINTF("    - Memory management fault on stacking\n");
        BOARD_DBG_PRINTF("      Cause: Error during exception entry stack push operation\n");
    }

    if ((cfsr & SCB_CFSR_MLSPERR_Msk) != 0U)
    {
        BOARD_DBG_PRINTF("    - Memory management fault during lazy FP state preservation\n");
        BOARD_DBG_PRINTF("      Cause: Error during floating-point context save\n");
    }
}

static void DBG_PrintBusFaults(uint32_t cfsr)
{
    if ((cfsr & SCB_CFSR_IBUSERR_Msk) != 0U)
    {
        BOARD_DBG_PRINTF("    - Instruction bus error\n");
        BOARD_DBG_PRINTF("      Cause: Bus fault on instruction fetch\n");
    }

    if ((cfsr & SCB_CFSR_PRECISERR_Msk) != 0U)
    {
        BOARD_DBG_PRINTF("    - Precise data bus error\n");
        BOARD_DBG_PRINTF("      Cause: Bus fault on data access (address in BFAR is valid)\n");
    }

    if ((cfsr & SCB_CFSR_IMPRECISERR_Msk) != 0U)
    {
        BOARD_DBG_PRINTF("    - Imprecise data bus error\n");
        BOARD_DBG_PRINTF("      Cause: Bus fault on data access (address in BFAR may not be valid)\n");
    }

    if ((cfsr & SCB_CFSR_UNSTKERR_Msk) != 0U)
    {
        BOARD_DBG_PRINTF("    - Bus fault on unstacking\n");
        BOARD_DBG_PRINTF("      Cause: Bus error during exception return\n");
    }

    if ((cfsr & SCB_CFSR_STKERR_Msk) != 0U)
    {
        BOARD_DBG_PRINTF("    - Bus fault on stacking\n");
        BOARD_DBG_PRINTF("      Cause: Bus error during exception entry\n");
    }

    if ((cfsr & SCB_CFSR_LSPERR_Msk) != 0U)
    {
        BOARD_DBG_PRINTF("    - Bus fault during lazy FP state preservation\n");
        BOARD_DBG_PRINTF("      Cause: Bus error during floating-point context save\n");
    }
}

static void DBG_PrintUsageFaults(uint32_t cfsr)
{
    if ((cfsr & SCB_CFSR_UNDEFINSTR_Msk) != 0U)
    {
        BOARD_DBG_PRINTF("    - Undefined instruction\n");
        BOARD_DBG_PRINTF("      Cause: Attempted to execute an undefined instruction\n");
    }

    if ((cfsr & SCB_CFSR_INVSTATE_Msk) != 0U)
    {
        BOARD_DBG_PRINTF("    - Invalid state\n");
        BOARD_DBG_PRINTF("      Cause: Attempted to execute instruction with invalid EPSR state\n");
    }

    if ((cfsr & SCB_CFSR_INVPC_Msk) != 0U)
    {
        BOARD_DBG_PRINTF("    - Invalid PC load\n");
        BOARD_DBG_PRINTF("      Cause: Attempted to load invalid PC value\n");
    }

    if ((cfsr & SCB_CFSR_NOCP_Msk) != 0U)
    {
        BOARD_DBG_PRINTF("    - No coprocessor\n");
        BOARD_DBG_PRINTF("      Cause: Attempted to access unavailable coprocessor\n");
    }

    if ((cfsr & SCB_CFSR_UNALIGNED_Msk) != 0U)
    {
        BOARD_DBG_PRINTF("    - Unaligned access\n");
        BOARD_DBG_PRINTF("      Cause: Unaligned memory access with UNALIGN_TRP set\n");
    }

    if ((cfsr & SCB_CFSR_DIVBYZERO_Msk) != 0U)
    {
        BOARD_DBG_PRINTF("    - Division by zero\n");
        BOARD_DBG_PRINTF("      Cause: Division by zero with DIV_0_TRP set\n");
    }
}

static void DBG_PrintRawData(const char* label, const char* start_marker, const char* end_marker,
                            const uint8_t* data, size_t size)
{
    BOARD_DBG_PRINTF("\n%s (Raw - %u bytes):\n", label, size);
    BOARD_DBG_PRINTF("%s\n", start_marker);

    for (size_t i = 0U; i < size; i++)
    {
        if (i % 16U == 0U)
        {
            BOARD_DBG_PRINTF("%04X: ", (unsigned int)i);
        }
        BOARD_DBG_PRINTF("%02X ", data[i]);
        if ((i + 1U) % 16U == 0U)
        {
            BOARD_DBG_PRINTF("\n");
        }
    }
    if (size % 16U != 0U)
    {
        BOARD_DBG_PRINTF("\n");
    }

    BOARD_DBG_PRINTF("%s\n", end_marker);
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
