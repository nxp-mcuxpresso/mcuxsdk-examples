/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "board.h"
#include "app.h"
#if defined(FSL_FEATURE_SOC_PORT_COUNT) && (FSL_FEATURE_SOC_PORT_COUNT > 0)
#include "fsl_port.h"
#endif
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Task priorities. */
/* clang-format off */
#define gpio_task_PRIORITY   (configMAX_PRIORITIES - 1 )
#define print_task_PRIORITY  (configMAX_PRIORITIES - 2)
#define TIME_DELAY_SLEEP      1000

SemaphoreHandle_t xMutex;

/* clang-format on */
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static void gpio_task(void *pvParameters);
static void print_task(void *pvParameters);
/* The PIN status */
volatile bool g_pinSet = false;
static uint16_t count  = 0u;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    xMutex = xSemaphoreCreateMutex();
    /* Define the init structure for the output LED pin*/
    gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0};

    BOARD_InitHardware();

    /* Print a note to terminal. */
    PRINTF("Freertos Gpio example\r\n");
    /* Init output LED GPIO. */
    GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, &led_config);

    /*Create tickless task*/
    if (xTaskCreate(gpio_task, "gpio_task", configMINIMAL_STACK_SIZE + 100, NULL, gpio_task_PRIORITY, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }
    if (xTaskCreate(print_task, "print_task", configMINIMAL_STACK_SIZE + 100, NULL, print_task_PRIORITY, NULL) !=
        pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }
    /*Task Scheduler*/
    vTaskStartScheduler();
    for (;;)
        ;
}

/* Gpio Task */
static void gpio_task(void *pvParameters)
{
    for (;;)
    {
        xSemaphoreTake(xMutex, portMAX_DELAY);
        PRINTF("\r\nThe LED is blinking.\r\n");
        count++;
        if (count == 0xFFFF)
        {
            count = 0;
        }
        if (g_pinSet)
        {
            GPIO_PinWrite(BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, 0U);
            g_pinSet = false;
        }
        else
        {
            GPIO_PinWrite(BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, 1U);
            g_pinSet = true;
        }
        xSemaphoreGive(xMutex);
        vTaskDelay(TIME_DELAY_SLEEP);
    }
}

/* Print Task */
static void print_task(void *pvParameters)
{
    for (;;)
    {
        xSemaphoreTake(xMutex, portMAX_DELAY);
        PRINTF("\r\nThe gpio task is running:%dtimes.\r\n", count);
        vTaskDelay(TIME_DELAY_SLEEP);
        xSemaphoreGive(xMutex);
    }
}
