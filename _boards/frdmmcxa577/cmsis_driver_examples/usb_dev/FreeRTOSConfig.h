/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeRTOSConfig.h for FRDM-MCXA577 (MCXA577, Cortex-M33, 96 MHz)
 * Uses the ARM_CM33_NTZ FreeRTOS port (no TrustZone split).
 */
#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#ifndef __IAR_SYSTEMS_ASM__
#include <stdint.h>
extern uint32_t SystemCoreClock;
#endif

#define configUSE_PREEMPTION                    1
#define configUSE_TICKLESS_IDLE                 0
#define configCPU_CLOCK_HZ                      (SystemCoreClock)
#define configTICK_RATE_HZ                      ((TickType_t)1000)
#define configMAX_PRIORITIES                    56
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#define configMINIMAL_STACK_SIZE                ((uint16_t)256)
#define configMAX_TASK_NAME_LEN                 20
#define configUSE_16_BIT_TICKS                  0
#define configIDLE_SHOULD_YIELD                 1
#define configUSE_MUTEXES                       1
#define configUSE_RECURSIVE_MUTEXES             1
#define configUSE_COUNTING_SEMAPHORES           1
#define configUSE_TASK_NOTIFICATIONS            1
#define configTASK_NOTIFICATION_ARRAY_ENTRIES   3
#define configUSE_TRACE_FACILITY                1
#define configUSE_STATS_FORMATTING_FUNCTIONS    0
#define configQUEUE_REGISTRY_SIZE               10
#define configUSE_QUEUE_SETS                    0
#define configUSE_TIME_SLICING                  1
#define configUSE_NEWLIB_REENTRANT              0
#define configENABLE_BACKWARD_COMPATIBILITY     0
#define configENABLE_MPU                        0
#define configENABLE_TRUSTZONE                  0
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 5
#define configSTACK_DEPTH_TYPE                  uint32_t
#define configTOTAL_HEAP_SIZE                   ((size_t)(64 * 1024))
#define configAPPLICATION_ALLOCATED_HEAP        0
#define configSUPPORT_STATIC_ALLOCATION         1
#define configSUPPORT_DYNAMIC_ALLOCATION        1

/* MCXA577 Cortex-M33: 3 NVIC priority bits implemented (__NVIC_PRIO_BITS = 3).
 * Valid encoded priorities are multiples of 32 (0x00, 0x20, 0x40 ... 0xE0).
 * Library values are the logical 3-bit numbers (0-7). */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY   7
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 2
#define configKERNEL_INTERRUPT_PRIORITY           (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - 3))
#define configMAX_SYSCALL_INTERRUPT_PRIORITY      (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - 3))

#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configCHECK_FOR_STACK_OVERFLOW          2
#define configUSE_MALLOC_FAILED_HOOK            1
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0

#define configGENERATE_RUN_TIME_STATS           0
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         1
#define configUSE_TIMERS                        1
#define configTIMER_TASK_STACK_DEPTH            512
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_PRIORITY               (configMAX_PRIORITIES - 1)

#define INCLUDE_xSemaphoreGetMutexHolder        1
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_xResumeFromISR                  1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_uxTaskGetStackHighWaterMark     1
#define INCLUDE_xTaskGetIdleTaskHandle          0
#define INCLUDE_eTaskGetState                   1
#define INCLUDE_xEventGroupSetBitFromISR        1
#define INCLUDE_xTimerPendFunctionCall          1
#define INCLUDE_xTaskAbortDelay                 1
#define INCLUDE_xTaskGetHandle                  0
#define INCLUDE_xTaskResumeFromISR              1

/* Map FreeRTOS ARM_CM33_NTZ port handler names to CMSIS standard vector names.
 * port.c exports vPortSVCHandler and xPortPendSVHandler; map them to the
 * CMSIS names used in the startup vector table.
 * SysTick_Handler is mapped to xPortSysTickHandler so that cmsis_os2.c can
 * own the SysTick_Handler symbol and call xPortSysTickHandler() internally. */
#define vPortSVCHandler    SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define SysTick_Handler    xPortSysTickHandler

/* Assert macro - halt on failed assertion. */
#define configASSERT(x)                         if ((x) == 0) { taskDISABLE_INTERRUPTS(); for (;;) {} }

/* ARM_CM33_NTZ port settings.
 * MCXA577 has no TrustZone NS/S split; all code runs in the single (secure)
 * world. Tell the port to use secure-world EXC_RETURN values so the first
 * context switch does not trigger CFSR.INVPC. */
#define configRUN_FREERTOS_SECURE_ONLY          1
#define configENABLE_FPU                        1

#endif /* FREERTOS_CONFIG_H */
