/*
 * Copyright 2025-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "atomic.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "string.h"
#include "stdbool.h"
#include "stdio.h"

/* NXP includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"

/* Shell includes */
#include "fsl_shell.h"

/* RPMSG includes */
#include "rpmsg_lite.h"

/* MPP App includes */
#include "mpp_api.h"
#include "mpp_app_task_notify.h"
#include "app_msg.h"
#include "app_types.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CONTROL_RECV_QUEUE_SIZE     8

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
shell_status_t start(shell_handle_t shellHandle, int32_t argc, char **argv);
shell_status_t stop(shell_handle_t shellHandle, int32_t argc, char **argv);
shell_status_t register_person(shell_handle_t shellHandle, int32_t argc, char **argv);

static void ctrl_task(void *params);
extern void mpp_app_start(mpp_app_params_t *params);
void ctrl_task_notify(uint8_t cmd, msg_t* para);

/*******************************************************************************
 * Variables declaration
 ******************************************************************************/
/* shell & serial handlers */
static shell_handle_t s_shellHandle;
extern serial_handle_t g_serialHandle;

static TaskHandle_t ctrl_task_handle = NULL;
QueueHandle_t ctrl_queue_recv = NULL;

static uint32_t crt_mpp_state = 0;

/* shell functions */
/* The handle should be 4 byte aligned, because unaligned access isn't be supported on some devices.*/
SDK_ALIGN(static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE], 4);

/* define shell commands */
SHELL_COMMAND_DEFINE(start,
        "\r\n\"start\": start <target>\r\n"
        "    target - Choose from [display, fr]\r\n"
        "        - if target = display, start only the display\r\n"
        "        - if target = fr, start face recognition and display\r\n",
        start,
        1);
SHELL_COMMAND_DEFINE(stop,
         "\r\n\"stop\": stop <target>\r\n"
         "    target - Choose from [display, fr]\r\n"
         "        - if target = display, both display and face detection are stopped\r\n"
         "        - if target = fr, stop face recognition\r\n"
         "             (display will still remain active)\r\n",
        stop,
        1);
SHELL_COMMAND_DEFINE(add,
        "\r\n\"add\": add <name> <timeout>\r\n"
        "    name - name of person you want to register\r\n"
        "    timeout - timeout in seconds for registration process\r\n",
        register_person,
        2);

/*******************************************************************************
 * Code
 ******************************************************************************/
void ctrl_task_notify(uint8_t cmd, msg_t* para)
{
    if (para == NULL) {
        para = (msg_t*)pvPortMalloc(sizeof(msg_t));
        para->msgID = cmd;
    }else{
        para->msgID = cmd;
    }

    if (ctrl_queue_recv == NULL)
    {
        PRINTF("Error: Control task queue not initialized\r\n");
        vPortFree(para);
        return;
    }

    if (__get_IPSR() == 0)
    {
        xQueueSend( ctrl_queue_recv, &para, portMAX_DELAY );
    }
    else
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xQueueSendFromISR( ctrl_queue_recv, &para, &xHigherPriorityTaskWoken );
        if (xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
        }
    }
}

shell_status_t start(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    if (argc > 0)
    {
        if (argv[1] != NULL)
        {
            // Check the target argument
            if (strcmp(argv[1], "display") == 0)
            {
                if ((crt_mpp_state == MPP_TASK_FR_ENABLE_LCD_ON) || (crt_mpp_state == MPP_TASK_FR_DISABLE_LCD_ON))
                {
                    PRINTF("Display is already running.\r\n");
                    return kStatus_SHELL_Error;
                }
                crt_mpp_state = MPP_TASK_FR_DISABLE_LCD_ON;
                mpp_task_notify(MPP_TASK_FR_DISABLE_LCD_ON, NULL);
            }
            else if (strcmp(argv[1], "fr") == 0)
            {
                if (crt_mpp_state == MPP_TASK_FR_ENABLE_LCD_ON)
                {
                    PRINTF("Face recognition is already running.\r\n");
                    return kStatus_SHELL_Error;
                }
                crt_mpp_state = MPP_TASK_FR_ENABLE_LCD_ON;
                mpp_task_notify(MPP_TASK_FR_ENABLE_LCD_ON, NULL);
            }
            else
            {
                PRINTF("Invalid target. Choose from [display, fr]\r\n");
                return kStatus_SHELL_Error;
            }
        }
        else
        {
            // If no specific target, start both display and face recognition
            PRINTF("Bad parameter combination for command start: argc=%d, argv[1]=NULL\r\n", argc);
            return kStatus_SHELL_Error;
        }
    }
    else
    {
        PRINTF("Target parameter not provided. Run help to see the list of arguments\r\n", argc);
        return kStatus_SHELL_Error;
    }

    return kStatus_SHELL_Success;
}

