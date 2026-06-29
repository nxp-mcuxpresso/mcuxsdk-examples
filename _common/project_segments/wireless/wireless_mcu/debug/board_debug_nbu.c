/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/************************************************************************************
 * Include
 ************************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "fwk_workq.h"
#include "fwk_nbu_dbg.h"
#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "fsl_os_abstraction.h"
#include "board_debug_utils.h"
#if defined(BOARD_NBUDBG_HCI_LOGGER)
#include "board_debug_nbu_port.h"
#if defined(BOARD_NBUDBG_HCI_LOG_ON_FAULT_ONLY) && (BOARD_NBUDBG_HCI_LOG_ON_FAULT_ONLY > 0)
#include "fwk_platform_dbg.h"
#endif
#endif
#if defined(BOARD_NBUDBG_APP_LOGGER) && (BOARD_NBUDBG_APP_LOGGER > 0) && \
    defined(gAppUseSerialManager_c) && (gAppUseSerialManager_c > 0)
#include "fsl_component_serial_manager.h"
#include "app.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Set to 1 to add direction byte before each packet, 0 for no direction marker */
#ifndef BOARD_NBUDBG_HCI_LOG_W_DIRECTION_MARKER
#define BOARD_NBUDBG_HCI_LOG_W_DIRECTION_MARKER 1U
#endif

/* Set to 1 to Base64-encode each HCI packet before writing it to the log port.
 * This is intended for ports that share the line with human-readable text (e.g.
 * the main serial port): Base64 keeps the stream ASCII-safe and each packet is
 * emitted as a single line "@<base64(...)>\r\n" so the host tool can extract it. */
#ifndef BOARD_NBUDBG_HCI_LOG_BASE64
#define BOARD_NBUDBG_HCI_LOG_BASE64 0U
#endif

#if defined(BOARD_NBUDBG_HCI_LOGGER)
/* Maximum raw bytes assembled for a single packet:
 * optional direction marker(0 or 1, depending on
 * BOARD_NBUDBG_HCI_LOG_W_DIRECTION_MARKER) + packet_type(1) + maximum HCI
 * payload (258). The whole packet is assembled into this single buffer and
 * emitted with one port write so the bytes can never be split or interleaved
 * by another writer sharing the port. */
#define BOARD_NBUDBG_HCI_LOG_MAX_RAW_SIZE (BOARD_NBUDBG_HCI_LOG_W_DIRECTION_MARKER + 259U)

#if (BOARD_NBUDBG_HCI_LOG_BASE64 == 1U)
/* Single-character line marker prefixing every Base64-encoded packet. It must
 * not belong to the Base64 alphabet (A-Z a-z 0-9 + / =) so the host can detect
 * packet lines unambiguously. */
#define BOARD_NBUDBG_HCI_LOG_BASE64_PREFIX '@'

/* Base64 expands 3 input bytes to 4 output chars (rounded up). */
#define BOARD_NBUDBG_HCI_LOG_MAX_B64_SIZE (((BOARD_NBUDBG_HCI_LOG_MAX_RAW_SIZE + 2U) / 3U) * 4U)

/* One framed line assembled in a single buffer: prefix(1) + base64 + newline(1).
 * The complete line is emitted with a single port write so the prefix, the
 * Base64 payload and the newline can never be split by another writer sharing
 * the serial port (which would break host-side line parsing). */
/* prefix(1) + base64 + new line */
#define BOARD_NBUDBG_HCI_LOG_MAX_LINE_SIZE (1U + BOARD_NBUDBG_HCI_LOG_MAX_B64_SIZE + 2U)
#endif
#endif /* BOARD_NBUDBG_HCI_LOGGER */

#if (defined(gDebugConsoleEnable_d) && (gDebugConsoleEnable_d == 1)) && defined(BOARD_NBUDBG_HCI_LOGGER)
#warning "HCI Logger: Debug console is enabled. \
          Same UART instance will be shared between debug prints and HCI logging. \
          This will lead to conflict."
#endif

