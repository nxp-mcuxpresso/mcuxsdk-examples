/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include <stdio.h>
/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_shell.h"

#ifndef SHELL_ADVANCE
#error Only support Shell advance mode
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static shell_status_t SHELL_BrDiscoverCommand(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t SHELL_LeAddPrimarySeriviceCommand(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t SHELL_LeDiscoverCommand(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t SHELL_ServiceEntry(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t SHELL_InitCommand(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t SHELL_DeinitCommand(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t SHELL_BrEntry(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t SHELL_LeEntry(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t SHELL_BtEntry(shell_handle_t shellHandle, int32_t argc, char **argv);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static SHELL_HANDLE_DEFINE(s_shellTest);

static shell_command_t *s_commandBrSet[] = {
    SHELL_COMMAND_EXDEFINE_ANONYMOUS(
        discover,
        NULL,
        "\"discover <value: on, off> [length: 1-48] [mode: limited]\": Start/stop BR Discover\r\n",
        SHELL_BrDiscoverCommand,
        1,
        2),
    NULL};

static shell_command_t *s_commandLeSeviceSet[] = {
    SHELL_COMMAND_EXDEFINE_ANONYMOUS(add - primary,
                                     NULL,
                                     "\"add-primary <UUID>\": Add primary service\r\n",
                                     SHELL_LeAddPrimarySeriviceCommand,
                                     1,
                                     0),
    NULL};

static shell_command_t *s_commandLeSet[] = {
    SHELL_COMMAND_EXDEFINE_ANONYMOUS(
        discover,
        NULL,
        "\"discover [UUID] [start handle] [end handle]\": initialize the stack with the instance\r\n",
        SHELL_LeDiscoverCommand,
        0,
        3),
    SHELL_COMMAND_EXDEFINE_ANONYMOUS(
        service, s_commandLeSeviceSet, "\"service\": sub-command of serivce\r\n", SHELL_ServiceEntry, 0, 0),
    NULL};

static shell_command_t *s_commandBtSet[] = {
    SHELL_COMMAND_EXDEFINE_ANONYMOUS(
        init, NULL, "\"init <instance>\": initialize the stack with the instance\r\n", SHELL_InitCommand, 1, 0),
    SHELL_COMMAND_EXDEFINE_ANONYMOUS(deinit, NULL, "\"deinit\": deinit the stack\r\n", SHELL_DeinitCommand, 0, 0),
    SHELL_COMMAND_EXDEFINE_ANONYMOUS(br, s_commandBrSet, "\"br\": sub-command of br\r\n", SHELL_BrEntry, 0, 0),
    SHELL_COMMAND_EXDEFINE_ANONYMOUS(le, s_commandLeSet, "\"le\": sub-command of LE\r\n", SHELL_LeEntry, 0, 0), NULL};

SHELL_COMMAND_EXDEFINE(bt, s_commandBtSet, "\"bt\": sub-command of Bluetooth\r\n", SHELL_BtEntry, 0, 0);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
void main_task(void *pvParameters)
{
    shell_status_t shellRet;

    shellRet = SHELL_Init((shell_handle_t)s_shellTest, (serial_handle_t)g_serialHandle, "Test@SHELL>");
    assert(kStatus_SHELL_Success == shellRet);

    shellRet = SHELL_RegisterCommand((shell_handle_t)s_shellTest, SHELL_COMMAND(bt));
    assert(kStatus_SHELL_Success == shellRet);

    for (;;)
    {
        OSA_TimeDelay(1000);
    }
}

static shell_status_t SHELL_BrDiscoverCommand(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    SHELL_Printf((shell_handle_t)s_shellTest, "%s\r\n", argv[0]);
    return kStatus_SHELL_Success;
}

static shell_status_t SHELL_LeAddPrimarySeriviceCommand(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    SHELL_Printf((shell_handle_t)s_shellTest, "%s\r\n", argv[0]);
    return kStatus_SHELL_Success;
}

static shell_status_t SHELL_LeDiscoverCommand(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    SHELL_Printf((shell_handle_t)s_shellTest, "%s\r\n", argv[0]);
    return kStatus_SHELL_Success;
}

static shell_status_t SHELL_ServiceEntry(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    return kStatus_SHELL_PrintCmdHelp;
}

static shell_status_t SHELL_InitCommand(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    SHELL_Printf((shell_handle_t)s_shellTest, "%s\r\n", argv[0]);
    return kStatus_SHELL_Success;
}

static shell_status_t SHELL_DeinitCommand(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    SHELL_Printf((shell_handle_t)s_shellTest, "%s\r\n", argv[0]);
    return kStatus_SHELL_Success;
}

static shell_status_t SHELL_BrEntry(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    return kStatus_SHELL_PrintCmdHelp;
}

static shell_status_t SHELL_LeEntry(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    return kStatus_SHELL_PrintCmdHelp;
}

static shell_status_t SHELL_BtEntry(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    return kStatus_SHELL_PrintCmdHelp;
}
