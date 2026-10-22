/*
 * Copyright (c) 2025 Arm Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * CMSIS-Driver Validation entry point.
 * Hardware init (clocks, pins, debug console) is done in hardware_init.c
 * (board-specific) which provides BOARD_InitHardware().
 * main() is provided by the SDK board template and calls app_main() after
 * calling BOARD_InitHardware().
 */

#include "cmsis_usbd_validation.h"
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_dv.h"

/* Main thread stack and TCB - both static so xTaskCreateStatic is used.
 * osThreadNew requires cb_mem+cb_size when stack_mem+stack_size are provided,
 * otherwise the CMSIS-OS2 FreeRTOS wrapper returns NULL. */
#define APP_MAIN_STK_SZ (4096U)
static uint64_t             app_main_stk[APP_MAIN_STK_SZ / 8];
static StaticTask_t         app_main_tcb;
static const osThreadAttr_t app_main_attr = {
    .stack_mem  = &app_main_stk[0],
    .stack_size = sizeof(app_main_stk),
    .cb_mem     = &app_main_tcb,
    .cb_size    = sizeof(app_main_tcb)
};

/*---------------------------------------------------------------------------
 * Application main thread - runs CMSIS-Driver Validation suite
 *---------------------------------------------------------------------------*/
static void app_main_thread (void *argument) {
    (void)argument;
    cmsis_dv(NULL);
    osThreadExit();
}

/*---------------------------------------------------------------------------
 * Application main function - called after hardware init
 *---------------------------------------------------------------------------*/
int32_t app_main (void) {
    osKernelInitialize();
    (void)osThreadNew(app_main_thread, NULL, &app_main_attr);
    osKernelStart();
    return 0;
}