shell_status_t stop(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    if (argc > 0)
    {
        if (argv[1] != NULL)
        {
            // Check the target argument
            if (strcmp(argv[1], "fr") == 0)
            {
                if ((crt_mpp_state == MPP_TASK_FR_DISABLE_LCD_ON) || (crt_mpp_state == MPP_TASK_FR_DISABLE_LCD_OFF))
                {
                    PRINTF("Face recognition is already stopped.\r\n");
                    return kStatus_SHELL_Error;
                }
                crt_mpp_state = MPP_TASK_FR_DISABLE_LCD_ON;
                mpp_task_notify(MPP_TASK_FR_DISABLE_LCD_ON, NULL);
            }
            else if (strcmp(argv[1], "display") == 0)
            {
                if (crt_mpp_state == MPP_TASK_FR_DISABLE_LCD_OFF)
                {
                    PRINTF("Both display and face recognition are already stopped.\r\n");
                    return kStatus_SHELL_Error;
                }
                crt_mpp_state = MPP_TASK_FR_DISABLE_LCD_OFF;
                mpp_task_notify(MPP_TASK_FR_DISABLE_LCD_OFF, NULL);
            }
            else
            {
                PRINTF("Invalid target. Choose from [display, fr]\r\n");
                return kStatus_SHELL_Error;
            }
        }
        else
        {
            // If no specific target, start both display and face recognition
            PRINTF("Bad parameter combination for command stop: argc=%d, argv[1]=NULL\r\n", argc);
            return kStatus_SHELL_Error;
        }
    }
    else
    {
        PRINTF("Target parameter not provided. Run help to see the list of arguments\r\n", argc);
        return kStatus_SHELL_Error;
    }

    return kStatus_SHELL_Success;
}

shell_status_t register_person(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    if (argc > 1)
    {
        if (argv[1] != NULL)
        {
            PRINTF("Person name: %s\r\n", argv[1]);
        }
        else
        {
            PRINTF("argv[1] is null\r\n");
            return kStatus_SHELL_Error;
        }
        if (argv[2] != NULL)
        {
            PRINTF("Timeout: %s seconds\r\n", argv[2]);
        }
        else
        {
            PRINTF("argv[2] is null\r\n");
            return kStatus_SHELL_Error;
        }
    }
    else
    {
        PRINTF("Please provide a name and timeout for registration.\r\n");
        return kStatus_SHELL_Error;
    }

    char *name = argv[1];
    char *timeout = argv[2];
    char *endptr;
    int num = (int) strtol(timeout, &endptr, 10);

    if (*endptr != '\0')
    {
        PRINTF("Provided timeout value is not a valid number\r\n");
        return kStatus_SHELL_Error;
    }

    msg_t *cmd = (msg_t *) pvPortMalloc(sizeof(msg_t));
    strncpy(cmd->reg_payload.name, name, NAME_SIZE - 1);
    cmd->reg_payload.timeout = num; // (num is provided in seconds)

    mpp_task_notify(MPP_TASK_FACE_REG_REQ, cmd);
    crt_mpp_state = MPP_TASK_FR_ENABLE_LCD_ON;

    return kStatus_SHELL_Success;
}

 /*!
 * @brief App control task
 */

