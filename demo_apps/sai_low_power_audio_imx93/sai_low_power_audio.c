/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "task.h"

#include "app.h"
#include "pin_mux.h"
#include "board.h"
#include "app_srtm.h"
#include "fsl_debug_console.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern volatile app_srtm_state_t srtmState;
extern bool APP_SRTM_IsAudioServiceIdle(void);
/*******************************************************************************
 * Code
 ******************************************************************************/
void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
    uint32_t irqMask;

    irqMask = DisableGlobalIRQ();

    /*
     * Only when no context switch is pending and no task is waiting for the scheduler
     * to be unsuspended then enter low power entry.
     */
    if (eTaskConfirmSleepModeStatus() != eAbortSleep)
    {
        if (APP_SRTM_IsAudioServiceIdle())
        {
            *((uint32_t *)M33_ACTIVE_FLAG) = M33_INACTIVE;
            /* M33 enter Stop mode */
            GPC_CTRL_CM33->CM_MODE_CTRL |= GPC_CPU_CTRL_CM_MODE_CTRL_CPU_MODE_TARGET(2);
            __DSB();
            __WFI();
            __ISB();
            *((uint32_t *)M33_ACTIVE_FLAG) = M33_ACTIVE;
            /* Reset the CM MODE CTRL to Run mode */
            GPC_CTRL_CM33->CM_MODE_CTRL |= GPC_CPU_CTRL_CM_MODE_CTRL_CPU_MODE_TARGET(0);
        }
    }

    EnableGlobalIRQ(irqMask);
}

void MainTask(void *pvParameters)
{
    /*
     * Wait For A core Side Become Ready
     */
    PRINTF("********************************\r\n");
    PRINTF(" Wait the Linux kernel boot up to create the link between M core and A core.\r\n");
    PRINTF("\r\n");
    PRINTF("********************************\r\n");
    while (srtmState != APP_SRTM_StateLinkedUp)
        ;
    PRINTF("The rpmsg channel between M core and A core created!\r\n");
    PRINTF("********************************\r\n");
    PRINTF("\r\n");

    while (true)
    {
        /* Use App task logic to replace vTaskDelay */
        PRINTF("\r\nTask %s is working now.\r\n", (char *)pvParameters);
        vTaskDelay(portMAX_DELAY);
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    char *taskID = "A";
    BOARD_InitHardware();

    PRINTF("\r\n####################  LOW POWER AUDIO TASK ####################\n\r\n");
    PRINTF("    Build Time: %s--%s \r\n", __DATE__, __TIME__);

    if (xTaskCreate(MainTask, "Main Task", 256U, (void *)taskID, tskIDLE_PRIORITY + 1U, NULL) != pdPASS)
    {
        PRINTF("\r\nFailed to create MainTask task\r\n");
        while (1)
        {
        }
    }

    /* Start FreeRTOS scheduler. */
    vTaskStartScheduler();

    /* Application should never reach this point. */
    for (;;)
    {
    }
}
void vApplicationMallocFailedHook(void)
{
    PRINTF("Malloc Failed!!!\r\n");
}
