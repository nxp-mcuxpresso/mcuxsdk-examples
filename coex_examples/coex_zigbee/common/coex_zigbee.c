/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <FreeRTOS.h>
#include "fsl_os_abstraction.h"

#include "zigbee_config.h"
#include "app_serial_commands.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define ZIGBEE_TASK_STACK_SIZE    4096
#define ZIGBEE_TASK_PRIORITY      PRIORITY_RTOS_TO_OSA(1)
#define ZIGBEE_COMMAND_BUF_SIZE   80 // same with the definition in app_serial_cammadns.c

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
extern void zigbee_main();
extern void vAppMain(void);
extern void vProcessCommand(char *tmp);
extern void vPrintUnkownCommand(char *token);

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
char zigbee_cmd[ZIGBEE_COMMAND_BUF_SIZE];
static OSA_TASK_DEFINE(zigbee_main, ZIGBEE_TASK_PRIORITY, 1, ZIGBEE_TASK_STACK_SIZE, 0);
OSA_TASK_HANDLE_DEFINE(zigbee_task_handle);

/****************************************************************************
 *
 * NAME: process_cmd_zigbee
 *
 * DESCRIPTION:
 *  Zigbee command process
 *
 * RETURNS:
 * uint8_t: 0 - success, 1 - command size overflow
 *
 ****************************************************************************/
uint8_t process_cmd_zigbee(int32_t argc, char **argv)
{
    uint8_t ret = 0;
    uint8_t zigbee_cmd_len = 0;

    memset(zigbee_cmd, 0, sizeof(char)*ZIGBEE_COMMAND_BUF_SIZE);

    strcpy(zigbee_cmd, argv[0]);
    zigbee_cmd_len += strlen(argv[0]);

    for (uint8_t i = 1; i < argc; i ++)
    {
        zigbee_cmd_len += (strlen(argv[i]) + 1);
        if (zigbee_cmd_len >= ZIGBEE_COMMAND_BUF_SIZE)
        {
            ret = 1; // command size OverFlow
            break;
        }
        strcat(zigbee_cmd, " ");
        strcat(zigbee_cmd, argv[i]);
    }

    vProcessCommand(zigbee_cmd);

    return ret;
}

/****************************************************************************
 *
 * NAME: help_cmd_zigbee
 *
 * DESCRIPTION:
 *  print list of available commands
 *
 * RETURNS:
 * Never
 *
 ****************************************************************************/
void help_cmd_zigbee(int32_t argc, char **argv)
{
    vPrintUnkownCommand("");
}

/****************************************************************************
 *
 * NAME: zigbee_start
 *
 * DESCRIPTION:
 *  Zigbee main function
 *
 * RETURNS:
 * VOID
 *
 ****************************************************************************/
void zigbee_init(void)
{
    (void)OSA_TaskCreate((osa_task_handle_t)zigbee_task_handle, OSA_TASK(zigbee_main), NULL);
}