/* Buffer for formatted output */
#define BOARD_NBUDBG_CONSOLE_BUFFER_SIZE  128

/* NBU Debug Warning Print Level Configuration:
 * 1 = Print warning indication only (default)
 * 2 = Extended print: warning indication + BLE debug data dump
 * Note: Fault/Error analysis is always printed in full regardless of this setting
 */
#ifndef BOARD_NBUDBG_NBU_WARNING_PRINT_LEVEL
#define BOARD_NBUDBG_NBU_WARNING_PRINT_LEVEL 1
#endif

#ifndef BOARD_NBUDBG_EVENT_QUEUE_SIZE
#define BOARD_NBUDBG_EVENT_QUEUE_SIZE (3U)
#endif

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
static int  BOARD_NBUDBG_Print(const char *format, ...);
static void BOARD_NBUDBG_ProcessDebugStruct(fwk_work_t *work);
static void BOARD_NBUDBG_NbuEventCallback(const nbu_dbg_context_t *nbu_event);
static void BOARD_NBUDBG_PrintMemoryManagementFaults(uint32_t cfsr);
static void BOARD_NBUDBG_PrintBusFaults(uint32_t cfsr);
static void BOARD_NBUDBG_PrintUsageFaults(uint32_t cfsr);
static void BOARD_NBUDBG_PrintRawData(const char* label, const char* start_marker, const char* end_marker,
                             const uint8_t* data, size_t size);
#if defined(BOARD_NBUDBG_HCI_LOGGER)
static void BOARD_NBUDBG_HciLogCallback(uint8_t packet_type, const uint8_t *data, uint16_t len, bool is_rx);
#if (BOARD_NBUDBG_HCI_LOG_BASE64 == 1U)
static void BOARD_NBUDBG_HciLogEmitBase64(uint8_t packet_type, const uint8_t *data, uint16_t len, bool is_rx);
#else
static void BOARD_NBUDBG_HciLogEmitRaw(uint8_t packet_type, const uint8_t *data, uint16_t len, bool is_rx);
#endif
#endif

/************************************************************************************
 * Private memory declarations
 ************************************************************************************/
static nbu_debug_struct_t debug_info;
#if defined(BOARD_NBUDBG_APP_LOGGER) && (BOARD_NBUDBG_APP_LOGGER > 0) && \
    defined(gAppUseSerialManager_c) && (gAppUseSerialManager_c > 0)
static SERIAL_MANAGER_WRITE_HANDLE_DEFINE(s_nbuDbgWriteHandle);
#endif

static fwk_work_t dbg_process_work = {
    .handler = BOARD_NBUDBG_ProcessDebugStruct,
};

static OSA_MSGQ_HANDLE_DEFINE(dbgMsgQueue, BOARD_NBUDBG_EVENT_QUEUE_SIZE, sizeof(nbu_dbg_context_t));

/************************************************************************************
*************************************************************************************
* Private type definitions and macros
*************************************************************************************
************************************************************************************/
#define BOARD_NBUDBG_PRINTF (void)BOARD_NBUDBG_Print

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/**
 * \brief Print in Serial console print
 * \param format Format string and variable arguments
 * \return Number of characters printed
 */
static int BOARD_NBUDBG_Print(const char *format, ...)
{
    int len = 0;
#if defined(BOARD_NBUDBG_APP_LOGGER) && (BOARD_NBUDBG_APP_LOGGER > 0) && \
    defined(gAppUseSerialManager_c) && (gAppUseSerialManager_c > 0)
    static char printBuf[BOARD_NBUDBG_CONSOLE_BUFFER_SIZE];
    va_list args;

    serial_manager_status_t status;

    /* Format the string */
    va_start(args, format);
    len = vsnprintf(printBuf, BOARD_NBUDBG_CONSOLE_BUFFER_SIZE, format, args);
    va_end(args);

    /* Ensure null termination and prevent buffer overflow */
    if (len >= BOARD_NBUDBG_CONSOLE_BUFFER_SIZE)
    {
        len = BOARD_NBUDBG_CONSOLE_BUFFER_SIZE - 1;
    }
    printBuf[len] = '\0';

    /* Send via Serial Manager */
    status = SerialManager_WriteBlocking((serial_write_handle_t)s_nbuDbgWriteHandle, (uint8_t *)printBuf, len);
    if (status != kStatus_SerialManager_Success)
    {
        len = 0;
        assert(false);
    }
#endif
    return len;
}