static void ctrl_task(void *params)
{
    (void) params;
    msg_t *recv_cmd;
    BaseType_t ret = pdFALSE;
    mpp_app_params_t mpp_params = {0};

    /* Boot secondary core */
    volatile uint16_t *mcmgr_event_data_p = mpp_boot_secondary_core();
    PRINTF("Secondary core is up\r\n");

    /* Initialize RPMSG module */
    struct rpmsg_lite_instance *rpmsg_inst = (struct rpmsg_lite_instance *) mpp_init_rpmsg();
    if (rpmsg_inst == NULL) {
        PRINTF("RPMSG init failed\r\n");
        goto err;
    }
    PRINTF("RPMsg intialized\r\n");

    ctrl_queue_recv = xQueueCreate(CONTROL_RECV_QUEUE_SIZE, sizeof(msg_t *));
    if (ctrl_queue_recv == NULL) {
        PRINTF("Failed to create control queue\r\n");
        goto err;
    }
    PRINTF("Created control receive queue\r\n");

        /* Init SHELL */
    s_shellHandle = (shell_handle_t)&s_shellHandleBuffer[0];

    char *propmt_text = "face_reco_control >";

    if (kStatus_SHELL_Success != SHELL_Init(s_shellHandle, g_serialHandle, propmt_text))
    {
        PRINTF("Shell initialization failed!\r\n");
        goto err;
    }

    /* Add new command to commands list */
    /* Definition of shell commands */
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(start));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(stop));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(add));

    /* Start MPP task */
    mpp_params.mcmgr_event_data_p = mcmgr_event_data_p;
    mpp_params.rpmsg_inst_p = rpmsg_inst;
    mpp_params.recognition_threshold = 0.0f;
    mpp_app_start(&mpp_params);

    for (;;)
    {
        ret = xQueueReceive(ctrl_queue_recv, &recv_cmd, portMAX_DELAY );
        if (ret != pdPASS )
        {
            PRINTF("Got error %d while receiving message from queue\r\n", ret);
            continue;
        }

        switch (recv_cmd->msgID)
        {
            case CTRL_TASK_FACE_DETECTED:
                /* PRINTF("Face detected, processing...\n\r"); */
                break;

            case CTRL_TASK_FACE_RECOGNIZED:
                PRINTF("[CTRL] Recognized %s, similarity %d\r\n", recv_cmd->fr_payload.name, recv_cmd->fr_payload.similarity);
                break;
            
            case CTRL_TASK_FACE_NOT_RECOGNIZED:
                 /* PRINTF("Face not recognized \r\nPlease use command add <name> to register new face\r\n"); */
                break;

            case CTRL_TASK_FACE_REG_COMPLETE:
                crt_mpp_state = MPP_TASK_FR_DISABLE_LCD_ON;
                PRINTF("[CTRL] Face registered \n\r");
                break;

            case CTRL_TASK_FACE_REG_TIMEOUT:
                crt_mpp_state = MPP_TASK_FR_DISABLE_LCD_ON;
            	PRINTF("[CTRL] Face registration timeout \n\r");
            	break;

            default:
                PRINTF("[CTRL] Unknown command received (%d)\r\n", recv_cmd->msgID);
                break;
        }

        if (recv_cmd != NULL)
        {
            vPortFree(recv_cmd);
        }
    }

err:
    for (;;) {
        PRINTF("Error running control task : ret %d\r\n", ret);
        vTaskSuspend(NULL);
    }
}

 /*!
 * @brief Application entry point.
 */

int main(void)
{
    BaseType_t ret = pdFAIL;

    /* Init board hardware. */
    BOARD_Init();

    ret = xTaskCreate(
        ctrl_task,
        "ctrl_task",
        configMINIMAL_STACK_SIZE + 128,
        NULL,
        2,
        &ctrl_task_handle);

    if (ret != pdPASS)
    {
        PRINTF("Failed to create control task\r\n");
    }

    /* start scheduler */
    vTaskStartScheduler();
    for (;;)
        vTaskSuspend(NULL);

    return 0;
}