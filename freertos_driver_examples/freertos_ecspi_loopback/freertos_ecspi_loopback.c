/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
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
#include "fsl_ecspi.h"
#include "fsl_ecspi_freertos.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Task priorities. */
#define ecspi_task_PRIORITY (configMAX_PRIORITIES - 2)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void ecspi_task(void *pvParameters);
uint32_t masterRxData[ECSPI_TRANSFER_SIZE] = {0};
uint32_t masterTxData[ECSPI_TRANSFER_SIZE] = {0};
static ecspi_master_config_t masterConfig;
static ecspi_transfer_t masterXfer;
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

    PRINTF("\r\n***FreeRTOS ECSPI Loopback Demo***\r\n");
    PRINTF("\r\nThis demo is a loopback transfer test for ECSPI.\r\n");
    PRINTF("The ECSPI will connect the transmitter and receiver sections internally.\r\n");
    PRINTF("So, there is no need to connect the MOSI and MISO pins.\r\n");

    if (xTaskCreate(ecspi_task, "Ecspi_task", configMINIMAL_STACK_SIZE + 100, NULL, ecspi_task_PRIORITY, NULL) !=
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

/*!
 * @brief Task responsible for ecspi.
 */
static void ecspi_task(void *pvParameters)
{
    uint8_t i;
    status_t status;
    ecspi_rtos_handle_t master_rtos_handle;

    ECSPI_MasterGetDefaultConfig(&masterConfig);

    masterConfig.baudRate_Bps   = ECSPI_TRANSFER_BAUDRATE;
    masterConfig.enableLoopback = true;

    status = ECSPI_RTOS_Init(&master_rtos_handle, ECSPI_MASTER_BASEADDR, &masterConfig, ECSPI_MASTER_CLK_FREQ);

    if (status != kStatus_Success)
    {
        PRINTF("ECSPI meets error during initialization. \r\n");
        vTaskSuspend(NULL);
    }

    for (i = 0; i < ECSPI_TRANSFER_SIZE; i++)
    {
        masterTxData[i] = i;
    }
    masterXfer.txData   = masterTxData;
    masterXfer.rxData   = masterRxData;
    masterXfer.dataSize = ECSPI_TRANSFER_SIZE;
    masterXfer.channel  = ECSPI_MASTER_TRANSFER_CHANNEL;
    /*Start master transfer*/

    status = ECSPI_RTOS_Transfer(&master_rtos_handle, &masterXfer);
    if (status != kStatus_Success)
    {
        PRINTF("ECSPI transfer completed with error. \r\n\r\n");
        vTaskSuspend(NULL);
    }

    /* Compare Tx and Rx data. */
    for (i = 0; i < ECSPI_TRANSFER_SIZE; i++)
    {
        if (masterTxData[i] != masterRxData[i])
        {
            break;
        }
    }

    if (ECSPI_TRANSFER_SIZE == i)
    {
        PRINTF("\r\nFreeRTOS ECSPI loopback test pass!");
    }
    else
    {
        PRINTF("\r\nFreeRTOS ECSPI loopback test fail!");
    }

    /* Deinit the ECSPI. */
    ECSPI_RTOS_Deinit(&master_rtos_handle);
    vTaskSuspend(NULL);
}
