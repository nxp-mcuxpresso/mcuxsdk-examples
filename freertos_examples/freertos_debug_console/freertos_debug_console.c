/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*System includes.*/
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "semphr.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
TaskHandle_t printTask1Handle = NULL;
TaskHandle_t printTask2Handle = NULL;
TaskHandle_t printTask3Handle = NULL;
TaskHandle_t scanfTaskHandle  = NULL;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Application API */
static void printTask1(void *pvParameters);
static void printTask2(void *pvParameters);
static void printTask3(void *pvParameters);
static void scanfTask(void *pvParameters);
/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    if (xTaskCreate(printTask1, "printTask1", configMINIMAL_STACK_SIZE + DEMO_TASK1_STACK_SIZE_OFFSET, NULL,
                    DEMO_PRINT_TASK1_PRIORITY, &printTask1Handle) != pdPASS)
    {
        PRINTF("Failed to create printTask1");
        while (1)
            ;
    }

    if (xTaskCreate(printTask2, "printTask2", configMINIMAL_STACK_SIZE + DEMO_TASK2_STACK_SIZE_OFFSET, NULL,
                    DEMO_PRINT_TASK2_PRIORITY, &printTask2Handle) != pdPASS)
    {
        PRINTF("Failed to create printTask2");
        while (1)
            ;
    }

    if (xTaskCreate(printTask3, "printTask3", configMINIMAL_STACK_SIZE + DEMO_TASK3_STACK_SIZE_OFFSET, NULL,
                    DEMO_PRINT_TASK3_PRIORITY, &printTask3Handle) != pdPASS)
    {
        PRINTF("Failed to create printTask3");
        while (1)
            ;
    }

    if (xTaskCreate(scanfTask, "scanfTask", configMINIMAL_STACK_SIZE + DEMO_TASK3_STACK_SIZE_OFFSET, NULL,
                    DEMO_PRINT_TASK3_PRIORITY, &scanfTaskHandle) != pdPASS)
    {
        PRINTF("Failed to create scanfTask");
        while (1)
            ;
    }

    vTaskStartScheduler();
    for (;;)
        ;
}

/*******************************************************************************
 * Application functions
 ******************************************************************************/
/*!
 * @brief print task1 function
 */
static void printTask1(void *pvParameters)
{
    while (1)
    {
        PRINTF("\r\nDebug console Demo-print task1.\r\n");
        PRINTF("\r\nTask1 priority %d, stack size is %d, current tick count %d\r\n", DEMO_PRINT_TASK1_PRIORITY,
               DEMO_TASK1_STACK_SIZE_OFFSET, xTaskGetTickCount());
        /* delay */
        vTaskDelay(100U);
    }
}

/*!
 * @brief print task2 function
 */
static void printTask2(void *pvParameters)
{
    while (1)
    {
        PRINTF("\r\nDebug console Demo-print task2.\r\n");
        PUTCHAR('c');
        PRINTF("\r\nTask2 priority %d, stack size is %d, current tick count %d\r\n", DEMO_PRINT_TASK2_PRIORITY,
               DEMO_TASK2_STACK_SIZE_OFFSET, xTaskGetTickCount());
        /* delay */
        vTaskDelay(500U);
    }
}

/*!
 * @brief print task3 function
 */
static void printTask3(void *pvParameters)
{
    while (1)
    {
        PRINTF("\r\nDebug console Demo-print task3.\r\n");
        PRINTF("\r\nTask3 priority %d, stack size is %d, current tick count %d\r\n", DEMO_PRINT_TASK3_PRIORITY,
               DEMO_TASK3_STACK_SIZE_OFFSET, xTaskGetTickCount());
        /* delay */
        vTaskDelay(300U);
    }
}

static void scanfTask(void *pvParameters)
{
    char ch;

    PRINTF("\r\nDebug console Demo-scanf task.\r\n");
    PRINTF("\r\n Please Input a char: \r\n");
    SCANF("%c", &ch);
    PUTCHAR(ch);
    /* suspend */
    vTaskSuspend(scanfTaskHandle);
}