static void BOARD_NBUDBG_ProcessDebugStruct(fwk_work_t *work)
{
    (void)work;

    nbu_dbg_context_t dbg_nbu_event;
    nbu_dbg_info_t *nbu_dbg_info;
    reg_info_t *regs;

    do
    {
        /* Check if there is a message in the queue
         * Important: do not set a blocking time to prevent blocking the system workqueue
         */
        if (OSA_MsgQGet(dbgMsgQueue, (void *)&dbg_nbu_event, 0) != KOSA_StatusSuccess)
        {
            break;
        }

        if (dbg_nbu_event.nbu_warning_count > 0U)
        {
            BOARD_NBUDBG_PRINTF("WARNING: %u New NBU Warnings detected\n", dbg_nbu_event.nbu_warning_count);
        }

        if (dbg_nbu_event.nbu_is_halted != 0U)
        {
            BOARD_NBUDBG_PRINTF("ERROR: NBU is stuck - possible stall or deadlock detected\n");
        }

        if (NBUDBG_StructDump(&debug_info) != 0)
        {
            BOARD_NBUDBG_PRINTF("ERROR: Failed to retrieve NBU debug information\n");
            break;
        }

        BOARD_NBUDBG_PRINTF("NBU Debug version: 0x%04X\n", debug_info.version);
        if (debug_info.version != (uint16_t)NBUDBG_VERSION)
        {
            BOARD_NBUDBG_PRINTF("!! Host Debug version 0x%04X != NBU debug version 0x%04X !!\n", (uint16_t)NBUDBG_VERSION, debug_info.version);
            BOARD_NBUDBG_PRINTF("!! The following analysis may be incorrect !!\n");
        }
#if (BOARD_NBUDBG_NBU_WARNING_PRINT_LEVEL == 2)
        if (dbg_nbu_event.nbu_warning_count > 0U)
        {
            BOARD_NBUDBG_PRINTF("=== Warning Circular Table ===\n");
            for(uint8_t i = 0U; i < NBUDBG_MAX_NB_WARNINGS; i++)
            {
                if (i == debug_info.nbu_dbg_info.warning_index)
                {
                    BOARD_NBUDBG_PRINTF("->");
                }
                BOARD_NBUDBG_PRINTF("%u\n", debug_info.nbu_dbg_info.warnings[i]);
            }
        }
#endif
        if ((dbg_nbu_event.nbu_error_count > 0U))
        {
            nbu_dbg_info = &debug_info.nbu_dbg_info;
            regs = &debug_info.nbu_dbg_info.u.reg_info;
            BOARD_NBUDBG_PRINTF("\n=== NBU Fault/Assert Analysis ===\n\n");
            if (nbu_dbg_info->exception_id == NBUDBG_EXCEPTION_ID_FOR_ASSERT_MAGIC)
            {
                /* Assert on NBU side */
                BOARD_NBUDBG_PRINTF("NBU Assert Detected\n");
                BOARD_NBUDBG_PRINTF("  Line: %u\n", nbu_dbg_info->u.assert_info.line);
                BOARD_NBUDBG_PRINTF("  File name: %s\n", nbu_dbg_info->u.assert_info.file_name);
            }
            else
            {
                /* Fault on NBU side */
                BOARD_NBUDBG_PRINTF("NBU Fault Detected\n");
                BOARD_NBUDBG_PRINTF("Exception Information:\n");
                BOARD_NBUDBG_PRINTF("  Exception ID: 0x%08X\n", nbu_dbg_info->exception_id);
                BOARD_NBUDBG_PRINTF("  NBU SHA1    : 0x%08X\n", nbu_dbg_info->nbu_sha1);

                BOARD_NBUDBG_PRINTF("\nProcessor State:\n");
                BOARD_NBUDBG_PRINTF("  PC  (Program Counter): 0x%08X\n", regs->pc);
                BOARD_NBUDBG_PRINTF("  LR  (Link Register)  : 0x%08X\n", regs->lr);
                BOARD_NBUDBG_PRINTF("  SP  (Stack Pointer)  : 0x%08X\n", regs->sp);
                BOARD_NBUDBG_PRINTF("  PSR (Program Status) : 0x%08X\n", regs->psr);

                BOARD_NBUDBG_PRINTF("\nGeneral Purpose Registers:\n");
                BOARD_NBUDBG_PRINTF("  R0:  0x%08X  R1:  0x%08X  R2:  0x%08X  R3:  0x%08X\n",
                       regs->r0, regs->r1, regs->r2, regs->r3);
                BOARD_NBUDBG_PRINTF("  R4:  0x%08X  R5:  0x%08X  R6:  0x%08X  R7:  0x%08X\n",
                       regs->r4, regs->r5, regs->r6, regs->r7);
                BOARD_NBUDBG_PRINTF("  R8:  0x%08X  R9:  0x%08X  R10: 0x%08X  R11: 0x%08X\n",
                       regs->r8, regs->r9, regs->r10, regs->r11);
                BOARD_NBUDBG_PRINTF("  R12: 0x%08X\n", regs->r12);

                BOARD_NBUDBG_PRINTF("\nFault Status Analysis:\n");
                BOARD_NBUDBG_PRINTF("  CFSR (Configurable Fault Status): 0x%08X\n", regs->cfsr);

                if (regs->cfsr != 0U)
                {
                    /* Check Memory Management Faults */
                    if ((regs->cfsr & 0xFFU) != 0U)
                    {
                        BOARD_NBUDBG_PRINTF("\n  Memory Management Faults Detected:\n");
                        BOARD_NBUDBG_PrintMemoryManagementFaults(regs->cfsr);

                        if ((regs->cfsr & SCB_CFSR_MMARVALID_Msk) != 0U)
                        {
                            BOARD_NBUDBG_PRINTF("    Faulting Address (MMFAR): 0x%08X\n", regs->xfar.mmfar);
                        }
                    }

                    /* Check Bus Faults */
                    if ((regs->cfsr & 0xFF00U) != 0U)
                    {
                        BOARD_NBUDBG_PRINTF("\n  Bus Faults Detected:\n");
                        BOARD_NBUDBG_PrintBusFaults(regs->cfsr);

                        if ((regs->cfsr & SCB_CFSR_BFARVALID_Msk) != 0U)
                        {
                            BOARD_NBUDBG_PRINTF("    Faulting Address (BFAR): 0x%08X\n", regs->xfar.bfar);
                        }
                    }

                    /* Check Usage Faults */
                    if ((regs->cfsr & 0xFFFF0000U) != 0U)
                    {
                        BOARD_NBUDBG_PRINTF("\n  Usage Faults Detected:\n");
                        BOARD_NBUDBG_PrintUsageFaults(regs->cfsr);
                    }
                }
                else
                {
                    BOARD_NBUDBG_PRINTF("  No fault status flags set in CFSR\n");
                }
            }

            BOARD_NBUDBG_PRINTF("\nExecution Context:\n");
            if (NBUDBG_IS_HANDLER_MODE(nbu_dbg_info->execution_context.u.handler_irq))
            {
                uint32_t irq_number = NBUDBG_GET_IRQ_NUMBER(nbu_dbg_info->execution_context.u.handler_irq);
                BOARD_NBUDBG_PRINTF("  Mode: Handler Mode (Interrupt Context)\n");
                BOARD_NBUDBG_PRINTF("  IRQ Number: %u\n", irq_number);
                (void) irq_number; /* Supress warning when debug console (PRINTF) is disabled */
            }
            else
            {
                BOARD_NBUDBG_PRINTF("  Mode: Thread Mode\n");
                BOARD_NBUDBG_PRINTF("  Thread Address: 0x%08X\n", nbu_dbg_info->execution_context.u.thread_info.thread_addr);
                BOARD_NBUDBG_PRINTF("  Thread Name: %.8s\n", nbu_dbg_info->execution_context.u.thread_info.thread_name);
            }

            BOARD_NBUDBG_PRINTF("\n=== End of NBU Fault/Assert Analysis ===\n\n");
        }
#if (BOARD_NBUDBG_NBU_WARNING_PRINT_LEVEL == 1)
        /* Dont print BLE debug data on warning */
        if ((dbg_nbu_event.nbu_is_halted != 0U) || (dbg_nbu_event.nbu_error_count > 0U))
#endif
        {
            /* Raw dump of BLE debug data */
            BOARD_NBUDBG_PrintRawData("BLE Debug Data", "DBG_BLE_START", "DBG_BLE_END",
                             debug_info.dbg_ble, NBUDBG_BLE_STRUCT_SIZE);
        }
    } while (false);
}
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
static void BOARD_NBUDBG_NbuEventCallback(const nbu_dbg_context_t *nbu_event)
{
    /* Non blocking call, safe to be called on the IDLE task */
    osa_status_t status = OSA_MsgQPut(dbgMsgQueue, (void *)nbu_event);
    if (status == KOSA_StatusSuccess)
    {
        if (WORKQ_Submit(&dbg_process_work) < 0)
        {
            assert(false);
        }
    }
    else
    {
        assert(false);
    }
}

