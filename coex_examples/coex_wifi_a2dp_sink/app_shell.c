/*
 * Copyright 2020 - 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Clean Model B app-owned shell layer for the coex A2DP sink app.
 *
 * Uses the plain fsl_shell API (edgefast_open PORT_SHELL). This file owns the
 * single shell instance and registers BOTH the "bt" (A2DP-sink control) and
 * "wifi" (WLAN CLI dispatch) commands in one place. The shell handle is static;
 * nothing outside this file needs it.
 *
 * The "wifi" handler only needs cli.h (lookup_command/help_command). cli.h pulls
 * only wmtypes.h - NOT wifi.h/wpa_supplicant common.h - so it does not trigger
 * the __maybe_unused redefinition clash with the edgefast_open zephyr gcc.h.
 * That clash is specific to wifi.h, which is NOT included here.
 *
 * No Zephyr shell (SHELL_CMD_ARG_REGISTER), no custom SHELL_InitPost override,
 * and no CONFIG_BT_SHELL. The edgefast_open a2dp_sink task calls app_shell_init()
 * from bt_ready().
 */

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
/* Zephyr BT headers are needed for bt_br_unpair()/bt_conn_disconnect(). These
 * are safe to include here because app_shell.c does NOT include wifi.h (which is
 * what triggers the __maybe_unused redefinition clash between wpa_supplicant
 * common.h and the edgefast_open zephyr gcc.h). */
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/classic/classic.h>
#include "fsl_debug_console.h"
#include "fsl_shell.h"
#include "cli.h"
#include "app_shell.h"
#include "app_a2dp_sink.h"
#include "app_connect.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static shell_status_t shell_bt(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t cmd_wifi(shell_handle_t shellHandle, int32_t argc, char **argv);

/* app_connect.c: become connectable/discoverable and auto-connect to bonded peer. */
extern void app_a2dp_snk_auto_connect(void);
extern void app_a2dp_set_connectable(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

SHELL_COMMAND_DEFINE(bt,
                     "\r\n\"bt\": BT A2DP sink control\r\n"
                     "  USAGE: bt [connect|disconnect|delete|help]\r\n"
                     "    connect       become connectable/discoverable (wait for a source)\r\n"
                     "    disconnect    disconnect current connection\r\n"
                     "    delete        delete all bonded devices\r\n",
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
    if ((argc < 2) || (strcmp(argv[1], "help") == 0))
    {
        SHELL_Printf(shellHandle,
                     "\r\n\"bt\": BT A2DP sink control\r\n"
                     "  USAGE: bt [connect|disconnect|delete|help]\r\n"
                     "    connect       become connectable/discoverable (wait for a source)\r\n"
                     "    disconnect    disconnect current connection\r\n"
                     "    delete        delete all bonded devices\r\n"
                     "    help          show this help\r\n");
        return kStatus_SHELL_Success;
    }

    if (strcmp(argv[1], "connect") == 0)
    {
        app_a2dp_snk_auto_connect();
    }
    else if (strcmp(argv[1], "disconnect") == 0)
    {
        if (default_conn != NULL)
        {
            int err = bt_conn_disconnect(default_conn, BT_HCI_ERR_REMOTE_USER_TERM_CONN);
            if (err != 0)
            {
                SHELL_Printf(shellHandle, "disconnect failed reason = %d\r\n", err);
                return kStatus_SHELL_Error;
            }
        }
        else
        {
            SHELL_Printf(shellHandle, "no active connection\r\n");
        }
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
 * Called by the coex app's main.c (task_main), after the a2dp_sink task is
 * started. Unlike a2dp_source, the stock edgefast_open a2dp_sink example has no
 * shell hook in bt_ready() and we must not modify any middleware file, so shell
 * init is driven from the coex app-local main.c instead. Creates the single
 * app-owned fsl_shell and registers BOTH the "bt" and "wifi" commands.
 */
void app_shell_init(void)
{
    DbgConsole_Flush();

    s_shellHandle = &s_shellHandleBuffer[0];
    (void)SHELL_Init(s_shellHandle, g_serialHandle, "@Coex> ");
    PRINTF("\r\n");

    (void)SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(bt));
    (void)SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(wifi));
}
