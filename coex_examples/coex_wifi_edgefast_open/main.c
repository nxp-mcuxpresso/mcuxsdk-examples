/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include "fsl_debug_console.h"
#include "fsl_shell.h"

#include "FreeRTOS.h"
#include "task.h"

#include "shell.h"
#include <zephyr/shell/shell.h>
#include "wlan.h"
#include "wlan_bt_fw.h"
#include "cli.h"

/* Forward declare ping/iperf CLI init - avoid including wifi_ping.h
 * which requires lwip ip_addr_t to be visible at this point */
extern int ping_cli_init(void);
extern int iperf_cli_init(void);

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define TASK_MAIN_PRIO       (configMAX_PRIORITIES - 5)
#define TASK_MAIN_STACK_SIZE (4 * 1024)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void BOARD_InitHardware(void);
extern struct shell *ctx_shell;
extern const struct cli_command *lookup_command(char *name, int len);
extern void help_command(int argc, char **argv);
extern int cli_register_command(const struct cli_command *command);

static shell_status_t cmd_wifi(shell_handle_t shellHandle, int32_t argc, char **argv);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile int g_wlan_initialized = 0;

SHELL_COMMAND_DEFINE(wifi,
                     "\r\n\"wifi\": Execute Wi-Fi CLI commands\r\n"
                     "  Usage: wifi <command> [args]\r\n"
                     "  Examples:\r\n"
                     "    wifi wlan-scan\r\n"
                     "    wifi wlan-version\r\n"
                     "    wifi wlan-mac\r\n"
                     "    wifi wlan-connect <profile>\r\n"
                     "    wifi wlan-stat\r\n"
                     "    wifi ping <ip>\r\n"
                     "    wifi iperf -s\r\n"
                     "    wifi help\r\n",
                     cmd_wifi,
                     SHELL_IGNORE_PARAMETER_COUNT);

/*******************************************************************************
 * Code
 ******************************************************************************/

static void printSeparator(void)
{
    (void)PRINTF("========================================\r\n");
}

/**
 * @brief Wi-Fi shell command handler.
 *
 * Looks up the WLAN CLI command by name and executes it directly.
 * This uses lookup_command() (requires COEX_APP_SUPPORT defined) to find
 * the registered Wi-Fi CLI command and call it in the shell task context.
 *
 * Usage from shell: wifi wlan-scan, wifi wlan-version, wifi ping <ip>, etc.
 */
static shell_status_t cmd_wifi(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    const struct cli_command *command = NULL;

    if (argc < 2)
    {
        (void)PRINTF("Usage: wifi <command> [args]\r\n");
        (void)PRINTF("Type 'wifi help' to list available Wi-Fi commands\r\n");
        return kStatus_SHELL_Success;
    }

    /* Shift argv to skip "wifi" prefix */
    argc--;
    argv++;

    /* lookup_command() searches the registered WLAN CLI commands by name.
     * Pass 0 for len to match the full command name. */
    command = lookup_command(argv[0], 0);
    if (command != NULL)
    {
        command->function(argc, argv);
    }
    else
    {
        (void)PRINTF("Unknown wifi command: %s\r\n", argv[0]);
        (void)PRINTF("Type 'wifi help' to list available commands\r\n");
    }

    return kStatus_SHELL_Success;
}