static void BOARD_NBUDBG_PrintMemoryManagementFaults(uint32_t cfsr)
{
    if ((cfsr & SCB_CFSR_IACCVIOL_Msk) != 0U)
    {
        BOARD_NBUDBG_PRINTF("    - Instruction access violation\n");
        BOARD_NBUDBG_PRINTF("      Cause: Attempted to execute from a region marked as non-executable\n");
    }

    if ((cfsr & SCB_CFSR_DACCVIOL_Msk) != 0U)
    {
        BOARD_NBUDBG_PRINTF("    - Data access violation\n");
        BOARD_NBUDBG_PRINTF("      Cause: Attempted to access a memory region without proper permissions\n");
    }

    if ((cfsr & SCB_CFSR_MUNSTKERR_Msk) != 0U)
    {
        BOARD_NBUDBG_PRINTF("    - Memory management fault on unstacking\n");
        BOARD_NBUDBG_PRINTF("      Cause: Error during exception return stack pop operation\n");
    }

    if ((cfsr & SCB_CFSR_MSTKERR_Msk) != 0U)
    {
        BOARD_NBUDBG_PRINTF("    - Memory management fault on stacking\n");
        BOARD_NBUDBG_PRINTF("      Cause: Error during exception entry stack push operation\n");
    }

    if ((cfsr & SCB_CFSR_MLSPERR_Msk) != 0U)
    {
        BOARD_NBUDBG_PRINTF("    - Memory management fault during lazy FP state preservation\n");
        BOARD_NBUDBG_PRINTF("      Cause: Error during floating-point context save\n");
    }
}

