/***************************************************************************
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 **************************************************************************/

/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** ThreadX Component                                                     */
/**                                                                       */
/**   Initialize                                                          */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#ifdef TX_INCLUDE_USER_DEFINE_FILE
#include "tx_user.h"
#endif

    EXTERN  _tx_thread_system_stack_ptr
    EXTERN  _tx_initialize_unused_memory
    EXTERN  _tx_timer_interrupt
    EXTERN  __main
    EXTERN  __vector_table
    EXTERN  _tx_thread_current_ptr
    EXTERN  _tx_thread_stack_error_handler
    EXTERN  PendSV_Handler

    SECTION `.text`:CODE:NOROOT(2)
    THUMB
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _tx_initialize_low_level                          Cortex-M33/IAR    */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function is responsible for any low-level processor            */
/*    initialization, including setting up interrupt vectors, setting     */
/*    up a periodic timer interrupt source, saving the system stack       */
/*    pointer for use in ISR processing later, and finding the first      */
/*    available RAM memory address for tx_application_define.             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _tx_initialize_kernel_enter           ThreadX entry function        */
/*                                                                        */
/**************************************************************************/
    PUBLIC  _tx_initialize_low_level
_tx_initialize_low_level:

    /* Disable interrupts during ThreadX initialization.  */
    CPSID   i

    /* Set system stack pointer from vector value.  */
    LDR     r0, =_tx_thread_system_stack_ptr        // Build address of system stack pointer
    LDR     r1, =__vector_table                     // Pickup address of vector table
    LDR     r1, [r1]                                // Pickup reset stack pointer
    STR     r1, [r0]                                // Save system stack pointer

    /* Setup Vector Table Offset Register.  */
    MOV     r0, #0xE000E000                         // Build address of NVIC registers
    LDR     r1, =__vector_table                     // Pickup address of vector table
    STR     r1, [r0, #0xD08]                        // Set vector table address

    /* SysTick are not configured here - to be done by the application  */

    /* Enables faulting or halting when the processor executes an instruction with a divisor of 0 */
    LDR r1, =0x00000010                             // DIV_0_TRP
    STR r1, [r0, #0xD14]                            // Configuration Control Register setting

    /* Configure handler priorities.  */
    LDR     r1, =0x00000000                         // Rsrv, UsgF, BusF, MemM
    STR     r1, [r0, #0xD18]                        // Setup System Handlers 4-7 Priority Registers
    LDR     r1, =0xFF000000                         // SVCl, Rsrv, Rsrv, Rsrv
    STR     r1, [r0, #0xD1C]                        // Setup System Handlers 8-11 Priority Registers
                                                    // Note: SVC must be lowest priority, which is 0xFF
    LDR     r1, =0x40FF0000                         // SysT, PnSV, Rsrv, DbgM
    STR     r1, [r0, #0xD20]                        // Setup System Handlers 12-15 Priority Registers
                                                    // Note: PnSV must be lowest priority, which is 0xFF

    /* Configure system handlers */
    LDR r1, [r0, #0xD24]                            // SHCSR
    ORR r1, #0x70000                                // Enable UsageFault, BusFault, MemManage
    STR r1, [r0, #0xD24]

    /* Return to caller.  */
    BX      lr


    PUBLIC  __tx_SysTickHandler
    PUBLIC SysTick_Handler
SysTick_Handler:
__tx_SysTickHandler:
    PUSH    {r0,lr}     // Save LR (and dummy r0 to maintain stack alignment)
#if (defined(TX_ENABLE_EXECUTION_CHANGE_NOTIFY) || defined(TX_EXECUTION_PROFILE_ENABLE))
    BL      _tx_execution_isr_enter             // Call the ISR enter function
#endif
    BL      _tx_timer_interrupt
#if (defined(TX_ENABLE_EXECUTION_CHANGE_NOTIFY) || defined(TX_EXECUTION_PROFILE_ENABLE))
    BL      _tx_execution_isr_exit              // Call the ISR exit function
#endif
    POP     {r0,lr}
    BX      lr

    PUBLIC SVC_Handler
SVC_Handler
  B     PendSV_Handler

    END
