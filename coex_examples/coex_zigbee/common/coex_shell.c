/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <string.h>
#include <porting.h>

#include "fsl_debug_console.h"
#include "shell_bt.h"
#include "fsl_shell.h"
#include "coex_shell.h"
#if (CONFIG_COEX_ENABLE_WIFI)
#include "cli.h"
#endif
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
SDK_ALIGN(static uint8_t shell_handle_buffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t shell_handle;

static char s_shellCoexPrompt[32] = "@coex> ";

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
extern uint8_t process_cmd_zigbee(int32_t argc, char **argv);
extern void help_cmd_zigbee(int32_t argc, char **argv);

/*******************************************************************************
 * Code
 ******************************************************************************/

static void printSeparator(void)
{
    PRINTF("========================================\r\n");
}

/*!
 * @brief Function to start wifi menu.
 */
#if(CONFIG_COEX_ENABLE_WIFI)
#define WIFI_PREFIX "wifi."

static void process_cmd_wifi(int32_t argc, char **argv)
{
    const struct cli_command *command = NULL;
    char *wifi_cmd = NULL;

    wifi_cmd = argv[0] + strlen(WIFI_PREFIX);
    strcpy(argv[0], wifi_cmd);

    command = lookup_command(argv[0], strlen(argv[0]));
    if (command != NULL)
    {
        command->function(argc, argv);
        (void)PRINTF("Command %s\r\n", command->name);
    }
    else
    {
        (void)PRINTF("Unknown comamnd %s\r\n", argv[0]);
    }
}

static int cmd_wifi(const struct shell *sh, size_t argc, char **argv)
{
    if (!strcmp(argv[0], "wifi.help")) {
        shell_help(sh);
        help_command(argc, argv);
        return SHELL_CMD_HELP_PRINTED;
    }

    process_cmd_wifi(argc, argv);
    return 0;
}

SHELL_CMD_ARG_REGISTER(wifi, NULL, "WiFi shell commands", cmd_wifi, 1, SHELL_MAX_ARGS);

static void wifi_CommandInit(shell_handle_t shell)
{
    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommandwifi))
    {
        (void)PRINTF(shell, "Shell register command %s failed!", g_shellCommandwifi.pcCommand);
    }
}
#endif

/*!
 * @brief Function to start zigbee menu.
 */
#if (CONFIG_COEX_ENABLE_ZIGBEE)
#define ZIGBEE_CMD_PREFIX "zigbee."

static int cmd_zigbee(const struct shell *sh, size_t argc, char **argv)
{
    int ret = kStatus_SHELL_Success;

#if (defined(SHELL_ADVANCE) && (SHELL_ADVANCE > 0))
    /* A workaround for fsl_shell.c */
     memcpy(argv[0], ZIGBEE_CMD_PREFIX, strlen(ZIGBEE_CMD_PREFIX));
#endif /* SHELL_ADVANCE */

    argv[0] = argv[0] + strlen(ZIGBEE_CMD_PREFIX);

    if (!strcmp(argv[0], "help")) {
        help_cmd_zigbee(argc, argv);
        ret = kStatus_SHELL_PrintCmdHelp;
    }
    else
    {
        if(process_cmd_zigbee(argc, argv))
        {
            (void)PRINTF("Command size overflow!\r\n");
            ret = kStatus_SHELL_Error;
        }
    }

    return ret;
}

SHELL_CMD_ARG_REGISTER(zigbee, NULL, "Zigbee shell commands", cmd_zigbee, 1, SHELL_MAX_ARGS);

static void zigbee_CommandInit(shell_handle_t shell)
{
    if ((shell_status_t)kStatus_Success != SHELL_RegisterCommand(shell, &g_shellCommandzigbee))
    {
        (void)PRINTF(shell, "Shell register command %s failed!", g_shellCommandzigbee.pcCommand);
    }
}
#endif

int coex_cli_init(void)
{
    shell_handle = (shell_handle_t)&shell_handle_buffer[0];
    if (kStatus_SHELL_Success != SHELL_Init(shell_handle, g_serialHandle, ""))
    {
        (void)PRINTF("Coex shell initialization failed!\r\n");
        return -1;
    }

#if(CONFIG_COEX_ENABLE_WIFI)
    printSeparator();
    (void)PRINTF("WiFi shell initialization\r\n");
    wifi_CommandInit(shell_handle);
#endif

#if(CONFIG_COEX_ENABLE_ZIGBEE)
    printSeparator();
    (void)PRINTF("Zigbee shell initialization\r\n");
    zigbee_CommandInit(shell_handle);
#endif

#if (CONFIG_COEX_ENABLE_BLE)
    printSeparator();
    (void)PRINTF("BLE shell initialization\r\n");
    bt_CommandInit(shell_handle);
#endif
    printSeparator();

    SHELL_ChangePrompt(shell_handle, (char *)s_shellCoexPrompt);
    return 0;
}
