/*
 * Copyright 2021-2022 NXP
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
#include "low_power_wakeword.h"
#include "lpm.h"
#include "fsl_debug_console.h"
#include "voice_engine_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void MainTask(void *pvParameters)
{
    PRINTF("Wait the Linux kernel boot up to create the link between M core and A core.\r\n");
    PRINTF("\r\n");

    APP_SRTM_Init();
#if defined(MIMX9352_cm33_SERIES) || defined(MIMX9322_cm33_SERIES) || defined(MIMX9596_cm7_SERIES)
    APP_SRTM_StartCommunication();
#endif
    PRINTF("The rpmsg channel between M core and A core created!\r\n");
    PRINTF("\r\n");

    voice_engine_create();

    LPM_Init();

    while (true)
    {
        /* Use App task logic to replace vTaskDelay */
        PRINTF_VERBOSE("\r\nMain task is working now.\r\n");
        vTaskDelay(portMAX_DELAY);
    }
}
/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

#if APP_SRTM_CODEC_AK4497_USED
    APP_SRTM_I2C_ReleaseBus();
    BOARD_I2C_ConfigurePins();
#endif
#if APP_SRTM_PDM_USED
    BOARD_PDM_ConfigurePins();
#endif
    PRINTF("\r\n#################### LOW POWER VOICE DEMO START ####################\n\r\n");
    PRINTF("Build Time: %s--%s \r\n", __DATE__, __TIME__);

    if (xTaskCreate(MainTask, "Main Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1U, NULL) != pdPASS)
    {
        PRINTF_VERBOSE("Task creation failed!.\r\n");
        while (1)
            ;
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
