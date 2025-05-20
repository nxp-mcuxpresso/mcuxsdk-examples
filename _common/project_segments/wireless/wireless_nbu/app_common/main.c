/*!
 * Copyright 2025 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * \file main.c
 * \brief generic main implementation used as a baseline for any
 *        application running on the NBU with ThreadX
 *
 */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "tx_api.h"
#include "board.h"
#include "app.h"

/* -------------------------------------------------------------------------- */
/*                               Private macros                               */
/* -------------------------------------------------------------------------- */
#define IDLE_TASK_PRIORITY (TX_MAX_PRIORITIES - 1)

#ifndef CONFIG_IDLE_TASK_STACK_SIZE
#define CONFIG_IDLE_TASK_STACK_SIZE 600
#endif

#ifndef CONFIG_MAIN_TASK_PRIORITY
#define CONFIG_MAIN_TASK_PRIORITY (TX_MAX_PRIORITIES >> 1)
#endif

#ifndef CONFIG_MAIN_TASK_STACK_SIZE
#define CONFIG_MAIN_TASK_STACK_SIZE 1024
#endif

#ifndef CONFIG_TICK_RATE_HZ
#define CONFIG_TICK_RATE_HZ 1000U
#endif

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */
static void idle_task(ULONG argument);
static void main_task(ULONG argument);

/* -------------------------------------------------------------------------- */
/*                               Private memory                               */
/* -------------------------------------------------------------------------- */
TX_THREAD idle_task_handle;
TX_THREAD main_task_handle;

uint32_t idle_task_stack[(CONFIG_IDLE_TASK_STACK_SIZE + sizeof(uint32_t) - 1) / sizeof(uint32_t)];
uint32_t main_task_stack[(CONFIG_MAIN_TASK_STACK_SIZE + sizeof(uint32_t) - 1) / sizeof(uint32_t)];

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */
static void idle_task(ULONG argument)
{
    (void)argument;

    while(true)
    {
        APP_Idle();
    }
}

static void main_task(ULONG argument)
{
    (void)argument;

    /* PostKernel init stage for the application - all kernel primitives are available */
    APP_PostKernelInit();

    /* main applicative loop
     * If this function returns, the main task will exit and stop running, so if the application needs
     * to keep it alive, it can implement a while loop as any other thread */
    APP_Main();
}

static void InitSysTicks(void)
{
    SysTick->LOAD |= (BOARD_GetSystemCoreClockFreq() / CONFIG_TICK_RATE_HZ) - 1U;
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */
void tx_application_define(void *first_unused_memory)
{
    /* Configure and enable SysTicks here because this is not done by _tx_initialize_low_level */
    InitSysTicks();

    /* Create the idle task */
    if(TX_SUCCESS != tx_thread_create(&idle_task_handle, "idle_task", idle_task, 0U, idle_task_stack,
                                        CONFIG_IDLE_TASK_STACK_SIZE, IDLE_TASK_PRIORITY, IDLE_TASK_PRIORITY,
                                        TX_NO_TIME_SLICE, TX_AUTO_START))
    {
        assert(0);
    }

    /* Create the main task - the PostKernel init stage will be done from this task */
    if(TX_SUCCESS != tx_thread_create(&main_task_handle, "main_task", main_task, 0U, main_task_stack,
                                        CONFIG_MAIN_TASK_STACK_SIZE, CONFIG_MAIN_TASK_PRIORITY,
                                        CONFIG_MAIN_TASK_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START))
    {
        assert(0);
    }

}

int main(void)
{
    BOARD_InitHardware();

    /* PreKernel init stage for the application
     * ThreadX primitives are no available yet */
    APP_PreKernelInit();

    /* Start ThreadX scheduler - tx_application_define will be called before the scheduler is started */
    tx_kernel_enter();

    /*won't run here*/
    assert(false);
    return 0;
}