static void BOARD_NBUDBG_PrintBusFaults(uint32_t cfsr)
{
    if ((cfsr & SCB_CFSR_IBUSERR_Msk) != 0U)
    {
        BOARD_NBUDBG_PRINTF("    - Instruction bus error\n");
        BOARD_NBUDBG_PRINTF("      Cause: Bus fault on instruction fetch\n");
    }

    if ((cfsr & SCB_CFSR_PRECISERR_Msk) != 0U)
    {
        BOARD_NBUDBG_PRINTF("    - Precise data bus error\n");
        BOARD_NBUDBG_PRINTF("      Cause: Bus fault on data access (address in BFAR is valid)\n");
    }

    if ((cfsr & SCB_CFSR_IMPRECISERR_Msk) != 0U)
    {
        BOARD_NBUDBG_PRINTF("    - Imprecise data bus error\n");
        BOARD_NBUDBG_PRINTF("      Cause: Bus fault on data access (address in BFAR may not be valid)\n");
    }

    if ((cfsr & SCB_CFSR_UNSTKERR_Msk) != 0U)
    {
        BOARD_NBUDBG_PRINTF("    - Bus fault on unstacking\n");
        BOARD_NBUDBG_PRINTF("      Cause: Bus error during exception return\n");
    }

    if ((cfsr & SCB_CFSR_STKERR_Msk) != 0U)
    {
        BOARD_NBUDBG_PRINTF("    - Bus fault on stacking\n");
        BOARD_NBUDBG_PRINTF("      Cause: Bus error during exception entry\n");
    }

    if ((cfsr & SCB_CFSR_LSPERR_Msk) != 0U)
    {
        BOARD_NBUDBG_PRINTF("    - Bus fault during lazy FP state preservation\n");
        BOARD_NBUDBG_PRINTF("      Cause: Bus error during floating-point context save\n");
    }
}

