/*
 * Copyright 2020 - 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Clean Model B app-owned shell layer for the coex A2DP source app.
 *
 * Uses the plain fsl_shell API (edgefast_open PORT_SHELL). This file owns the
 * single shell instance and registers BOTH the "bt" (A2DP-source control) and
 * "wifi" (WLAN CLI dispatch) commands in one place - matching the proven layout
 * of the original ethermind coex_wifi_a2dp_source app. The shell handle is
 * static; nothing outside this file needs it.
 *
 * The "wifi" handler only needs cli.h (lookup_command/help_command). cli.h pulls
 * only wmtypes.h - NOT wifi.h/wpa_supplicant common.h - so it does not trigger
 * the __maybe_unused redefinition clash with the edgefast_open zephyr gcc.h.
 * That clash is specific to wifi.h, which is NOT included here.
 *
 * No Zephyr shell (SHELL_CMD_ARG_REGISTER), no custom SHELL_InitPost override,
 * and no CONFIG_BT_SHELL. The edgefast_open a2dp_source task calls
 * app_shell_init() from bt_ready().
 */

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
/* Zephyr BT headers are needed for bt_br_unpair(). These are safe to include
 * here because app_shell.c does NOT include wifi.h (which is what triggers the
 * __maybe_unused redefinition clash between wpa_supplicant common.h and the
 * edgefast_open zephyr gcc.h - that clash only affects main.c). */
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/classic/classic.h>
#include "fsl_debug_console.h"
#include "fsl_shell.h"
#include "cli.h"
#include "app_shell.h"
#include "app_discover.h"
#include "app_connect.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static shell_status_t shell_bt(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t cmd_wifi(shell_handle_t shellHandle, int32_t argc, char **argv);

/*******************************************************************************
 * Variables
 ******************************************************************************/

SHELL_COMMAND_DEFINE(bt,
                     "\r\n\"bt\": BT A2DP source control\r\n"
                     "  USAGE: bt [discover|connect <n>|disconnect|delete]\r\n"
                     "    discover      start to find BT devices\r\n"
                     "    connect <n>   connect to the n-th found device (from 1)\r\n"
                     "    disconnect    disconnect current connection\r\n"
                     "    delete        delete all bonded devices (disconnect first)\r\n",
                     shell_bt,
                     SHELL_IGNORE_PARAMETER_COUNT);

SHELL_COMMAND_DEFINE(wifi,
                     "\r\n\"wifi\": Wi-Fi related function\r\n"
                     "  USAGE: wifi <wlan-command> [args]\r\n"
                     "  e.g.:  wifi wlan-scan, wifi wlan-version, wifi ping <ip>, wifi iperf ...\r\n",
                     cmd_wifi,
                     SHELL_IGNORE_PARAMETER_COUNT);

SDK_ALIGN(static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t s_shellHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/

static shell_status_t shell_bt(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    uint8_t *addr;

    if ((argc < 2) || (strcmp(argv[1], "help") == 0))
    {
        SHELL_Printf(shellHandle,
                     "\r\n\"bt\": BT A2DP source control\r\n"
                     "  USAGE: bt [discover|connect <n>|disconnect|delete|help]\r\n"
                     "    discover      start to find BT devices\r\n"
                     "    connect <n>   connect to the n-th found device (from 1)\r\n"
                     "    disconnect    disconnect current connection\r\n"
                     "    delete        delete all bonded devices (disconnect first)\r\n"
                     "    help          show this help\r\n");
        return kStatus_SHELL_Success;
    }

    if (strcmp(argv[1], "discover") == 0)
    {
        app_discover();
    }
    else if (strcmp(argv[1], "connect") == 0)
    {
        uint8_t select_index = 0;
        char *ch;

        if (argc < 3)
        {
            SHELL_Printf(shellHandle, "the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }
        ch = argv[2];

        for (select_index = 0; select_index < strlen(ch); ++select_index)
        {
            if ((ch[select_index] < '0') || (ch[select_index] > '9'))
            {
                SHELL_Printf(shellHandle, "the parameter is wrong\r\n");
                return kStatus_SHELL_Error;
            }
        }

        switch (strlen(ch))
        {
            case 1:
                select_index = ch[0] - '0';
                break;
            case 2:
                select_index = (ch[0] - '0') * 10 + (ch[1] - '0');
                break;
            default:
                SHELL_Printf(shellHandle, "the parameter is wrong\r\n");
                return kStatus_SHELL_Error;
        }

        if (select_index == 0U)
        {
            SHELL_Printf(shellHandle, "the parameter is wrong\r\n");
            return kStatus_SHELL_Error;
        }
        addr = app_get_addr(select_index - 1);
        app_connect(addr);
    }
    else if (strcmp(argv[1], "disconnect") == 0)
    {
        app_disconnect();
    }
    else if (strcmp(argv[1], "delete") == 0)
    {
        int err = bt_br_unpair(NULL);
        if (err != 0)
        {
            SHELL_Printf(shellHandle, "failed reason = %d\r\n", err);
        }
        else
        {
            SHELL_Printf(shellHandle, "success\r\n");
        }
    }
    else
    {
        SHELL_Printf(shellHandle, "%s unknown parameter: %s\r\n", argv[0], argv[1]);
        return kStatus_SHELL_Error;
    }

    return kStatus_SHELL_Success;
}

/**
 * @brief Wi-Fi shell command handler.
 *
 * Strips the "wifi" prefix, looks up the WLAN CLI command by name and executes
 * it. The WLAN CLIs themselves are registered by the coex middleware's
 * wlan_event_callback (controller_coex_nxp.c) on WLAN_REASON_INITIALIZED.
 * Usage from shell: wifi wlan-scan, wifi wlan-version, wifi ping <ip>, ...
 */
static shell_status_t cmd_wifi(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    const struct cli_command *command = NULL;

    /* No subcommand, or "wifi help": list all registered WLAN CLI commands.
     * The WLAN CLIs do not register a built-in "help" (that is added by
     * cli_init(), which is not used in this app - fsl_shell is the input
     * engine), so we call help_command() directly to enumerate cli.commands[]. */
    if ((argc < 2) || (strcmp(argv[1], "help") == 0))
    {
        SHELL_Printf(shellHandle, "\r\nWi-Fi commands (use: wifi <command> [args]):\r\n");
        help_command(0, NULL);
        return kStatus_SHELL_Success;
    }

    /* Shift argv to skip the "wifi" prefix */
    argc--;
    argv++;

    command = lookup_command(argv[0], strlen(argv[0]));
    if (command != NULL)
    {
        command->function(argc, argv);
    }
    else
    {
        SHELL_Printf(shellHandle, "Unknown wifi subcommand: %s\r\n", argv[0]);
    }

    return kStatus_SHELL_Success;
}

/*
 * Called by the edgefast_open a2dp_source task from bt_ready(). Creates the
 * single app-owned fsl_shell and registers BOTH the "bt" and "wifi" commands,
 * giving one prompt (@Coex>). WLAN CLIs (wlan-scan, ping, iperf, ...) are
 * registered by the coex middleware wlan_event_callback and reached via
 * "wifi <wlan-command>".
 */
void app_shell_init(void)
{
    DbgConsole_Flush();

    s_shellHandle = &s_shellHandleBuffer[0];
    (void)SHELL_Init(s_shellHandle, g_serialHandle, "@Coex> ");
    PRINTF("\r\n");

    (void)SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(bt));
#if (CONFIG_WIFI_BLE_COEX_APP)
    (void)SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(wifi));
#endif
}
