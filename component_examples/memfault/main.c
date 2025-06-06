/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "memfault/components.h"
#include "memfault/ports/freertos.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct _app_cmd
{
    char cmdChar;
    const char *cmdDesc;
    void (*cmdFunc)(void);
} app_cmd_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void trigger_fault(void);
static void dump_data(void);
static void self_test(void);
static void main_task(void *pvParameters);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static const app_cmd_t appCmds[] = {
    {'A', "Trigger fault", trigger_fault},
    {'B', "Dump Memfault data", dump_data},
    {'C', "Self test", self_test},
};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    memfault_platform_boot();

    if (xTaskCreate(main_task, "main_task", configMINIMAL_STACK_SIZE + 200, NULL, configMAX_PRIORITIES - 1, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }
    vTaskStartScheduler();

    for (;;)
        ;
}

static void main_task(void *pvParameters)
{
    char ch;
    int i;

    while(1)
    {
        PRINTF("\r\n===========================");
        PRINTF("\r\nPlease enter the option:");
        for (i = 0; i < ARRAY_SIZE(appCmds); i++)
        {
            PRINTF("\r\n%c: %s", appCmds[i].cmdChar, appCmds[i].cmdDesc);
        }
        PRINTF("\r\n===========================\r\n");

        ch = GETCHAR();
        PRINTF("%c\r\n", ch);

        /* Convert to upper case */
        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch = ch - 'a' + 'A';
        }

        /*  Check if the option is valid */
        for (i = 0; i < ARRAY_SIZE(appCmds); i++)
        {
            if (appCmds[i].cmdChar == ch)
            {
                break;
            }
        }

        if (i < ARRAY_SIZE(appCmds))
        {
            /* Run the comand */
            PRINTF(appCmds[i].cmdDesc);
            PRINTF("\r\n");
            appCmds[i].cmdFunc();
        }
        else
        {
            PRINTF("\r\nInvalid option, please try again!\r\n");
        }
    }
}

static void trigger_fault(void)
{
    /* Define a function pointer, point to a invalid address, call it */
    void (*func_ptr)(void) = (void (*)(void))0xFFFFFFF0;
    func_ptr();
}

static void dump_data(void)
{
    memfault_data_export_dump_chunks();
    PRINTF("Dump data done\r\n");
}

static void self_test(void)
{
    int result = memfault_self_test_run(
                           kMemfaultSelfTestFlag_CoredumpRegions |
                           kMemfaultSelfTestFlag_CoredumpStorage |
                           kMemfaultSelfTestFlag_CoredumpStorageCapacity |
                           kMemfaultSelfTestFlag_DataExport);

    if (result == 0)
    {
        PRINTF("Test successed\r\n");
    }
    else
    {
        PRINTF("Test failed\r\n");
    }
}
