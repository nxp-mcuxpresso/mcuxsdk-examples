/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////
#include "fsl_os_abstraction.h"
#include "fsl_debug_console.h"
#include "board.h"

#ifdef RW612_SERIES
#if defined(MBEDTLS_NXP_SSSAPI)
#include "sssapi_mbedtls.h"
#elif defined(MBEDTLS_MCUX_CSS_API)
#include "platform_hw_ip.h"
#include "css_mbedtls.h"
#elif defined(MBEDTLS_MCUX_CSS_PKC_API)
#include "platform_hw_ip.h"
#include "css_pkc_mbedtls.h"
#elif defined(MBEDTLS_MCUX_ELS_PKC_API)
#include "platform_hw_ip.h"
#include "els_pkc_mbedtls.h"
#elif defined(MBEDTLS_MCUX_ELS_API)
#include "platform_hw_ip.h"
#include "els_mbedtls.h"
#elif defined(MBEDTLS_MCUX_ELE_S400_API)
#include "ele_mbedtls.h"
#else
#ifdef CONFIG_KSDK_MBEDTLS
#include "ksdk_mbedtls.h"
#endif
#endif
#endif /* RW612_SERIES */

#include "coex_shell.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define COEX_INIT_TASK_STACK_SIZE 4048
#define COEX_INIT_PRIORITY        PRIORITY_RTOS_TO_OSA(1)
static void coex_init_task(osa_task_param_t arg);

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void BOARD_InitHardware(void);
extern void coex_controller_init(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static OSA_TASK_DEFINE(coex_init_task, COEX_INIT_PRIORITY, 1, COEX_INIT_TASK_STACK_SIZE, 0);
OSA_TASK_HANDLE_DEFINE(coex_init_task_handle);

/*******************************************************************************
 * Code
 ******************************************************************************/

static void printSeparator(void)
{
    PRINTF("========================================\r\n");
}

void coex_init_task(osa_task_param_t arg)
{
    printSeparator();
#if defined(RW612_SERIES)
    PRINTF("     Initialize RW612 Module\r\n");
#endif
    printSeparator();

    coex_controller_init();
    coex_cli_init();

    /* Initialization job is complete, destroy the task */
    OSA_TaskDestroy((osa_task_handle_t)coex_init_task_handle);
}

#if defined (APP_CONFIG_ENABLE_STACK_OVERFLOW_FREERTOS_HOOK) \
        && (APP_CONFIG_ENABLE_STACK_OVERFLOW_FREERTOS_HOOK == 1U)
void stackOverflowHookHandler(void* task_name)
{
    printf("stack-overflow exception from task: %s\r\n",(char*)task_name);
}
#endif /* #if defined (APP_CONFIG_ENABLE_STACK_OVERFLOW_FREERTOS_HOOK) && (APP_CONFIG_ENABLE_STACK_OVERFLOW_FREERTOS_HOOK == 1U) */

int main(void)
{
    OSA_Init();

    BOARD_InitHardware();

    PRINTF("        Coex edgefast zigbee APP\r\n");
    printSeparator();

    (void)OSA_TaskCreate((osa_task_handle_t)coex_init_task_handle, OSA_TASK(coex_init_task), NULL);

    /*start scheduler*/
    OSA_Start();

    /*won't run here*/
    assert(0);
    return 0;
}


#ifndef __GNUC__
void __assert_func(const char *file, int line, const char *func, const char *failedExpr)
{
    PRINTF("ASSERT ERROR \" %s \": file \"%s\" Line \"%d\" function name \"%s\" \n", failedExpr, file, line, func);
    for (;;)
    {
        __BKPT(0);
    }
}
#endif
