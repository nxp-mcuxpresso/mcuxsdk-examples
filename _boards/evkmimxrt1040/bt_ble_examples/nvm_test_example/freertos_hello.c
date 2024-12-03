/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include <stdio.h>
/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "NVM_Interface.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CONFIG_NVM_SETTINGS_MAX_PAIRED   (2U)
#define CONFIG_NVM_SETTINGS_MAX_NAME_LEN (32U)
#if defined(__IAR_SYSTEMS_ICC__)
_Pragma("location=\"NVM_TABLE\"") __root NVM_DataEntry_t NvDataTable[CONFIG_NVM_SETTINGS_MAX_PAIRED + 1];
#elif defined(__GNUC__)
NVM_DataEntry_t NvDataTable[CONFIG_NVM_SETTINGS_MAX_PAIRED + 1] __attribute__((section(".NVM_TABLE"), used));
#else
#error "Must define NvDataTable"
#endif

static char NvmSaveBuf[CONFIG_NVM_SETTINGS_MAX_PAIRED][32U];
static uint8_t count;
/* Task priorities. */
#define hello_task_PRIORITY (configMAX_PRIORITIES - 1)
#define idle_task_PRIORITY  (0U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void hello_task(void *pvParameters);

/* Initialize the nvm backend. */
static int nvm_backend_init(void)
{
#if gAppUseNvm_d
    uint32_t index;
    for (index = 0; index < CONFIG_NVM_SETTINGS_MAX_PAIRED; index++)
    {
        NvDataTable[index].pData         = NvmSaveBuf[index];
        NvDataTable[index].ElementSize   = 1;
        NvDataTable[index].ElementsCount = CONFIG_NVM_SETTINGS_MAX_NAME_LEN;
        NvDataTable[index].DataEntryID   = 0xf101 + index;
        NvDataTable[index].DataEntryType = gNVM_MirroredInRam_c;
    }
    NvDataTable[index].pData         = &count;
    NvDataTable[index].ElementSize   = 1;
    NvDataTable[index].ElementsCount = 1;
    NvDataTable[index].DataEntryID   = 0xff31;
    NvDataTable[index].DataEntryType = gNVM_MirroredInRam_c;
    /* Initialize NV module */
    (void)NvModuleInit();
#endif

    return 0;
}
static void flexspi_clock_init(void)
{
    CLOCK_SetMux(kCLOCK_FlexspiMux, 0x2); /* Choose PLL2 PFD2 clock as flexspi source clock. 396M */
    CLOCK_SetDiv(kCLOCK_FlexspiDiv, 2);   /* flexspi clock 133M. */
}

static void NVMIdleTask(void *pvParameters)
{
    while (1)
    {
#if (gAppUseNvm_d)
        NvIdle();
#endif
    }
}
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    flexspi_clock_init();
    if (xTaskCreate(hello_task, "Hello_task", configMINIMAL_STACK_SIZE + 800, NULL, hello_task_PRIORITY, NULL) !=
        pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }
    if (xTaskCreate(NVMIdleTask, "idle_task", configMINIMAL_STACK_SIZE + 800, NULL, idle_task_PRIORITY, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }
    vTaskStartScheduler();
    for (;;)
        ;
}

uint32_t test = 1;
/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
static void hello_task(void *pvParameters)
{
    for (;;)
    {
        PRINTF("Hello world.\r\n");
        //      while(test);
        nvm_backend_init();
        NvRestoreDataSet(&count, false);
        if (count != 0)
        {
            NvRestoreDataSet(NvmSaveBuf[0], false);
            PRINTF(NvmSaveBuf[0]);
            NvRestoreDataSet(NvmSaveBuf[1], false);
            PRINTF(NvmSaveBuf[1]);
        }

        count++;
        sprintf(NvmSaveBuf[0], "Test NVM 1####count%d\r\n", count);
        NvSaveOnIdle(NvmSaveBuf[0], false);
        sprintf(NvmSaveBuf[1], "Test NVM 2####count%d\r\n", count);
        NvSaveOnIdle(NvmSaveBuf[1], false);
        NvSaveOnIdle(&count, false);
        PRINTF("Save NVM data Count%d.\r\n", count);

        vTaskSuspend(NULL);
    }
}
