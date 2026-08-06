/*
 * Copyright 2019 - 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Clean Model B app-owned shell layer for the coex HFP Audio Gateway (AG) app.
 *
 * Uses the plain fsl_shell API (edgefast_open PORT_SHELL). This file owns the
 * single shell instance and registers BOTH the "bt" (HFP-AG control) and
 * "wifi" (WLAN CLI dispatch) commands in one place. The shell handle is static.
 *
 * The "wifi" handler only needs cli.h (lookup_command/help_command). cli.h pulls
 * only wmtypes.h - NOT wifi.h/wpa_supplicant common.h - so it does not trigger
 * the __maybe_unused redefinition clash with the edgefast_open zephyr gcc.h.
 *
 * No Zephyr shell (SHELL_CMD_ARG_REGISTER), no custom SHELL_InitPost override,
 * and no CONFIG_BT_SHELL. The edgefast_open handsfree_ag task calls
 * app_shell_init() from bt_ready(). This app-local app_shell.c shadows the stock
 * handsfree_ag one via CMake include-path ordering.
 */

#include <string.h>
#include <stdint.h>
#include <errno.h>
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
#include <zephyr/bluetooth/classic/hfp_ag.h>
#include "fsl_debug_console.h"
#include "fsl_shell.h"
#include "cli.h"
#include "app_shell.h"
#include "app_discover.h"
#include "app_connect.h"
#include "app_handsfree_ag.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static shell_status_t shellBt(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t cmd_wifi(shell_handle_t shellHandle, int32_t argc, char **argv);

/*******************************************************************************
 * Variables
 ******************************************************************************/

