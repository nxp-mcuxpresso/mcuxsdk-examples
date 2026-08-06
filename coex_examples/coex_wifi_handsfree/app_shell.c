/*
 * Copyright 2021 - 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Clean Model B app-owned shell layer for the coex HFP Hands-Free (HF) app.
 *
 * Uses the plain fsl_shell API (edgefast_open PORT_SHELL). This file owns the
 * single shell instance and registers BOTH the "bt" (HFP-HF control) and
 * "wifi" (WLAN CLI dispatch) commands in one place. The shell handle is static;
 * nothing outside this file needs it.
 *
 * The "wifi" handler only needs cli.h (lookup_command/help_command). cli.h pulls
 * only wmtypes.h - NOT wifi.h/wpa_supplicant common.h - so it does not trigger
 * the __maybe_unused redefinition clash with the edgefast_open zephyr gcc.h.
 *
 * No Zephyr shell (SHELL_CMD_ARG_REGISTER), no custom SHELL_InitPost override,
 * and no CONFIG_BT_SHELL. The edgefast_open handsfree task calls app_shell_init()
 * from bt_ready(). This app-local app_shell.c shadows the stock handsfree one via
 * CMake include-path ordering.
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
#include "fsl_debug_console.h"
#include "fsl_shell.h"
#include "cli.h"
#include "app_shell.h"
#include <zephyr/bluetooth/classic/hfp_hf.h>
#include "app_handsfree.h"
#include "app_connect.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static shell_status_t shellBt(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t cmd_wifi(shell_handle_t shellHandle, int32_t argc, char **argv);

/*******************************************************************************
 * Variables
 ******************************************************************************/

SHELL_COMMAND_DEFINE(bt,
                     "\r\n\"bt\": BT HFP Hands-Free control\r\n"
                     "  USAGE: bt [dial|aincall|eincall]\r\n"
                     "    dial          dial out call.\r\n"
                     "    aincall       accept the incoming call.\r\n"
                     "    eincall       end an incoming call.\r\n"
                     "    toutcall      terminate outgoing call.\r\n"
                     "    svr           start voice recognition.\r\n"
                     "    evr           stop voice recognition.\r\n"
                     "    clip          enable CLIP notification.\r\n"
                     "    disclip       disable CLIP notification.\r\n"
                     "    ccwa          enable call waiting notification.\r\n"
                     "    disccwa       disable call waiting notification.\r\n"
                     "    micVolume     Update mic Volume.\r\n"
                     "    speakerVolume Update Speaker Volume.\r\n"
                     "    lastdial      call the last dial number.\r\n"
                     "    voicetag      Get Voice-tag Phone Number (BINP).\r\n"
                     "    multipcall    multiple call option.\r\n"
                     "    triggercodec  trigger codec connection.\r\n"
                     "    clcc          Query list of current calls.\r\n",
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

static shell_status_t shellBt(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    if (argc < 2)
    {
        PRINTF("the parameter count is wrong\r\n");
        return kStatus_SHELL_Error;
    }

    if (strcmp(argv[1], "dial") == 0)
    {
        uint8_t selectIndex = 0;
        char *ch;

        if (argc < 3)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }
        ch = argv[2];

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
        hfp_dial(ch);
    }
    else if (strcmp(argv[1], "svr") == 0)
    {
        hfp_start_voice_recognition();
    }
    else if (strcmp(argv[1], "evr") == 0)
    {
        hfp_stop_voice_recognition();
    }
    else if (strcmp(argv[1], "micVolume") == 0)
    {
        if (argc < 3)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }
        hfp_volume_update(hf_volume_type_mic, hfp_get_value_from_str(argv[2]));
    }
    else if (strcmp(argv[1], "speakerVolume") == 0)
    {
        if (argc < 3)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }
        hfp_volume_update(hf_volume_type_speaker, hfp_get_value_from_str(argv[2]));
    }
    else if (strcmp(argv[1], "lastdial") == 0)
    {
        hfp_last_dial();
    }
    else if (strcmp(argv[1], "memorydial") == 0)
    {
        if (argc < 3)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }
        dial_memory(hfp_get_value_from_str(argv[2]));
    }
    else if (strcmp(argv[1], "clip") == 0)
    {
        hfp_enable_clip(1);
    }
    else if (strcmp(argv[1], "disclip") == 0)
    {
        hfp_enable_clip(0);
    }
    else if (strcmp(argv[1], "ccwa") == 0)
    {
        hfp_enable_ccwa(1);
    }
    else if (strcmp(argv[1], "disccwa") == 0)
    {
        hfp_enable_ccwa(0);
    }
    else if (strcmp(argv[1], "multipcall") == 0)
    {
        if (argc < 3)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }
        hfp_multiparty_call_option(hfp_get_value_from_str(argv[2]));
    }
    else if (strcmp(argv[1], "aincall") == 0)
    {
        hfp_AnswerCall();
    }
    else if (strcmp(argv[1], "toutcall") == 0)
    {
        hfp_terminate();
    }
    else if (strcmp(argv[1], "voicetag") == 0)
    {
        hfp_hf_get_last_voice_tag_number();
    }
    else if (strcmp(argv[1], "eincall") == 0)
    {
        hfp_RejectCall();
    }
    else if (strcmp(argv[1], "triggercodec") == 0)
    {
        hfp_trigger_codec_connection();
    }
    else if (strcmp(argv[1], "clcc") == 0)
    {
        hfp_hf_query_list_current_calls();
    }
    else
    {
        PRINTF("%s unknown parameter: %s\r\n", argv[0], argv[1]);
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
 * Called by the edgefast_open handsfree task from bt_ready(). Creates the single
 * app-owned fsl_shell and registers BOTH the "bt" and "wifi" commands, giving one
 * prompt (@Coex>). WLAN CLIs (wlan-scan, ping, iperf, ...) are registered by the
 * coex middleware wlan_event_callback and reached via "wifi <wlan-command>".
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
