/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "app.h"

#include "fsl_os_abstraction.h"
#include "zephyr_headers/debug/coredump.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define COREDUMP_HEADER_SIZE (sizeof(struct coredump_hdr_t))

union
{
    uint8_t u8CoredumpHeaderBuf[COREDUMP_HEADER_SIZE];
    uint32_t u32CoredumpHeaderBuf[COREDUMP_HEADER_SIZE / 4];
} CoredumpHeader;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void DEMO_TriggerUseFault(void);

void DEMO_MainTask(void *parameters);
void DEMO_FaultTrigTask(void *parameters);
/*******************************************************************************
 * Variables
 ******************************************************************************/

#define MAIN_TASK_PRIO 1
static OSA_TASK_HANDLE_DEFINE(main_task);
static OSA_TASK_DEFINE(DEMO_MainTask, MAIN_TASK_PRIO, 1, 1024, 0);

#define FAULT_TRIGGER_TASK_PRIO 2
static OSA_TASK_HANDLE_DEFINE(fault_trigger_task);
static OSA_TASK_DEFINE(DEMO_FaultTrigTask, FAULT_TRIGGER_TASK_PRIO, 1, 1024, 0);

/*******************************************************************************
 * Code
 ******************************************************************************/

void DEMO_MainTask(void *parameters)
{
    while (1)
    {
        PRINTF("Press any key to trigger usagefault...\r\n");
        OSA_TaskYield();
    }
}

void DEMO_FaultTrigTask(void *parameters)
{
    while (1)
    {
        GETCHAR();
        DEMO_TriggerUseFault();
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    if (BOARD_SoftwareResetAsserted() == false)
    {
        OSA_Init();
        PRINTF("Coredump Fault Example Start!\r\n");
        OSA_TaskCreate((osa_task_handle_t)main_task, OSA_TASK(DEMO_MainTask), (osa_task_param_t)NULL);
        OSA_TaskCreate((osa_task_handle_t)fault_trigger_task, OSA_TASK(DEMO_FaultTrigTask), (osa_task_param_t)NULL);

        /* Start scheduler. */
        OSA_Start();
    }
    else
    {
        PRINTF("Coredump Fault Example Rebooted!!!\r\n");
        PRINTF("Press any key to check coredump reason...\r\n");
        GETCHAR();
        if (coredump_query(COREDUMP_QUERY_HAS_STORED_DUMP, NULL) != 0)
        {
            if (coredump_query(COREDUMP_QUERY_GET_ERROR, NULL) != 0)
            {
                PRINTF("[Error] Stored dump have error!\r\n");
            }
            int32_t storedDumpSize = coredump_query(COREDUMP_QUERY_GET_STORED_DUMP_SIZE, NULL);
            if (storedDumpSize > 0)
            {
                PRINTF("[Info] Size: %d.\r\n", storedDumpSize);
                struct coredump_cmd_copy_arg tmpArgs = {
                    .offset = 0U,
                    .length = COREDUMP_HEADER_SIZE,
                    .buffer = CoredumpHeader.u8CoredumpHeaderBuf,
                };
                if (coredump_cmd(COREDUMP_CMD_COPY_STORED_DUMP, &tmpArgs) == COREDUMP_HEADER_SIZE)
                {
                    if (CoredumpHeader.u32CoredumpHeaderBuf[2] == K_ERR_ARM_USAGE_DIV_0)
                    {
                        PRINTF(
                            "[Info] Reason: Usagefault caused by an integer divsion by "
                            "zero!\r\n");
                    }
                }
                else
                {
                    PRINTF("[Error] Fail copy stored dump!\r\n");
                }
            }
            else
            {
                PRINTF("[Error] Stored dump size query fail!\r\n");
            }
        }
    }

    while (1)
    {
    }
}

void DEMO_TriggerUseFault(void)
{
    volatile uint32_t x = 1UL;
    volatile uint32_t y = 0UL;

    /* Enable division by zero cause a usagefault. */
    SCB->CCR |= SCB_CCR_DIV_0_TRP_Msk;
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk;
    SCB->AIRCR |= SCB_AIRCR_BFHFNMINS_Msk;

    /* Trigger usefault by divide 0. */
    x = x / y;
}
