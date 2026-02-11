/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Task priorities. */
#define prp_task_PRIORITY (configMAX_PRIORITIES - 2)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void prp_example_main(void *pvParameters);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
int main(void)
{
    status_t result                  = kStatus_Success;

    /* Init board hardware. */
    BOARD_InitHardware();

    result = APP_MDIO_Init();
    if (result != kStatus_Success)
    {
        PRINTF("\r\nMDIO Init failed!\r\n");
        return result;
    }

    result = APP_PHY_Init();
    if (result != kStatus_Success)
    {
        PRINTF("\r\nPHY Init failed!\r\n");
        return result;
    }

    PRINTF("Run PRP stack!.\r\n");
    if (xTaskCreate(prp_example_main, "prp_task", configMINIMAL_STACK_SIZE + 256, NULL, prp_task_PRIORITY, NULL) !=
        pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();
    for (;;)
        ;
}
