/*
 * Copyright 2020 - 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "ff.h"
#include <stdbool.h>
#include <sys/atomic.h>
#include <sys/byteorder.h>
#include <sys/util.h>
#include <sys/slist.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include "fsl_debug_console.h"
#include "fsl_shell.h"
#include "app_shell.h"
#include "app_discover.h"
#include "app_connect.h"

#if !defined(RW612_SERIES)
#include "db_gen.h"
#include "host_msd_fatfs.h"
#endif

#include "cli.h"
#include "coex_cli.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static shell_status_t shell_bt(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t cmd_wifi(shell_handle_t shellHandle, int32_t argc, char **argv);

/*******************************************************************************
 * Variables
 ******************************************************************************/

SHELL_COMMAND_DEFINE(bt,
                     "\r\n\"bt\": BT related function\r\n"
                     "  USAGE: bt [discover|connect|disconnect|delete]\r\n"
                     "    discover    start to find BT devices\r\n"
                     "    connect     connect to the device that is found, for example: bt connectdevice n (from 1)\r\n"
                     "    disconnect  disconnect current connection.\r\n"
                     "    delete      delete all devices. Ensure to disconnect the HCI link connection with the peer device before attempting to delete the bonding information.\r\n",
                     shell_bt,
                     SHELL_IGNORE_PARAMETER_COUNT);

SHELL_COMMAND_DEFINE(wifi,
                     "\r\n\"wifi\": wifi related function\r\n"
                     "  USAGE: wifi [subcommand]\r\n",
                     cmd_wifi,
                     SHELL_IGNORE_PARAMETER_COUNT);

SDK_ALIGN(static uint8_t shell_handle_buffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t shell_handle;

/*******************************************************************************
 * Code
 ******************************************************************************/

#if(CONFIG_WIFI_BLE_COEX_APP && (CONFIG_DISABLE_BLE == 0))
static char s_shellCoexPrompt[32] = "@Coex> ";
#elif(CONFIG_DISABLE_BLE)
static char s_shellCoexPrompt[32] = "@wifi> ";
#else
static char s_shellCoexPrompt[32] = "@ble> ";
#endif

#define WIFI_PREFIX "wifi"

static void process_cmd_wifi(int32_t argc, char **argv) 
{
    const struct cli_command *command = NULL;

    if (argc < 2)
    {
        PRINTF("Missing wifi subcommand\r\n");
        return;
    }

    // Shift argv to skip "wifi"
    argc--;                // argc = argc - 1
    argv++;                // argv[0] = actual subcommand

    // Lookup the command
    command = lookup_command(argv[0], strlen(argv[0]));

    if (command != NULL)
    {
        command->function(argc, argv);
        PRINTF("Command %s executed\r\n", command->name);
    }
    else
    {
        PRINTF("Unknown wifi subcommand: %s\r\n", argv[0]);
    }
}


static shell_status_t cmd_wifi(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    process_cmd_wifi(argc, argv);
    return kStatus_SHELL_Success;
}

static shell_status_t shell_bt(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    uint8_t *addr;

    if (argc < 1)
    {
        PRINTF("the parameter count is wrong\r\n");
    }

    if (strcmp(argv[1], "discover") == 0)
    {
        app_discover();
    }
    else if (strcmp(argv[1], "connect") == 0)
    {
        uint8_t select_index = 0;
        char *ch = argv[2];

        if (argc < 2)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }

        for (select_index = 0; select_index < strlen(ch); ++select_index)
        {
            if ((ch[select_index] < '0') || (ch[select_index] > '9'))
            {
                PRINTF("the parameter is wrong\r\n");
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
            PRINTF("the parameter is wrong\r\n");
            break;
        }

        if (select_index == 0U)
        {
            PRINTF("the parameter is wrong\r\n");
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
        int err = 0;
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
    }

    return kStatus_SHELL_Success;
}

void app_shell_init(void)
{
    DbgConsole_Flush();
    /* Init SHELL */
    shell_handle = &shell_handle_buffer[0];
    if (kStatus_SHELL_Success != SHELL_Init(shell_handle, g_serialHandle, ""))
    {
        (void)PRINTF("Coex shell initialization failed!\r\n");
    }

#if(CONFIG_WIFI_BLE_COEX_APP)
    (void)PRINTF("WiFi shell initialization\r\n");
    SHELL_RegisterCommand(shell_handle, SHELL_COMMAND(wifi));
#endif

#if(CONFIG_DISABLE_BLE == 0)
    PRINTF("\r\n");
    (void)PRINTF("BLE shell initialization\r\n");
    SHELL_RegisterCommand(shell_handle, SHELL_COMMAND(bt));
#endif
    PRINTF("\r\n");
    SHELL_ChangePrompt(shell_handle, (char *)s_shellCoexPrompt);
}