static void BOARD_NBUDBG_PrintUsageFaults(uint32_t cfsr)
{
    if ((cfsr & SCB_CFSR_UNDEFINSTR_Msk) != 0U)
    {
        BOARD_NBUDBG_PRINTF("    - Undefined instruction\n");
        BOARD_NBUDBG_PRINTF("      Cause: Attempted to execute an undefined instruction\n");
    }

    if ((cfsr & SCB_CFSR_INVSTATE_Msk) != 0U)
    {
        BOARD_NBUDBG_PRINTF("    - Invalid state\n");
        BOARD_NBUDBG_PRINTF("      Cause: Attempted to execute instruction with invalid EPSR state\n");
    }

    if ((cfsr & SCB_CFSR_INVPC_Msk) != 0U)
    {
        BOARD_NBUDBG_PRINTF("    - Invalid PC load\n");
        BOARD_NBUDBG_PRINTF("      Cause: Attempted to load invalid PC value\n");
    }

    if ((cfsr & SCB_CFSR_NOCP_Msk) != 0U)
    {
        BOARD_NBUDBG_PRINTF("    - No coprocessor\n");
        BOARD_NBUDBG_PRINTF("      Cause: Attempted to access unavailable coprocessor\n");
    }

    if ((cfsr & SCB_CFSR_UNALIGNED_Msk) != 0U)
    {
        BOARD_NBUDBG_PRINTF("    - Unaligned access\n");
        BOARD_NBUDBG_PRINTF("      Cause: Unaligned memory access with UNALIGN_TRP set\n");
    }

    if ((cfsr & SCB_CFSR_DIVBYZERO_Msk) != 0U)
    {
        BOARD_NBUDBG_PRINTF("    - Division by zero\n");
        BOARD_NBUDBG_PRINTF("      Cause: Division by zero with DIV_0_TRP set\n");
    }
}

static void BOARD_NBUDBG_PrintRawData(const char* label, const char* start_marker, const char* end_marker,
                            const uint8_t* data, size_t size)
{
    BOARD_NBUDBG_PRINTF("\n%s (Raw - %u bytes):\n", label, size);
    BOARD_NBUDBG_PRINTF("%s\n", start_marker);

    for (size_t i = 0U; i < size; i++)
    {
        if (i % 16U == 0U)
        {
            BOARD_NBUDBG_PRINTF("%04X: ", (unsigned int)i);
        }
        BOARD_NBUDBG_PRINTF("%02X ", data[i]);
        if ((i + 1U) % 16U == 0U)
        {
            BOARD_NBUDBG_PRINTF("\n");
        }
    }
    if (size % 16U != 0U)
    {
        BOARD_NBUDBG_PRINTF("\n");
    }

    BOARD_NBUDBG_PRINTF("%s\n", end_marker);
}