/* Callback Function passed to WLAN Connection Manager */
int wlan_event_callback(enum wlan_event_reason reason, void *data)
{
    int ret;

    switch (reason)
    {
        case WLAN_REASON_INITIALIZED:
            (void)PRINTF("app_cb: WLAN initialized\r\n");
            printSeparator();

            ret = wlan_basic_cli_init();
            if (ret != WM_SUCCESS)
            {
                (void)PRINTF("Failed to initialize BASIC WLAN CLIs\r\n");
                return 0;
            }

            ret = wlan_cli_init();
            if (ret != WM_SUCCESS)
            {
                (void)PRINTF("Failed to initialize WLAN CLIs\r\n");
                return 0;
            }
            (void)PRINTF("WLAN CLIs are initialized\r\n");
            printSeparator();

            ret = wlan_enhanced_cli_init();
            if (ret != WM_SUCCESS)
            {
                (void)PRINTF("Failed to initialize ENHANCED WLAN CLIs\r\n");
                return 0;
            }
            (void)PRINTF("ENHANCED WLAN CLIs are initialized\r\n");
            printSeparator();

            ret = ping_cli_init();
            if (ret != WM_SUCCESS)
            {
                (void)PRINTF("Failed to initialize PING CLI\r\n");
                return 0;
            }
            (void)PRINTF("PING CLI is initialized\r\n");
            printSeparator();

            ret = iperf_cli_init();
            if (ret != WM_SUCCESS)
            {
                (void)PRINTF("Failed to initialize IPERF CLI\r\n");
                return 0;
            }
            (void)PRINTF("IPERF CLI is initialized\r\n");
            printSeparator();

            /* Register help command so 'wifi help' works */
            {
                static const struct cli_command help_cmd = {"help", NULL, help_command};
                (void)cli_register_command(&help_cmd);
            }

            g_wlan_initialized = 1;
            break;

        case WLAN_REASON_SUCCESS:
            (void)PRINTF("app_cb: WLAN: connected\r\n");
            break;

        case WLAN_REASON_CONNECT_FAILED:
            (void)PRINTF("app_cb: WLAN: connect failed\r\n");
            break;

        case WLAN_REASON_NETWORK_NOT_FOUND:
            (void)PRINTF("app_cb: WLAN: network not found\r\n");
            break;

        case WLAN_REASON_NETWORK_AUTH_FAILED:
            (void)PRINTF("app_cb: WLAN: network auth failed\r\n");
            break;

        case WLAN_REASON_USER_DISCONNECT:
            (void)PRINTF("app_cb: WLAN: disconnected\r\n");
            break;

        default:
            break;
    }
    return 0;
}

static void task_main(void *param)
{
    int32_t result;

    printSeparator();
#if defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
    (void)PRINTF("     Initialize Firecrest-2EL (IW612) M2 Module\r\n");
#elif defined(WIFI_IW416_BOARD_MURATA_1XK_M2)
    (void)PRINTF("     Initialize IW416 1XK Direct-M2 Module\r\n");
#else
    (void)PRINTF("     Initialize WLAN Driver\r\n");
#endif
    printSeparator();

    /* Initialize WIFI Driver - downloads combo firmware */
    result = wlan_init(wlan_fw_bin, wlan_fw_bin_len);
    assert(0 == result);

    result = wlan_start(wlan_event_callback);
    assert(0 == result);
    (void)PRINTF("WLAN started successfully\r\n");

    /* Wait for WLAN initialized callback */
    while (g_wlan_initialized == 0)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    (void)PRINTF("Starting BT shell...\r\n");
    printSeparator();

    /* Start BT shell in this task context */
    shell_task(NULL);
}

/*
 * This task waits for the BT shell to be initialized, then registers
 * the "wifi" command on it.
 */
static void wifi_cmd_register_task(void *param)
{
    /* Wait for shell to be initialized */
    while (ctx_shell == NULL || ctx_shell->sh == NULL)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    /* Register wifi command on the BT shell */
    (void)SHELL_RegisterCommand(ctx_shell->sh, SHELL_COMMAND(wifi));
    (void)PRINTF("\r\nWi-Fi commands registered. Use: wifi <command>\r\n");
    (void)PRINTF("Type 'wifi help' for available Wi-Fi commands\r\n\r\n");

    /* Job done */
    vTaskDelete(NULL);
}

int main(void)
{
    BOARD_InitHardware();

    printSeparator();
    (void)PRINTF("coex_wifi_edgefast_open demo\r\n");
    printSeparator();

    if (xTaskCreate(task_main, "main", TASK_MAIN_STACK_SIZE, NULL, TASK_MAIN_PRIO, NULL) != pdPASS)
    {
        PRINTF("task_main creation failed!\r\n");
        while (1)
            ;
    }

    /* Task to register wifi command after shell is ready */
    if (xTaskCreate(wifi_cmd_register_task, "wifi_reg", 512, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
    {
        PRINTF("wifi_cmd_register_task creation failed!\r\n");
        while (1)
            ;
    }

    vTaskStartScheduler();
    for (;;)
        ;
}
