/*
 * Copyright 2020 - 2021, 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "demo_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
EventGroupHandle_t g_errorEvent = NULL;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void led_blinky_init(void);
extern void rtc_init(void);
#ifdef ACCELEROMETER_EXISTS
extern void acc_init(void);
#endif
extern void sdcard_init(void);
extern void audio_init(void);
extern void mouse_init(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("Multi-peripheral Test!\r\n\n");
    g_errorEvent = xEventGroupCreate();

    rtc_init();
    audio_init();
    mouse_init();
    led_blinky_init();
#ifdef ACCELEROMETER_EXISTS
    acc_init();
#endif
#ifndef DEMO_SDCARD_NOSPORT
    sdcard_init();
#endif

    /* Start scheduler. */
    vTaskStartScheduler();
}