SHELL_COMMAND_DEFINE(bt,
                     "\r\n\"bt\": BT HFP Audio Gateway control\r\n"
                     "  USAGE: bt [discover|connect|disconnect|delete]\r\n"
                     "    discover             start to find BT devices\r\n"
                     "    connect              connect to the device that is found, for example: bt connect n (from 1)\r\n"
                     "    select_ag <0|1>      select the ag conn to process\r\n"
                     "    openaudio            open audio connection without calls on the selected ag conn\r\n"
                     "    closeaudio           close audio connection without calls on the selected ag conn\r\n"
                     "    sincall              start an incoming call on the selected ag conn\r\n"
                     "    aincall [index]      accept the call on the selected ag conn\r\n"
                     "    eincall [index]      end a call on the selected ag conn\r\n"
                     "    select_codec         codec select for codec negotiation, e.g. bt select_codec 2\r\n"
                     "    set_mic_volume       update mic Volume, e.g. bt set_mic_volume 14\r\n"
                     "    set_speaker_volume   update speaker Volume, e.g. bt set_speaker_volume 14\r\n"
                     "    stwcincall           start multiple incoming call on the selected ag conn\r\n"
                     "    disconnect <index>   disconnect the acl connection (bt_conn_index)\r\n"
                     "    delete               delete all bonded devices (disconnect first)\r\n"
                     "    set_hf_ind <1|2> <enable|disable>   enable/disable hf indicator (1 - enh driver safety; 2 - battery)\r\n",
                     shellBt,
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
static uint32_t hfp_get_value_from_str(char *ch)
{
    uint8_t selectIndex = 0;
    uint8_t value       = 0;

    for (selectIndex = 0; selectIndex < strlen(ch); ++selectIndex)
    {
        if ((ch[selectIndex] < '0') || (ch[selectIndex] > '9'))
        {
            PRINTF("The Dial parameter is wrong\r\n");
            return kStatus_SHELL_Error;
        }
    }

    if (selectIndex == 0U)
    {
        PRINTF("The Dial parameter is wrong\r\n");
    }
    else if (selectIndex == 1U)
    {
        value = (ch[0] - '0');
    }
    else if (selectIndex == 2U)
    {
        value = (ch[0] - '0') * 10 + (ch[1] - '0');
    }
    return value;
}

static int app_get_selected_index(char *ch, uint8_t *index)
{
    uint8_t selectIndex = 0;

    for (selectIndex = 0; selectIndex < strlen(ch); ++selectIndex)
    {
        if ((ch[selectIndex] < '0') || (ch[selectIndex] > '9'))
        {
            PRINTF("the parameter is wrong\r\n");
            return -EINVAL;
        }
    }

    switch (strlen(ch))
    {
        case 1:
            selectIndex = ch[0] - '0';
            break;
        case 2:
            selectIndex = (ch[0] - '0') * 10 + (ch[1] - '0');
            break;
        default:
            PRINTF("the parameter is wrong\r\n");
            return -EINVAL;
    }

    *index = selectIndex;

    return 0;
}

static shell_status_t shellBt(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    uint8_t *addr;

    if (argc < 2)
    {
        PRINTF("the parameter count is wrong\r\n");
        return kStatus_SHELL_Error;
    }

    if (strcmp(argv[1], "discover") == 0)
    {
        app_discover();
    }
    else if (strcmp(argv[1], "connect") == 0)
    {
        uint8_t selectIndex = 0;

        if (argc < 3)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }

        if (app_get_selected_index((char *)argv[2], &selectIndex))
        {
            return kStatus_SHELL_Error;
        }

        if (selectIndex == 0U)
        {
            PRINTF("the parameter is wrong\r\n");
            return kStatus_SHELL_Error;
        }
        addr = app_get_addr(selectIndex - 1);
        app_connect(addr);
    }
    else if (strcmp(argv[1], "disconnect") == 0)
    {
        uint8_t index = 0U;

        if (argc < 3)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }

        if (app_get_selected_index((char *)argv[2], &index))
        {
            return kStatus_SHELL_Error;
        }

        if (index >= CONFIG_BT_MAX_CONN)
        {
            PRINTF("the parameter is wrong\r\n");
            return kStatus_SHELL_Error;
        }

        app_disconnect(index);
    }
    else if (strcmp(argv[1], "select_ag") == 0)
    {
        char *index_str;
        uint8_t index = 0U;

        if (argc < 3)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }

        index_str = argv[2];

        if ((index_str[0] == '0') || (index_str[0] == '1'))
        {
            index = index_str[0] - '0';
        }

        if ((index != 0U) && (index != 1U))
        {
            PRINTF("the parameter is wrong\r\n");
            return kStatus_SHELL_Error;
        }

        app_hfp_ag_select_conn(index);
        PRINTF("success\r\n");
    }
    else if (strcmp(argv[1], "openaudio") == 0)
    {
        app_hfp_ag_open_audio();
    }
    else if (strcmp(argv[1], "closeaudio") == 0)
    {
        app_hfp_ag_close_audio();
    }
    else if (strcmp(argv[1], "sincall") == 0)
    {
        app_hfp_ag_start_incoming_call();
    }
    else if (strcmp(argv[1], "stwcincall") == 0)
    {
        app_hfp_ag_start_twc_incoming_call();
    }

    else if (strcmp(argv[1], "aincall") == 0)
    {
        uint8_t callIndex = 0;

        if (argc > 2)
        {
            callIndex = hfp_get_value_from_str(argv[2]);
            if (callIndex > CONFIG_BT_HFP_AG_MAX_CALLS)
            {
                PRINTF("the call index exceeds maximum allowed calls (%d)\r\n", CONFIG_BT_HFP_AG_MAX_CALLS);
                return kStatus_SHELL_Error;
            }
        }

        app_hfp_ag_accept_incoming_call(callIndex);
    }
    else if (strcmp(argv[1], "eincall") == 0)
    {
        uint8_t callIndex = 0;

        if (argc > 2)
        {
            callIndex = hfp_get_value_from_str(argv[2]);
            if (callIndex > CONFIG_BT_HFP_AG_MAX_CALLS)
            {
                PRINTF("the call index exceeds maximum allowed calls (%d)\r\n", CONFIG_BT_HFP_AG_MAX_CALLS);
                return kStatus_SHELL_Error;
            }
        }
        app_hfp_ag_stop_incoming_call(callIndex);
    }
    else if (strcmp(argv[1], "select_codec") == 0)
    {
        if (argc < 3)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }
        app_hfp_ag_codec_select(hfp_get_value_from_str(argv[2]));
    }
    else if (strcmp(argv[1], "set_mic_volume") == 0)
    {
        if (argc < 3)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }
        app_hfp_ag_volume_update(hfp_ag_volume_type_mic, hfp_get_value_from_str(argv[2]));
    }
    else if (strcmp(argv[1], "set_speaker_volume") == 0)
    {
        if (argc < 3)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }
        app_hfp_ag_volume_update(hfp_ag_volume_type_speaker, hfp_get_value_from_str(argv[2]));
    }
    else if (strcmp(argv[1], "delete") == 0)
    {
        int err = bt_unpair(BT_ID_DEFAULT, NULL);
        if (err != 0)
        {
            PRINTF("failed reason = %d\r\n", err);
        }
        else
        {
            PRINTF("success\r\n");
        }
    }
    else if (strcmp(argv[1], "set_hf_ind") == 0)
    {
        char *indicator_str;
        char *control_str;
        uint8_t indicator = 0xFFU;
        uint8_t control   = 0xFFU;

        if (argc < 4)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }

        indicator_str = argv[2];
        control_str   = argv[3];

        if ((indicator_str[0] == '1') || (indicator_str[0] == '2'))
        {
            indicator = indicator_str[0] - '0';
        }

        if (strcmp(control_str, "enable") == 0)
        {
            control = 1U;
        }
        else if (strcmp(control_str, "disable") == 0)
        {
            control = 0U;
        }

        if ((indicator != 0xFFU) && (control != 0xFFU))
        {
            int err = app_hfp_ag_set_hf_indicator(indicator, control);
            if (err)
            {
                PRINTF("fail to send cmd:%d\r\n", err);
            }
        }
        else
        {
            PRINTF("wrong parameter\r\n");
        }
    }
    else
    {
        PRINTF("%s unknown parameter: %s\r\n", argv[0], argv[1]);
    }

    return kStatus_SHELL_Success;
}

/**
 * @brief Wi-Fi shell command handler.
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