#if defined(BOARD_NBUDBG_HCI_LOGGER)
#if (BOARD_NBUDBG_HCI_LOG_BASE64 == 1U)
/**
 * \brief Assemble a single Base64-framed HCI log line and emit it atomically
 *
 * The large work buffers live in this dedicated helper so that their stack
 * footprint is only reserved when a packet is actually logged. The callback
 * drops most packets (for example when fault-only logging is enabled), so
 * keeping the buffers out of the callback frame avoids paying that stack cost
 * on every dropped packet.
 *
 * \param[in] packet_type HCI packet type byte
 * \param[in] data        pointer to the HCI payload (must not be NULL)
 * \param[in] len         number of payload bytes (must be greater than 0)
 * \param[in] is_rx       true when the packet is controller-to-host (RX)
 */
static void BOARD_NBUDBG_HciLogEmitBase64(uint8_t packet_type, const uint8_t *data, uint16_t len, bool is_rx)
{
    /* Assemble direction + type + payload, then emit a single line
     * "@<base64>\r\n" so the host tool can extract one packet per line. */
    uint8_t  raw_buf[BOARD_NBUDBG_HCI_LOG_MAX_RAW_SIZE];
    char     line_buf[BOARD_NBUDBG_HCI_LOG_MAX_LINE_SIZE];
    uint32_t raw_len  = 0U;
    uint32_t line_len = 0U;
    uint16_t copy_len = len;
    uint32_t max_payload;

#if (BOARD_NBUDBG_HCI_LOG_W_DIRECTION_MARKER == 1U)
    raw_buf[raw_len] = (uint8_t)((is_rx != false) ? 1U : 0U);
    raw_len++;
#else
    (void)is_rx;
#endif
    raw_buf[raw_len] = packet_type;
    raw_len++;

    /* Clamp the payload to the buffer capacity to avoid overflow */
    max_payload = (uint32_t)BOARD_NBUDBG_HCI_LOG_MAX_RAW_SIZE - raw_len;
    if ((uint32_t)copy_len > max_payload)
    {
        copy_len = (uint16_t)max_payload;
    }

    (void)memcpy(&raw_buf[raw_len], data, (size_t)copy_len);
    raw_len += copy_len;

    /* Build the full framed line prefix + base64 + newline in one buffer and
     * emit it with a single port write. This keeps the whole line atomic from
     * the host parser point of view. The scheduler must stay enabled here: the
     * serialmgr port write blocks on OSA_Wait, which would deadlock if the
     * scheduler were disabled. */
    line_buf[line_len] = (char)BOARD_NBUDBG_HCI_LOG_BASE64_PREFIX;
    line_len++;
    line_len += BOARD_DBG_Base64Encode(raw_buf, raw_len, &line_buf[line_len]);
    /* Terminate with CRLF */
    line_buf[line_len] = '\r';
    line_len++;
    line_buf[line_len] = '\n';
    line_len++;

    (void)BOARD_DbgNbuPortWrite((const uint8_t *)line_buf, (uint16_t)line_len);
}
#else
/**
 * \brief Assemble a raw HCI log packet and emit it atomically
 *
 * As with the Base64 helper, the work buffer lives here so its stack footprint
 * is only reserved when a packet is actually logged.
 *
 * \param[in] packet_type HCI packet type byte
 * \param[in] data        pointer to the HCI payload (must not be NULL)
 * \param[in] len         number of payload bytes (must be greater than 0)
 * \param[in] is_rx       true when the packet is controller-to-host (RX)
 */
