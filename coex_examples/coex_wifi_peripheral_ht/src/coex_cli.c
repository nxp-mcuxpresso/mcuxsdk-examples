/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

 ///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////
#include <string.h>

#include "fsl_debug_console.h"
#include "cli.h"
#include "coex_cli.h"
#include "wlan.h"

#define CMD_BUF_SIZE 512
char cmd_buf[CMD_BUF_SIZE];

void coex_menuPrint(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

static void printSeparator(void)
{
    PRINTF("========================================\r\n");
}

static struct cli_command built_ins[] = {
    {"help (exit: 0)", NULL, help_command},
};

int coex_cli_init()
{
    if (cli_register_commands(&built_ins[0], (int)(sizeof(built_ins) / sizeof(struct cli_command))) != 0)
    {
        return -1;
    }
    return 0;
}

static int coex_console_input(void)
{
    char a;
    int i;

retry:
    i = 0;
    PRINTF("\n>");

    while (1)
    {
        a = pollChar();
        if (a == '\n')
            continue;
        if (a == '\r')
        {
            cmd_buf[i]='\0';
            break;
        }

        cmd_buf[i++] = a;

        if (i >= CMD_BUF_SIZE)
        {
            PRINTF("Error: overflow!\r\n");
            goto retry;
        }
    }

    if (strcmp(cmd_buf, "0")==0)
    {
        coex_menuPrint();
        return -1;
    }
    if (strcmp(cmd_buf,"")==0)
    {
        goto retry;
    }

    return 0;
}

void execute_commnd(char *wlan_command,char** args,int j);
void wifi_cli(void)
{
    char* cmd_list[20] = {0};
    char *wlantemp = NULL;
    char *psave = NULL;
    char *delim = " ";
    int j=1;
    int ret = 0;

    PRINTF("Enter WiFi commands\n");

    while (1)
    {
        ret = coex_console_input();
        if (ret == -1)
        {
            return;
        }
        wlantemp=strtok_r(cmd_buf, delim, &psave);
        j = 1;
        cmd_list[0]=cmd_buf;
        while(1)
        {
            wlantemp=strtok_r(NULL, delim, &psave);
            if(wlantemp == NULL)
                break;
            cmd_list[j++]=wlantemp;
        }

        PRINTF("wifi commands: %s\n", cmd_buf);
        execute_commnd(cmd_buf, cmd_list, j);
    }
}

void execute_commnd(char *wlan_command,char** args,int j)
{
    osa_status_t status;
    (void)status;
    const struct cli_command *command = NULL;
    const char *p;
    int i=0;

    i = ((p = strchr(wlan_command, '.')) == NULL) ? 0 : (p - wlan_command);
    command = lookup_command(wlan_command, i);
    if (command == NULL)
        PRINTF("Invalid Command");
    else
        command->function(j, args);
}

/*!
 * @brief struct for coex menu.
 */
typedef struct
{
    char key;
    void (*func)(void);
    const char *text;
} menu_item_t;

menu_item_t menuItems[] = {
    {'0',coex_menuPrint, "Coex menu print"},
    {'w',wifi_cli, "Wifi Cli"},
    {0, NULL, NULL},
};

/*!
 * @brief Function to coex menu print.
 */
void coex_menuPrint(void)
{
    PRINTF(" Coex menu called\r\n");
    printSeparator();

    for (int i = 0; menuItems[i].text != NULL; i++)
    {
        if (menuItems[i].key)
            PRINTF("  %c  %s\r\n", menuItems[i].key, menuItems[i].text);
        else
            PRINTF("  %d  %s\r\n", i, menuItems[i].text);
    }

    printSeparator();
}

/*!
 * @brief Function to coex menu action.
 */
void coex_menuAction(int ch)
{
    if (ch == '\r' || ch == ' ' || ch == '\n')
    {
        printSeparator();
        return;
    }

    for (int i = 0; menuItems[i].func != NULL; i++)
    {
        if (menuItems[i].key == ch)
        {
            PRINTF("Key '%c': %s\r\n", ch, menuItems[i].text);
            menuItems[i].func();
            return;
        }
    }
    PRINTF("ERROR: No action bound to '%c'\r\n", ch);
}


/*!
 * @brief Function to Poll char.
 */
int pollChar(void)
{
    int tmp;
    tmp = GETCHAR();
    return tmp;
}

//*********************************************************************************************