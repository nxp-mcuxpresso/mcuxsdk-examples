/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

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
 * In the example's custom linker file for the NBU core, we defined a workarea
 * memory section that is accessible by the S200. We use this reserved workarea
 * to safely place the S200's input and output buffers when calling SSS APIs
 * from the NBU core.
 */
#define S200_WORKAREA_SIZE (0x1000)
#if defined(__ICCARM__) /* IAR Workbench */
#pragma location = "nbu_s200_workarea_section"
uint8_t nbu_s200_workarea[S200_WORKAREA_SIZE];
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION) /* Keil MDK */
uint8_t nbu_s200_workarea[S200_WORKAREA_SIZE] __attribute__((section("nbu_s200_workarea_section")));
#elif defined(__GNUC__)
uint8_t nbu_s200_workarea[S200_WORKAREA_SIZE] __attribute__((section(".noinit.$m_nbu_s200_workarea")));
#else
#error "Unsupported compiler"
#endif

/**
 * platform_patova
 *
 * Translate cm33_core0 addresses to cm33_core1 addresses
 *
 * This function is directly taken from the RPMSG_Lite middleware, from the
 * middleware/multicore/rpmsg-lite/lib/rpmsg_lite/porting/platform/kw47b42/rpmsg_platform_ext.c
 * implementation file.
 */
static void *platform_patova(uintptr_t addr)
{
    return ((void *)(char *)((addr & 0x0000FFFFu) + 0xB0000000u));
}

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
    uint32_t startupData;
    mcmgr_status_t mcmgr_status;

    /* Init board hardware.*/
    BOARD_InitHardware();

    /* Initialize MCMGR, install generic event handlers */
    (void)MCMGR_Init();

    /* Get the startup data */
    do
    {
        mcmgr_status = MCMGR_GetStartupData(kMCMGR_Core0, &startupData);
    } while (mcmgr_status != kStatus_MCMGR_Success);

    /* The startup data is the address to the shared global context */
    g_ctx_ptr = (ele_multicore_ctx_t *)platform_patova((uintptr_t)(startupData));

    /* Initialize HW. Or just core-side SW if the given core got here late. */
    DO_TASK(init_s200(),
            g_ctx_ptr->core1_tasks[0].task_retval,
            g_ctx_ptr->core1_tasks[0].task_completed);

    /* Task loop; start at index 1, as 0 is reserved for the above init */
    for (size_t i = 1u; i < NUMBER_OF_TASKS; i++)
    {
        /* Introduce some variance */
        if (i % 2u == 0u)
        {
            DO_TASK(sha2_256(), g_ctx_ptr->core1_tasks[i].task_retval, g_ctx_ptr->core1_tasks[i].task_completed);
        }
        else
        {
            DO_TASK(aes_128(), g_ctx_ptr->core1_tasks[i].task_retval, g_ctx_ptr->core1_tasks[i].task_completed);
        }

        /* Simulate some non-S200 work */
        (void)SDK_DelayAtLeastUs(5500U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    }

    while (1)
    {
    }
}
