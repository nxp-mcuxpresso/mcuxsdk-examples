/*
 * Copyright 2024-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html.
 */

/*
 * Coex Wi-Fi + BT A2DP Sink Application (Edgefast Open) - Clean Model B
 *
 * Architecture:
 *   - Wi-Fi and BT/NB firmware use INDEPENDENT download: the coex middleware
 *     (coex_controller_init) downloads the NB(BT) firmware over UART first, then
 *     brings up WLAN over SDIO. The coex middleware is retained ONLY for this
 *     independent-download orchestration + board init + linker + config headers.
 *   - The shell is app-owned (fsl_shell / edgefast_open PORT_SHELL). The single
 *     shell instance is created by app_shell_init() (app_shell.c). Unlike
 *     a2dp_source (whose stock middleware example ships app_shell.c/.h and calls
 *     app_shell_init() from bt_ready()), the stock a2dp_sink example has no shell
 *     hook and we must not modify any middleware file, so main.c (task_main)
 *     calls app_shell_init() directly. app_shell.c registers BOTH the "bt"
 *     (A2DP-sink control) and "wifi" (WLAN CLI dispatch) commands,
 *     giving one prompt (@Coex>). WLAN CLIs (wlan-scan, ping, iperf, ...) are
 *     registered by the coex middleware's wlan_event_callback on
 *     WLAN_REASON_INITIALIZED and are reached via "wifi <wlan-command>".
 *   - main.c only orchestrates startup: coex_controller_init() (independent NB FW
 *     download + WLAN init) then launches the a2dp_sink task.
 */

///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include "fsl_os_abstraction.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "task.h"

#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_adapter_uart.h"

#include "FreeRTOS.h"

#if !defined(RW612_SERIES)
#include "wifi.h"
#include "fsl_sdmmc_host.h"
#include "app.h"
#include "fsl_device_registers.h"
#include "fsl_iomuxc.h"
#include "fsl_gpc.h"
#include "fsl_lpuart_edma.h"
#include "fsl_dmamux.h"
#endif /* RW612_SERIES */

/* NOTE: do NOT include "app_a2dp_sink.h" here. It pulls in the edgefast_open
 * zephyr toolchain headers (zephyr/toolchain/gcc.h) which redefine __maybe_unused
 * and clash with the wpa_supplicant common.h definition included via wifi.h.
 * Only the task entry prototype is needed in main.c. */
extern void app_a2dp_sink_task(void *param);
#include "app_shell.h"

#if defined(MBEDTLS_USER_CONFIG_FILE)
#include MBEDTLS_USER_CONFIG_FILE
#endif
#if defined(MBEDTLS_THREADING_C) && defined(MBEDTLS_THREADING_ALT)
#include "threading_alt.h"
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern void BOARD_InitHardware(void);
extern void coex_controller_init(void);
extern void otSysRunIdleTask(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

const int TASK_MAIN_PRIO       = (CONFIG_NUM_PREEMPT_PRIORITIES - 5);
const int TASK_MAIN_STACK_SIZE = (2 * 1024);
TaskHandle_t task_main_handle;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void printSeparator(void)
{
    PRINTF("========================================\r\n");
}

void task_main(void *param)
{
    printSeparator();
#if defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
    PRINTF("     Initialize Firecrest-2EL (IW612) Direct-M2 Module\r\n");
#elif defined(WIFI_IW416_BOARD_MURATA_1XK_M2)
    PRINTF("     Initialize RB3P 1XK Direct-M2 Module\r\n");
#elif defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2)
    PRINTF("     Initialize CA2 1ZM Direct-M2 Module\r\n");
#endif
    printSeparator();

    /* Independent NB(BT) FW download over UART, then WLAN init over SDIO.
     * WLAN CLIs are registered by the coex middleware wlan_event_callback. */
    coex_controller_init();
    PRINTF("host init done\r\n");

    /* Wait for the WLAN interface to come up before starting BT/A2DP */
    vTaskDelay(3000);

    if (xTaskCreate(app_a2dp_sink_task, "app_a2dp_sink_task", configMINIMAL_STACK_SIZE * 8, NULL,
                    tskIDLE_PRIORITY + 1, NULL) != pdPASS)
    {
        PRINTF("a2dp task creation failed!\r\n");
        while (1)
            ;
    }

    /*
     * Create the single app-owned fsl_shell and register BOTH the "bt" and
     * "wifi" commands. The stock edgefast_open a2dp_sink example (in the
     * middleware) has no shell hook, so - unlike a2dp_source - we cannot call
     * app_shell_init() from bt_ready(). We must NOT modify any middleware file,
     * so the coex app drives shell init here. app_shell_init() only needs the
     * debug-console serial handle (set up by BOARD_InitHardware) and the
     * compile-time "bt"/"wifi" command definitions; it does not depend on the
     * BT stack being ready, so this deterministic point is safe.
     */
    app_shell_init();

    /* Initialization job is complete, destroy the task */
    vTaskDelete(NULL);
}

int main(void)
{
    BOARD_InitHardware();

#if defined(MBEDTLS_THREADING_C) && defined(MBEDTLS_THREADING_ALT)
    config_mbedtls_threading_alt();
#endif

    printSeparator();
    PRINTF("        Coex A2DP Sink APP\r\n");
    printSeparator();

    BaseType_t result =
        xTaskCreate(task_main, "main", TASK_MAIN_STACK_SIZE, NULL, TASK_MAIN_PRIO, &task_main_handle);
    assert(pdPASS == result);

    (void)result;
    vTaskStartScheduler();
    for (;;)
    {
        ;
    }
}

#if CONFIG_COEX_APP
void vApplicationIdleHook(void)
{
#if (CONFIG_OT_CLI)
    otSysRunIdleTask();
#endif
}
#endif

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    PRINTF("ERROR: stack overflow on task %s.\r\n", pcTaskName);

    portDISABLE_INTERRUPTS();

    (void)xTask;
    (void)pcTaskName;

    for (;;)
    {
    }
}

#ifndef __GNUC__
void __assert_func(const char *file, int line, const char *func, const char *failedExpr)
{
    PRINTF("ASSERT ERROR \" %s \": file \"%s\" Line \"%d\" function name \"%s\" \n", failedExpr, file, line, func);
    for (;;)
    {
        __BKPT(0);
    }
}
#endif
