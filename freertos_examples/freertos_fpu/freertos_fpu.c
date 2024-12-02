/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*System includes.*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/* Freescale includes. */
#include "app.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_gpt.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MAX_COUNT_LENGTH 10
#define EPSILON          0.0000001
/*******************************************************************************
 * Globals
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Application API */
static void sin_task(void *pvParameters);
static void cos_task(void *pvParameters);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static double s_sinCount[10][MAX_COUNT_LENGTH]; /* Sin data */
static double s_cosCount[10][MAX_COUNT_LENGTH]; /* Cos data */
static double s_logCount[MAX_COUNT_LENGTH];     /* Log data */

/*******************************************************************************
 * Code
 ******************************************************************************/

static void DEMO_InitGpt(void)
{
    gpt_config_t gptConfig;

    GPT_GetDefaultConfig(&gptConfig);
    GPT_Init(DEMO_GPT, &gptConfig);
    /* Set GPT modules to 0.001 second duration */
    GPT_SetOutputCompareValue(DEMO_GPT, kGPT_OutputCompare_Channel1, DEMO_GPT_CLK_FREQ / 1000);

    /* Enable GPT Output Compare1 interrupt */
    GPT_EnableInterrupts(DEMO_GPT, kGPT_OutputCompare1InterruptEnable);
    /* Change priority */
    GIC_SetPriority(DEMO_GPT_IRQ, DEMO_GPT_GIC_PRIO);
    /* Enable at the Interrupt */
    EnableIRQ(DEMO_GPT_IRQ);
    GPT_StartTimer(DEMO_GPT);
}

void DEMO_TIMER_IRQHandler(void)
{
    static uint32_t s_IRQCount = 0;
    double temp;

    /* Clear interrupt flag.*/
    GPT_ClearStatusFlags(DEMO_GPT, kGPT_OutputCompare1Flag);

    temp = log(s_IRQCount);
    if (fabs(temp - s_logCount[s_IRQCount]) > EPSILON)
    {
        PRINTF("\r\nFPU count error of log(%d) in IRQ handler!!\r\n", s_IRQCount);
    }

    s_IRQCount++;
    if (s_IRQCount == MAX_COUNT_LENGTH)
    {
        s_IRQCount = 0;
    }
    SDK_ISR_EXIT_BARRIER;
}

void DEMO_CalcRefData(void)
{
    uint32_t i, j;
    for (i = 0; i < 10; i++)
    {
        for (j = 0; j < MAX_COUNT_LENGTH; j++)
        {
            s_sinCount[i][j] = sin(i * 10 + j);
            s_cosCount[i][j] = cos(i * 10 + j);
        }
    }

    for (i = 0; i < MAX_COUNT_LENGTH; i++)
    {
        s_logCount[i] = log(i);
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    DEMO_CalcRefData();
    DEMO_InitGpt();

    if (xTaskCreate(sin_task, "sin_task", configMINIMAL_STACK_SIZE + 166, NULL, tskIDLE_PRIORITY + 2, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }
    if (xTaskCreate(cos_task, "cos_task", configMINIMAL_STACK_SIZE + 166, NULL, tskIDLE_PRIORITY + 2, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
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
 * @brief sin_task function
 */
static void sin_task(void *pvParameters)
{
    double temp;
    uint32_t i   = 0;
    uint32_t j   = 0;
    uint32_t err = 0;

    for (;;)
    {
        for (i = 0; i < 10; i++)
        {
            for (j = 0; j < MAX_COUNT_LENGTH; j++)
            {
                temp = sin(i * 10 + j);
                if (fabs(temp - s_sinCount[i][j]) > EPSILON)
                {
                    taskENTER_CRITICAL();
                    PRINTF("FPU count error of sin(%d)!!\r\n", (i * 10 + j));
                    taskEXIT_CRITICAL();
                    err++;
                }
            }
        }

        if (err == 0)
        {
            taskENTER_CRITICAL();
            PRINTF("sin_task FPU test succeed!\r\n");
            taskEXIT_CRITICAL();
        }
    }
}

/*!
 * @brief cos_task function
 */
static void cos_task(void *pvParameters)
{
    double temp;
    uint32_t i   = 0;
    uint32_t j   = 0;
    uint32_t err = 0;

    for (;;)
    {
        for (i = 0; i < 10; i++)
        {
            for (j = 0; j < MAX_COUNT_LENGTH; j++)
            {
                temp = cos(i * 10 + j);
                if (fabs(temp - s_cosCount[i][j]) > EPSILON)
                {
                    taskENTER_CRITICAL();
                    PRINTF("FPU count error of cos(%d)!!\r\n", (i * 10 + j));
                    taskEXIT_CRITICAL();
                    err++;
                }
            }
        }

        if (err == 0)
        {
            taskENTER_CRITICAL();
            PRINTF("cos_task FPU test succeed!\r\n");
            taskEXIT_CRITICAL();
        }
    }
}