static void BOARD_NBUDBG_HciLogEmitRaw(uint8_t packet_type, const uint8_t *data, uint16_t len, bool is_rx)
{
    /* Raw mode: assemble direction + type + payload into one buffer and emit
     * it with a single port write. A single write keeps the bytes contiguous
     * so another writer sharing the port cannot interleave them, and the
     * scheduler stays enabled (the serialmgr port write blocks on OSA_Wait,
     * which would deadlock if the scheduler were disabled). */
    uint8_t  raw_buf[BOARD_NBUDBG_HCI_LOG_MAX_RAW_SIZE];
    uint32_t raw_len  = 0U;
    uint16_t copy_len = len;
    uint32_t max_payload;

#if (BOARD_NBUDBG_HCI_LOG_W_DIRECTION_MARKER == 1U)
    raw_buf[raw_len] = (uint8_t)((is_rx != false) ? 1U : 0U);
    raw_len++;
#else
    (void)is_rx;
#endif
    raw_buf[raw_len] = packet_type;
    raw_len++;

    /* Clamp the payload to the buffer capacity to avoid overflow */
    max_payload = (uint32_t)BOARD_NBUDBG_HCI_LOG_MAX_RAW_SIZE - raw_len;
    if ((uint32_t)copy_len > max_payload)
    {
        copy_len = (uint16_t)max_payload;
    }
    (void)memcpy(&raw_buf[raw_len], data, (size_t)copy_len);
    raw_len += copy_len;

    (void)BOARD_DbgNbuPortWrite(raw_buf, (uint16_t)raw_len);
}
#endif

static void BOARD_NBUDBG_HciLogCallback(uint8_t packet_type, const uint8_t *data, uint16_t len, bool is_rx)
{
    bool log_allowed = true;

    do
    {
#if defined(BOARD_NBUDBG_HCI_LOG_ON_FAULT_ONLY) && (BOARD_NBUDBG_HCI_LOG_ON_FAULT_ONLY > 0)
        /* Only forward packets once an NBU fatal fault has been detected.
         * PLATFORM_IsNbuFaultSet() reads the fault status register directly, so it is
         * accurate and immediate (no dependency on the queued notification callback).
         * Packets received before the fault are dropped; from the fault onward every
         * packet (including the automatically generated coredump HCI) is logged. */
        log_allowed = PLATFORM_IsNbuFaultSet();
#endif

        /* Drop early so the large assembly buffers (held in the emit helpers
         * below) never cost stack on the dropped-packet path. */
        if ((log_allowed == false) || (data == NULL) || (len == 0U))
        {
            break;
        }

#if (BOARD_NBUDBG_HCI_LOG_BASE64 == 1U)
        BOARD_NBUDBG_HciLogEmitBase64(packet_type, data, len, is_rx);
#else
        BOARD_NBUDBG_HciLogEmitRaw(packet_type, data, len, is_rx);
#endif
    } while (false);
}
#endif /* BOARD_NBUDBG_HCI_LOGGER */

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

int BOARD_DbgNbuInit(void)
{
    osa_status_t osa_status;
    int ret = 0;

    do
    {
        NBUDBG_RegisterNbuDebugNotificationCb(BOARD_NBUDBG_NbuEventCallback);
        /* Configure to send debug structure via HCI vendor events */
        NBUDBG_ConfigureHciVendorEvent(NBUDBG_HCI_EVENT_DEBUG_STRUCT | NBUDBG_HCI_EVENT_STALL_EVENT);
#if defined(BOARD_NBUDBG_HCI_LOGGER)
        /* Register HCI logger callback */
        NBUDBG_RegisterHciLogCallback(BOARD_NBUDBG_HciLogCallback);
        /* Init HCI log port */
        ret = BOARD_DbgNbuPortInit();
        if (ret < 0)
        {
            break;
        }
#endif
        ret = WORKQ_InitSysWorkQ();
        if (ret < 0)
        {
            break;
        }

        osa_status = OSA_MsgQCreate(dbgMsgQueue, BOARD_NBUDBG_EVENT_QUEUE_SIZE, sizeof(nbu_dbg_context_t));
        if (osa_status != KOSA_StatusSuccess)
        {
            ret = -1;
            break;
        }

#if defined(BOARD_NBUDBG_APP_LOGGER) && (BOARD_NBUDBG_APP_LOGGER > 0) && \
    defined(gAppUseSerialManager_c) && (gAppUseSerialManager_c > 0)
        /*open write handle*/
        if(SerialManager_OpenWriteHandle((serial_handle_t)gSerMgrIf, (serial_write_handle_t)s_nbuDbgWriteHandle) != kStatus_SerialManager_Success)
        {
            ret = -1;
            break;
        }
#endif
    } while (false);

    return ret;
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
