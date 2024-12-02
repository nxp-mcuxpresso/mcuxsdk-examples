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
#include "board.h"
#include "app.h"

#include "fsl_lpsci_freertos.h"
#include "fsl_lpsci.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Task priorities. */
#define lpsci_task_PRIORITY (configMAX_PRIORITIES - 1)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void lpsci_task(void *pvParameters);

/*******************************************************************************
 * Code
 ******************************************************************************/
char *to_send               = "FreeRTOS LPSCI driver example!\r\n";
char *send_ring_overrun     = "\r\nRing buffer overrun!\r\n";
char *send_hardware_overrun = "\r\nHardware buffer overrun!\r\n";
uint8_t background_buffer[32];
uint8_t recv_buffer[4];

lpsci_rtos_handle_t handle;
struct _lpsci_handle t_handle;

lpsci_rtos_config_t lpsci_config = {
    .baudrate    = 115200,
    .parity      = kLPSCI_ParityDisabled,
    .stopbits    = kLPSCI_OneStopBit,
    .buffer      = background_buffer,
    .buffer_size = sizeof(background_buffer),
};

/*!
 * @brief Application entry point.
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();
    if (xTaskCreate(lpsci_task, "Uart_task", configMINIMAL_STACK_SIZE, NULL, lpsci_task_PRIORITY, NULL) != pdPASS)
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
 * @brief Task responsible for loopback.
 */
static void lpsci_task(void *pvParameters)
{
    int error;
    size_t n            = 0;
    lpsci_config.srcclk = DEMO_LPSCI_CLK_FREQ;
    lpsci_config.base   = DEMO_LPSCI;

    if (kStatus_Success != LPSCI_RTOS_Init(&handle, &t_handle, &lpsci_config))
    {
        vTaskSuspend(NULL);
    }

    /* Send introduction message. */
    if (kStatus_Success != LPSCI_RTOS_Send(&handle, (uint8_t *)to_send, strlen(to_send)))
    {
        vTaskSuspend(NULL);
    }

    /* Receive user input and send it back to terminal. */
    do
    {
        error = LPSCI_RTOS_Receive(&handle, recv_buffer, sizeof(recv_buffer), &n);
        if (error == kStatus_LPSCI_RxHardwareOverrun)
        {
            /* Notify about hardware buffer overrun */
            if (kStatus_Success !=
                LPSCI_RTOS_Send(&handle, (uint8_t *)send_hardware_overrun, strlen(send_hardware_overrun)))
            {
                vTaskSuspend(NULL);
            }
        }
        if (error == kStatus_LPSCI_RxRingBufferOverrun)
        {
            /* Notify about ring buffer overrun */
            if (kStatus_Success != LPSCI_RTOS_Send(&handle, (uint8_t *)send_ring_overrun, strlen(send_ring_overrun)))
            {
                vTaskSuspend(NULL);
            }
        }

        if (n > 0)
        {
            /* send back the received data */
            LPSCI_RTOS_Send(&handle, recv_buffer, n);
        }
    } while (kStatus_Success == error);

    LPSCI_RTOS_Deinit(&handle);
    vTaskSuspend(NULL);
}
