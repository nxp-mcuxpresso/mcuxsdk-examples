/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "mcmgr.h"

#include "fsl_sss_mgmt.h"
#include "fsl_sss_sscp.h"
#include "fsl_sscp_mu.h"

#include "common_definitions.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/**
 * We place our global intercore context into the rpmsg shared memory section.
 * This section is reserved for intercore communication and is only used
 * for the purpose of saving return values of the second core's tasks.
 * If an application implements the two cores as fully independent processes,
 * no such context is necessary.
 */
#if defined(__ICCARM__) /* IAR Workbench */
#pragma location = "rpmsg_sh_mem_section"
ele_multicore_ctx_t g_ctx;
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION) /* Keil MDK */
ele_multicore_ctx_t g_ctx __attribute__((section("rpmsg_sh_mem_section")));
#elif defined(__GNUC__)
ele_multicore_ctx_t g_ctx __attribute__((section(".noinit.$rpmsg_sh_mem")));
#else
#error "Unsupported compiler"
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    char ch;

    /* Init board hardware.*/
    BOARD_InitHardware();

    /* Initialize the global context in shared memory and get a pointer to it */
    memset(&g_ctx, 0, sizeof(ele_multicore_ctx_t));
    g_ctx_ptr = &g_ctx;

    /* Initialize MCMGR, install generic event handlers */
    (void)MCMGR_Init();

    /* Init semaphore */
    SEMA42_Init(APP_SEMA42);
    (void)SEMA42_ResetGate(APP_SEMA42, APP_SEMA42_GATE);

    /* Boot Secondary core application */
    (void)MCMGR_StartCore(kMCMGR_Core1, (void *)(char *)CORE1_BOOT_ADDRESS, (uint32_t)&g_ctx, kMCMGR_Start_Synchronous);
    (void)PRINTF("Secondary core started\r\n\r\n");

    /* Initialize HW. Or just core-side SW if the given core got here late. */
    DO_TASK(init_s200(),
            g_ctx_ptr->core0_tasks[0].task_retval,
            g_ctx_ptr->core0_tasks[0].task_completed);

    /* Show which of the two cores got here first. Unless somehow delayed,
     * it was probably NBU.
     */
    (void)mutex_lock();
    (void)PRINTF("Initialization of the S200 was first done by %s core\r\n\r\n",
                 g_ctx_ptr->init_done_by_core == 0u ? "Main" :
                 g_ctx_ptr->init_done_by_core == 5u ? "NBU" :
                                                      "?unknown?");
    (void)mutex_unlock();

    /* Task loop; start at index 1, as 0 is reserved for the above init */
    for (size_t i = 1u; i < NUMBER_OF_TASKS; i++)
    {
        if (i % 2u == 0u)
        {
            DO_TASK(sha2_256(), g_ctx_ptr->core0_tasks[i].task_retval, g_ctx_ptr->core0_tasks[i].task_completed);
        }
        else
        {
            DO_TASK(aes_128(), g_ctx_ptr->core0_tasks[i].task_retval, g_ctx_ptr->core0_tasks[i].task_completed);
        }

        /* Simulate some non-S200 work */
        (void)SDK_DelayAtLeastUs(5000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    }

    /* Wait for other core to finish as well */
    bool core1_finished = false;
    do
    {
        (void)mutex_lock();
        core1_finished = g_ctx_ptr->core1_tasks[NUMBER_OF_TASKS - 1].task_completed;
        (void)mutex_unlock();

        /* Give NBU some space to not starve out the mutex */
        (void)SDK_DelayAtLeastUs(5000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    } while (false == core1_finished);
    (void)PRINTF("ALL TASKS COMPLETED, CHECKING SUCCESS RATES : \r\n");

    size_t core0_successes = 0u;
    size_t core1_successes = 0u;
    (void)mutex_lock();
    for (size_t i = 0u; i < NUMBER_OF_TASKS; i++)
    {
        if (kStatus_Success == g_ctx_ptr->core0_tasks[i].task_retval)
        {
            core0_successes++;
        }
        if (kStatus_Success == g_ctx_ptr->core1_tasks[i].task_retval)
        {
            core1_successes++;
        }
    }
    (void)mutex_unlock();

    (void)PRINTF("  Main core successfully completed %u tasks out of %u\r\n", core0_successes, NUMBER_OF_TASKS);
    (void)PRINTF("  NBU  core successfully completed %u tasks out of %u\r\n", core1_successes, NUMBER_OF_TASKS);

    (void)PRINTF("\r\n");

    while (1)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }
}
