/*
 * Copyright 2021 - 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Clean Model B app-owned shell layer for the coex SPP app.
 *
 * Uses the plain fsl_shell API (edgefast_open PORT_SHELL). This file owns the
 * single shell instance and registers the "bt" (BR/EDR connection control),
 * "spp" (SPP RFCOMM control) and "wifi" (WLAN CLI dispatch) commands in one
 * place. The shell handle is static; nothing outside this file needs it.
 *
 * The "wifi" handler only needs cli.h (lookup_command/help_command). cli.h pulls
 * only wmtypes.h - NOT wifi.h/wpa_supplicant common.h - so it does not trigger
 * the __maybe_unused redefinition clash with the edgefast_open zephyr gcc.h.
 *
 * No Zephyr shell (SHELL_CMD_ARG_REGISTER), no custom SHELL_InitPost override,
 * and no CONFIG_BT_SHELL. The edgefast_open spp task calls app_shell_init() from
 * bt_ready(). This app-local app_shell.c shadows the stock spp one via CMake
 * include-path ordering.
 */

#include <string.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdbool.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/slist.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/addr.h>
#include "fsl_debug_console.h"
#include "fsl_shell.h"
#include "cli.h"
#include "app_shell.h"
#include "app_spp.h"
#include "app_discover.h"
#include "app_connect.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SPP_UUID_16_STR  (char *)"1101"
#define SPP_UUID_128_STR (char *)"00001101-0000-1000-8000-00805F9B34FB"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static uint8_t shell_parse_parameter(int32_t argc, char **argv);
static shell_status_t shell_bt(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t shell_spp(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t cmd_wifi(shell_handle_t shellHandle, int32_t argc, char **argv);

/*******************************************************************************
 * Variables
 ******************************************************************************/
SDK_ALIGN(static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t s_shellHandle;

/* bt command */
SHELL_COMMAND_DEFINE(bt,
                     "\r\n\"bt\": BT related function\r\n"
                     "  USAGE: bt <discover|connect|disconnect|delete>\r\n"
                     "    bt conns          print all active bt connection\r\n"
                     "    bt switch <index> switch a bt connection\r\n"
                     "    bt discover       start to find BT devices\r\n"
                     "    bt connect        connect to the device that is found, for example: bt connect n (from 1)\r\n"
                     "    bt disconnect     disconnect current connection.\r\n"
                     "    bt delete         delete all devices. Ensure to disconnect the HCI link connection with the peer device before attempting to delete the bonding information.\r\n",
                     shell_bt,
                     SHELL_IGNORE_PARAMETER_COUNT);

/* spp command */
SHELL_COMMAND_DEFINE(spp,
                     "\r\n\"spp\": SPP related function\r\n"
                     "  USAGE: \r\n"
                     "    spp register <cid>        register a spp server channel(cid)\r\n"
                     "    spp uuidconnect [uuid]    connect to SPP service by UUID (default: standard SPP UUID if not specified)\r\n"
                     "    spp connect <cid>         create spp connection\r\n"
                     "    spp disconnect            disconnect current spp connection.\r\n"
                     "    spp send <1|2|3|4>        send data over spp connection.\r\n",
                     shell_spp,
                     SHELL_IGNORE_PARAMETER_COUNT);

/* wifi command */
SHELL_COMMAND_DEFINE(wifi,
                     "\r\n\"wifi\": Wi-Fi related function\r\n"
                     "  USAGE: wifi <wlan-command> [args]\r\n"
                     "  e.g.:  wifi wlan-scan, wifi wlan-version, wifi ping <ip>, wifi iperf ...\r\n",
                     cmd_wifi,
                     SHELL_IGNORE_PARAMETER_COUNT);

/*******************************************************************************
 * Code
 ******************************************************************************/
static uint8_t shell_parse_parameter(int32_t argc, char **argv)
{
    uint8_t select_index = 0U;
    char   *ch = argv[argc - 1];

    for (select_index = 0U; select_index < strlen(ch); select_index++)
    {
        if ((ch[select_index] < '0') || (ch[select_index] > '9'))
        {
            PRINTF("the parameter is wrong\r\n");
            return 0xFFU;
        }
    }

    switch (strlen(ch))
    {
    case 1:
        select_index = ch[0] - '0';
        break;
    case 2:
        select_index = (ch[0] - '0') * 10U + (ch[1] - '0');
        break;
    default:
        PRINTF("the parameter is wrong\r\n");
        select_index = 0xFFU;
        break;
    }

    return select_index;
}

static shell_status_t shell_bt(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    uint8_t *addr;
    uint8_t select_index = 0U;
    int     err          = 0;
    char    conn_addr[BT_ADDR_LE_STR_LEN];

    if (argc < 2)
    {
        PRINTF("the parameter count is wrong\r\n");
        return kStatus_SHELL_Error;
    }

    if (strcmp(argv[1], "discover") == 0)
    {
        app_discover();
    }
    else if (strcmp(argv[1], "conns") == 0)
    {
        PRINTF("Connected device address:\r\n");
        for (select_index = 0U; select_index < CONFIG_BT_MAX_CONN; select_index++)
        {
            if (NULL != br_conns[select_index])
            {
                bt_addr_to_str(bt_conn_get_dst_br(br_conns[select_index]), conn_addr, sizeof(conn_addr));
                PRINTF("conn handle %d: %s\r\n", select_index, conn_addr);
            }
        }

        if (NULL != default_conn)
        {
            bt_addr_to_str(bt_conn_get_dst_br(default_conn), conn_addr, sizeof(conn_addr));
            PRINTF("Default conn address:%s\r\n", conn_addr);
        }
    }
    else if (strcmp(argv[1], "switch") == 0)
    {
        select_index = shell_parse_parameter(argc, argv);
        if (0xFFU == select_index)
        {
            PRINTF("the parameter is wrong\r\n");
            return kStatus_SHELL_Error;
        }
        else
        {
            default_conn = br_conns[select_index];
            PRINTF("switch successful, selected index: %d\r\n", select_index);
        }
    }
    else if (strcmp(argv[1], "connect") == 0)
    {
        select_index = shell_parse_parameter(argc, argv);
        if (0xFFU == select_index)
        {
            PRINTF("the parameter is wrong\r\n");
            return kStatus_SHELL_Error;
        }
        else
        {
            addr = app_get_addr(select_index - 1U);
            app_connect(addr);
        }
    }
    else if (strcmp(argv[1], "disconnect") == 0)
    {
        app_disconnect();
    }
    else if (strcmp(argv[1], "delete") == 0)
    {
        err = bt_unpair(BT_ID_DEFAULT, NULL);
        if (err != 0)
        {
            PRINTF("failed reason = %d\r\n", err);
        }
        else
        {
            PRINTF("success\r\n");
        }
    }
    else
    {
        PRINTF("Invalid bt command, please enter help to get the bt command list.\n");
    }

    return kStatus_SHELL_Success;
}

static shell_status_t shell_spp(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    uint8_t select_index = 0U;

    if (argc < 2)
    {
        PRINTF("the parameter count is wrong\r\n");
        return kStatus_SHELL_Error;
    }

    if (strcmp(argv[1], "register") == 0)
    {
        select_index = shell_parse_parameter(argc, argv);
        if (0xFFU == select_index)
        {
            PRINTF("the parameter is wrong\r\n");
            return kStatus_SHELL_Error;
        }
        spp_appl_server_register(select_index);
    }
    else if (strcmp(argv[1], "uuidconnect") == 0)
    {
        char *uuidstr = NULL;

        /* If UUID parameter is provided, parse it; otherwise use default SPP UUID */
        if (argc >= 3)
        {
            /* User provided a UUID parameter */
            uuidstr = argv[2];
        }
        else
        {
            uuidstr = SPP_UUID_16_STR;
            if (spp_appl_connect_by_uuid(default_conn, uuidstr) < 0)
            {
                /* failure to try to use 128 uuid */
                uuidstr = SPP_UUID_128_STR;
            }
            else
            {
                /* successfully connected with 16-bit UUID, return here */
                return kStatus_SHELL_Success;
            }
        }

        if (spp_appl_connect_by_uuid(default_conn, uuidstr) < 0)
        {
            PRINTF("connect by uuid failure\r\n");
            return kStatus_SHELL_Error;
        }
    }
    else if (strcmp(argv[1], "connect") == 0)
    {
        select_index = shell_parse_parameter(argc, argv);
        if (0xFFU == select_index)
        {
            PRINTF("the parameter is wrong\r\n");
            return kStatus_SHELL_Error;
        }
        spp_appl_connect(default_conn, select_index);
    }
    else if (strcmp(argv[1], "disconnect") == 0)
    {
        spp_appl_disconnect();
    }
    else if (strcmp(argv[1], "send") == 0)
    {
        uint8_t *data = NULL;
        size_t   len  = 0;

        select_index = shell_parse_parameter(argc, argv);
        if (0xFFU == select_index)
        {
            PRINTF("the parameter is wrong\r\n");
            return kStatus_SHELL_Error;
        }

        /* Select AT command based on index */
        switch (select_index)
        {
            case 1:
                data = (uint8_t *)"AT+CIND=?\r\n";
                len  = strlen((const char *)data);
                break;
            case 2:
                data = (uint8_t *)"AT+CIND?\r\n";
                len  = strlen((const char *)data);
                break;
            case 3:
                data = (uint8_t *)"ATEP\r\n";
                len  = strlen((const char *)data);
                break;
            case 4:
                data = (uint8_t *)"AT+CKPD=E\r\n";
                len  = strlen((const char *)data);
                break;
            default:
                PRINTF("Invalid send index (valid: 1-4)\r\n");
                return kStatus_SHELL_Error;
        }

        spp_appl_send(data, len);
    }
    else
    {
        PRINTF("Invalid spp command, please enter help to get the spp command list.\n");
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
 * Called by the edgefast_open spp task from bt_ready(). Creates the single
 * app-owned fsl_shell and registers the "bt", "spp" and "wifi" commands, giving
 * one prompt (@Coex>). WLAN CLIs (wlan-scan, ping, iperf, ...) are registered by
 * the coex middleware wlan_event_callback and reached via "wifi <wlan-command>".
 */
void app_shell_init(void)
{
    DbgConsole_Flush();

    s_shellHandle = &s_shellHandleBuffer[0];
    (void)SHELL_Init(s_shellHandle, g_serialHandle, "@Coex> ");
    PRINTF("\r\n");

    (void)SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(bt));
    (void)SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(spp));
#if (CONFIG_WIFI_BLE_COEX_APP)
    (void)SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(wifi));
#endif
}
