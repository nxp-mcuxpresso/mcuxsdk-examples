/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "fsl_debug_console.h"

#include "board.h"
#include "app.h"
#include "app_srtm.h"
#include "fsl_wdog32.h"

/*******************************************************************************
 * Struct Definitions
 ******************************************************************************/
/*
 * We use 32KHz Clk divided by 256 as WDOG Clock Source
 */
#define WDOG_TIMEOUT_1S (32768 / 256)
#define WDOG_TIMEOUT    (5 * WDOG_TIMEOUT_1S)
/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Function Code
 ******************************************************************************/
static void WDOG32_Configure(uint16_t timeout)
{
    wdog32_config_t config;

    WDOG32_GetDefaultConfig(&config);
    config.testMode = kWDOG32_UserModeEnabled;

    config.clockSource  = kWDOG32_ClockSource1; // 2, internal clock 8MHz, 1, LPO OSC 32KHz
    config.prescaler    = kWDOG32_ClockPrescalerDivide256;
    config.windowValue  = 0U;
    config.timeoutValue = timeout;

    config.enableWindowMode = false;
    config.enableWdog32     = true;

    WDOG32_Init(CM4__WDOG, &config);
}

static void srtm_codec_demo(void *pvParameters)
{
    char c;
    PRINTF("\r\n####################  CODEC SRTM DEMO ####################\n\r\n");
    PRINTF("    Build Time: %s--%s \n\r\n", __DATE__, __TIME__);
    PRINTF("##########################################################\r\n");

    APP_SRTM_StartCommunication();

    for (;;)
    {
        PRINTF("Press 'r' to do M4 partition reset\r\n");
        c = GETCHAR();
        PRINTF("%c\r\n", c);
        if (c == 'r')
        {
            WDOG32_Configure(WDOG_TIMEOUT);
            PRINTF("Wait a while to reboot\r\n");
            break;
        }
    }

    while (true)
        ;
}

void vApplicationMallocFailedHook(void)
{
    PRINTF("Malloc Failed!!!\r\n");
}

static void APP_PeerCoreResetHandler(void *param)
{
    /*
     * In this App simply call Handle function from APP SRTM
     */
    APP_SRTM_HandlePeerReboot();
}

/*! @brief Main function */
int main(void)
{
    BOARD_InitHardware();

    /*
     * Enable A35 Reboot IRQ
     */
    BOARD_RegisterEventHandler(kSCEvent_PeerCoreReboot, APP_PeerCoreResetHandler, NULL);

    BOARD_EnableSCEvent(SC_EVENT_MASK(kSCEvent_PeerCoreReboot), true);
    BOARD_Enable_SCIRQ(true);
    /*
     * Output Demo Information
     */
    APP_SRTM_Init();

    if (xTaskCreate(srtm_codec_demo, "SRTMDemo", 512U, NULL, tskIDLE_PRIORITY + 1U, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
    }

    /* Start FreeRTOS scheduler. */
    vTaskStartScheduler();

    /* Application should never reach this point. */
    for (;;)
    {
    }
}
