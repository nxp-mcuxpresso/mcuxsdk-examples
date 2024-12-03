/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_component_led.h"
#include "fsl_component_serial_manager.h"
#include "fsl_shell.h"
#if (defined(BUTTON_COUNT) && (BUTTON_COUNT > 0U))
#include "fsl_component_button.h"
#endif
#include "fsl_component_timer_manager.h"

#include "app.h"

#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define SHELL_APP_BUFFER_LENGTH (64U)

#define SHELL_APP_WRITE_TSET_HANDLE_COUNT (10U)

#define SHELL_APP_TEST_MEMORY_SIZE (256U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static shell_status_t SHELL_LedCommand(shell_handle_t shellHandle, int32_t argc, char **argv);

/*******************************************************************************
 * Variables
 ******************************************************************************/

extern serial_handle_t g_serialHandle;

SDK_ALIGN(static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t s_shellHandle;

SHELL_COMAMND_DEFINE(led,
                     "\r\n\"led arg1\": Set the LED status\r\n"
                     "  Usage:\r\n    arg1: on/off\r\n",
                     SHELL_LedCommand,
                     1);

extern led_config_t g_ledMonochrome[LED_TYPE_MONOCHROME_COUNT];
SDK_ALIGN(static uint8_t s_ledMonochromeHandleBuffer[LED_TYPE_MONOCHROME_COUNT][LED_HANDLE_SIZE], 4);
static led_handle_t s_ledMonochromeHandle[LED_TYPE_MONOCHROME_COUNT];

#if (defined(BUTTON_COUNT) && (BUTTON_COUNT > 0U))
extern button_config_t g_buttonConfig[BUTTON_COUNT];
SDK_ALIGN(uint8_t gpioSwBuffer[BUTTON_COUNT][BUTTON_HANDLE_SIZE], 4);
static button_handle_t buttonHandle[BUTTON_COUNT];
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

static shell_status_t SHELL_LedCommand(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    if (0 == strcmp(argv[1], "on"))
    {
        LED_TurnOnOff(s_ledMonochromeHandle[0], 1);
    }
    else if (0 == strcmp(argv[1], "off"))
    {
        LED_TurnOnOff(s_ledMonochromeHandle[0], 0);
    }
    else
    {
        SHELL_Printf(shellHandle, "Invalid command!\r\n");
    }
    return kStatus_SHELL_Success;
}

#if (defined(BUTTON_COUNT) && (BUTTON_COUNT > 0U))
button_status_t button_callback(void *buttonHandle, button_callback_message_t *message, void *callbackParam)
{
    button_status_t status = kStatus_BUTTON_Success;

    switch (message->event)
    {
        case kBUTTON_EventOneClick:
            PRINTF("kBUTTON_EventOneClick\r\n");
            LED_TurnOnOff(s_ledMonochromeHandle[0], 0);
            break;
        case kBUTTON_EventShortPress:
            PRINTF("kBUTTON_EventShortPress\r\n");
            LED_TurnOnOff(s_ledMonochromeHandle[0], 0);
            break;
        case kBUTTON_EventDoubleClick:
            PRINTF("kBUTTON_EventDoubleClick\r\n");
            LED_TurnOnOff(s_ledMonochromeHandle[0], 1);
            break;
        case kBUTTON_EventLongPress:
            PRINTF("kBUTTON_EventLongPress\r\n");
            LED_TurnOnOff(s_ledMonochromeHandle[0], 1);
            break;
        case kBUTTON_EventError:
            PRINTF("kBUTTON_EventError\r\n");
            break;
        default:
            status = kStatus_BUTTON_Error;
            break;
    }

    return status;
}
#endif

/*! @brief Main function */
int main(void)
{
    PRINTF("The demo is used to demonstrate how to use new components.\r\n");
    PRINTF("The main function of the demo is to control the led by using the shell or button.\r\n");
    PRINTF(
        "For shell, please enter \"help\" to get the help information firstly. Turn on LED by"
        " using command \"led on\". And turn off LED by using command \"led off\".\r\n");
#if (defined(BUTTON_COUNT) && (BUTTON_COUNT > 0U))
    PRINTF(
        "For button, please press the button %s to control LED. Turn on LED when the"
        " button is pressed with long press or double click event. And turn off LED"
        " when the button is pressed with short press or one click event.\r\n",
        BUTTON_NAME);
#endif

    s_shellHandle = (shell_handle_t)&s_shellHandleBuffer[0];
    if (kStatus_SHELL_Success != SHELL_Init(s_shellHandle, g_serialHandle, "Test@SHELL>"))
    {
        PRINTF("Shell initialization failed!\r\n");
    }

    if (kStatus_SHELL_Success != SHELL_RegisterCommand(s_shellHandle, SHELL_COMAMND(led)))
    {
        PRINTF("Shell register command failed!\r\n");
    }

    for (int i = 0; i < LED_TYPE_MONOCHROME_COUNT; i++)
    {
        s_ledMonochromeHandle[i] = &s_ledMonochromeHandleBuffer[i][0];
        if (kStatus_LED_Success != LED_Init(s_ledMonochromeHandle[i], &g_ledMonochrome[0]))
        {
            PRINTF("LED %d initialization failed\r\n", i);
        }
    }

#if (defined(BUTTON_COUNT) && (BUTTON_COUNT > 0U))
    for (int i = 0; i < BUTTON_COUNT; i++)
    {
        buttonHandle[i] = (button_handle_t)&gpioSwBuffer[i][0];
        BUTTON_Init(buttonHandle[i], &g_buttonConfig[i]);
        BUTTON_InstallCallback(buttonHandle[i], button_callback, NULL);
    }
#endif

    while (1)
    {
#if !(defined(SHELL_NON_BLOCKING_MODE) && (SHELL_NON_BLOCKING_MODE > 0U))
        SHELL_Task(s_shellHandle);
#endif
    }
}
