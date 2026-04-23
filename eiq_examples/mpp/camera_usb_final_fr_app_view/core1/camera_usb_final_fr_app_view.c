/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rpmsg_lite.h"
#include "rpmsg_queue.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "task.h"
#include "app.h"

#include "hal_camera_shared.h"

#include "images/kaggle_img_rgb_320_yuyv_jpg.h"
#include "images/kaggle_img_ir_640_yuyv_jpg.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_TASK_STACK_SIZE (256U)
#define APP_RPMSG_READY_EVENT_DATA (1U)
#define FPS_COMPUTATION_INTERVAL   (5000)  // ms

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile virtual_usb_cam_msg_t msg = {0};
static volatile virtual_usb_cam_msg_t reply_msg = {0};
#ifdef RPMSG_LITE_MASTER_IS_LINUX
static char helloMsg[13];
#endif /* RPMSG_LITE_MASTER_IS_LINUX */

/* Static image parameters */
const uint8_t *rgb_image_data = &kaggle_img_rgb_data[0];
uint32_t rgb_image_size = sizeof(kaggle_img_rgb_data);
const uint8_t *ir_image_data = &kaggle_img_ir_data[0];
uint32_t ir_image_size = sizeof(kaggle_img_ir_data);

const uint8_t *crt_rgb_image_data;
uint32_t crt_rgb_image_size;
const uint8_t *crt_ir_image_data;
uint32_t crt_ir_image_size;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
static TaskHandle_t app_task_handle = NULL;

static struct rpmsg_lite_instance *volatile my_rpmsg = NULL;

static struct rpmsg_lite_endpoint *volatile my_ept = NULL;
static volatile rpmsg_queue_handle my_queue        = NULL;

static bool error_occurred = false;

void app_destroy_task(void)
{
    if (app_task_handle)
    {
        vTaskDelete(app_task_handle);
        app_task_handle = NULL;
    }

    if (my_ept)
    {
        rpmsg_lite_destroy_ept(my_rpmsg, my_ept);
        my_ept = NULL;
    }

    if (my_queue)
    {
        rpmsg_queue_destroy(my_rpmsg, my_queue);
        my_queue = NULL;
    }

    if (my_rpmsg)
    {
        rpmsg_lite_deinit(my_rpmsg);
        my_rpmsg = NULL;
    }
}

static void app_task(void *param)
{
    volatile uint32_t remote_addr      = 0U;

    virtual_usb_cam_config_msg_t cfg_msg;
    int32_t rpmsg_ret;

    /* Print the initial banner */
    (void)PRINTF("\r\nVirtual camera with static images demo...\r\n");

#ifdef MCMGR_USED
    uint32_t startupData;
    volatile mcmgr_status_t status;

    /* Get the startup data */
    do
    {
        status = MCMGR_GetStartupData(kMCMGR_Core0, &startupData);
    } while (status != kStatus_MCMGR_Success);

    my_rpmsg = rpmsg_lite_remote_init((void *)(char *)(platform_patova(startupData)), RPMSG_LITE_LINK_ID, RL_NO_FLAGS);
    if (my_rpmsg == NULL)
    {
        (void)PRINTF("Failed to initialize rpmsg...\r\n");
        error_occurred = true;
        goto cleanup;
    }

    /* Signal the other core we are ready by triggering the event and passing the APP_RPMSG_READY_EVENT_DATA */
    (void)MCMGR_TriggerEvent(kMCMGR_Core0, kMCMGR_RemoteApplicationEvent, APP_RPMSG_READY_EVENT_DATA);
#else
    (void)PRINTF("RPMSG Share Base Addr is 0x%x\r\n", RPMSG_LITE_SHMEM_BASE);
    my_rpmsg = rpmsg_lite_remote_init((void *)RPMSG_LITE_SHMEM_BASE, RPMSG_LITE_LINK_ID, RL_NO_FLAGS);
    if (my_rpmsg == NULL)
    {
        (void)PRINTF("Failed to initialize rpmsg...\r\n");
        error_occurred = true;
        goto cleanup;
    }
#endif /* MCMGR_USED */
    rpmsg_lite_wait_for_link_up(my_rpmsg, RL_BLOCK);
    (void)PRINTF("Link is up!\r\n");

    my_queue = rpmsg_queue_create(my_rpmsg);
    if (my_queue == NULL)
    {
        (void)PRINTF("Failed to create queue...\r\n");
        error_occurred = true;
        goto cleanup;
    }

    my_ept = rpmsg_lite_create_ept(my_rpmsg, CORE1_EPT_ADDRESS, rpmsg_queue_rx_cb, my_queue);
    if (my_ept == NULL)
    {
        (void)PRINTF("Failed to create endpoint...\r\n");
        error_occurred = true;
        goto cleanup;
    }

    (void)MCMGR_TriggerEvent(kMCMGR_Core0, kMCMGR_RemoteApplicationEvent, APP_EP_READY_EVENT_DATA);
    PRINTF("EP is ready, waiting for camera config...\r\n");

#ifdef RPMSG_LITE_MASTER_IS_LINUX
    /* Wait Hello handshake message from Remote Core. */
    (void)rpmsg_queue_recv(my_rpmsg, my_queue, (uint32_t *)&remote_addr, helloMsg, sizeof(helloMsg), ((void *)0),
                           RL_BLOCK);
#endif /* RPMSG_LITE_MASTER_IS_LINUX */

    /* Wait for camera config message from core 0 */
    rpmsg_ret = rpmsg_queue_recv(my_rpmsg,
                                 my_queue,
                                 (uint32_t *)&remote_addr,
                                 (char *)&msg,
                                 sizeof(virtual_usb_cam_msg_t),
                                 ((void *)0),
                                 RL_BLOCK);
    if (rpmsg_ret != RL_SUCCESS)
    {
        PRINTF("Got error %d while receiving USB camera config message from core 0\r\n", rpmsg_ret);
        goto cleanup;
    }

    if (msg.msg_type != VIRT_USB_CAM_CONFIG)
    {
        PRINTF("Got message %d while waiting for config message from core 0\r\n", msg.msg_type, rpmsg_ret);
        goto cleanup;
    }

    msg.msg_type = VIRT_USB_CAM_CONFIG_ACK;
    rpmsg_ret = rpmsg_lite_send(my_rpmsg, my_ept, remote_addr, (char *)&msg, sizeof(msg.msg_type), RL_BLOCK);

    if (rpmsg_ret != RL_SUCCESS)
    {
        PRINTF("Got error %d while trying to send CONFIG ACK message to core 0\r\n", rpmsg_ret);
        goto cleanup;
    }

    /* print config parameters */
    PRINTF("Received message %d from remote %d\r\n", msg.msg_type, (uint32_t) remote_addr);
    memcpy((void *)&cfg_msg, (void *)&msg.msg_payload.config, sizeof(virtual_usb_cam_config_msg_t));
    PRINTF("Camera parameters:\r\n");
    PRINTF("\tcamera_width = %d\r\n", cfg_msg.camera_width);
    PRINTF("\tcamera_height = %d\r\n", cfg_msg.camera_height);
    PRINTF("\tcolor_format = %d\r\n", cfg_msg.color_format);
    PRINTF("\tfps = %d\r\n", cfg_msg.fps);

    PRINTF("RGB image addr = 0x%x\r\n", rgb_image_data);
    PRINTF("RGB image size = 0x%x\r\n", rgb_image_size);
    PRINTF("IR image addr = 0x%x\r\n", ir_image_data);
    PRINTF("IR image size = 0x%x\r\n", ir_image_size);

    uint32_t n_rgb_cnt = 0;
    uint32_t n_ir_cnt = 0;
    uint32_t start_time = 0;
    uint32_t time;

    crt_rgb_image_data = rgb_image_data;
    crt_rgb_image_size = rgb_image_size;
    crt_ir_image_data = ir_image_data;
    crt_ir_image_size = ir_image_size;

    start_time = xTaskGetTickCount();

    /* Loop forever waiting for messages from core 0 */
    while (1)
    {
        memset((void *)&reply_msg, 0, sizeof(reply_msg));
        memset((void *)&msg, 0, sizeof(msg));

        /* Wait for camera config message from core 0 */
        rpmsg_ret = rpmsg_queue_recv(my_rpmsg,
                                     my_queue,
                                     (uint32_t *)&remote_addr,
                                     (char *)&msg,
                                     sizeof(virtual_usb_cam_msg_t),
                                     ((void *)0),
                                     RL_BLOCK);

        if (rpmsg_ret != RL_SUCCESS)
        {
            PRINTF("Got error %d while receiving USB camera config message from core 0\r\n", rpmsg_ret);
            goto cleanup;
        }

        if ((msg.msg_type != VIRT_USB_CAM_REQRGBIR) && (msg.msg_type != VIRT_USB_CAM_REQRGB) && (msg.msg_type != VIRT_USB_CAM_REQIR))
        {
            PRINTF("Got message %d while waiting for buff req message from core 0\r\n", msg.msg_type, rpmsg_ret);
            goto cleanup;
        }

        if (msg.user_id != MPP_USER_ID)
        {
            PRINTF("Got wrong user id %d \r\n", msg.user_id);
            goto cleanup;
        }

        /* Copy frame to buffer configured by core 0 */
        /* RGB will always be required */
        if (((msg.msg_type == VIRT_USB_CAM_REQRGBIR) || (msg.msg_type == VIRT_USB_CAM_REQRGB)) && (rgb_image_size > msg.msg_payload.req.rgb_max_frame_size))
        {
            PRINTF("Cannot copy RGB frame to destination buffer. It will not fit into dest buffer\r\n");
            PRINTF("RGB frame size: 0x%x\r\n", rgb_image_size);
            PRINTF("Buffer configured size: 0x%x\r\n", msg.msg_payload.req.rgb_max_frame_size);
            goto cleanup;
        }
        if (((msg.msg_type == VIRT_USB_CAM_REQRGBIR) || (msg.msg_type == VIRT_USB_CAM_REQIR)) && (ir_image_size > msg.msg_payload.req.ir_max_frame_size))
        {
            PRINTF("Cannot copy IR frame to destination buffer. It will not fit into dest buffer\r\n");
            PRINTF("IR frame size: 0x%x\r\n", ir_image_size);
            PRINTF("Buffer configured size: 0x%x\r\n", msg.msg_payload.req.ir_max_frame_size);
            goto cleanup;
        }
        if ((msg.msg_type == VIRT_USB_CAM_REQRGB) || (msg.msg_type == VIRT_USB_CAM_REQRGBIR))
        {
            reply_msg.msg_payload.rsp.rgb_frame_addr = msg.msg_payload.req.rgb_frame_addr;
            reply_msg.msg_payload.rsp.rgb_frame_size = crt_rgb_image_size;
            memcpy((void *)msg.msg_payload.req.rgb_frame_addr, crt_rgb_image_data, crt_rgb_image_size);
            n_rgb_cnt++;
        }
        if ((msg.msg_type == VIRT_USB_CAM_REQIR) || (msg.msg_type == VIRT_USB_CAM_REQRGBIR))
        {
            reply_msg.msg_payload.rsp.ir_frame_addr = msg.msg_payload.req.ir_frame_addr;
            reply_msg.msg_payload.rsp.ir_frame_size = crt_ir_image_size;
            memcpy((void *)msg.msg_payload.req.ir_frame_addr, crt_ir_image_data, crt_ir_image_size);
            n_ir_cnt++;
        }

        // PRINTF("Sending VIRT_USB_CAM_RSPRGBIR message to remote %d \r\n", (uint32_t) remote_addr);
        if (msg.msg_type == VIRT_USB_CAM_REQRGB)
            reply_msg.msg_type = VIRT_USB_CAM_RSPRGB;
        else if (msg.msg_type == VIRT_USB_CAM_REQIR)
            reply_msg.msg_type = VIRT_USB_CAM_RSPIR;
        else
            reply_msg.msg_type = VIRT_USB_CAM_RSPRGBIR;

        rpmsg_ret = rpmsg_lite_send(my_rpmsg, my_ept, remote_addr, (char *)&reply_msg, sizeof(virtual_usb_cam_msg_t), RL_BLOCK);

        if (rpmsg_ret != RL_SUCCESS)
        {
            PRINTF("Got error %d while trying to send msg to core 0\r\n", rpmsg_ret);
            goto cleanup;
        }

        /* Compute frame rates */
        if (((xTaskGetTickCount() - start_time) * portTICK_PERIOD_MS) > FPS_COMPUTATION_INTERVAL)
        {
            time = xTaskGetTickCount() - start_time;

            PRINTF("Frame Update Rate RGB: %d fps\r\n", n_rgb_cnt * 1000 / (time * portTICK_PERIOD_MS));
            PRINTF("Frame Update Rate IR: %d fps\r\n", n_ir_cnt * 1000 / (time * portTICK_PERIOD_MS));
            n_rgb_cnt = 0;
            n_ir_cnt = 0;

            start_time = xTaskGetTickCount();
        }
    }

cleanup:
    (void)PRINTF("APP done, deinitializing...\r\n");

    if (my_ept)
    {
        (void)rpmsg_lite_destroy_ept(my_rpmsg, my_ept);
        my_ept = ((void *)0);
    }

    if (my_queue)
    {
        (void)rpmsg_queue_destroy(my_rpmsg, my_queue);
        my_queue = ((void *)0);
    }

    if (my_rpmsg)
    {
        (void)rpmsg_lite_deinit(my_rpmsg);
        my_rpmsg = ((void *)0);
    }

    if (error_occurred)
    {
        (void)PRINTF("Error occurred, looping forever...\r\n");
    }
    else
    {
        (void)PRINTF("Looping forever...\r\n");
    }

    /* End of the example */
    for (;;)
    {
    }
}

void app_create_task(void)
{
    if (app_task_handle == NULL &&
        xTaskCreate(app_task, "APP_TASK", APP_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &app_task_handle) != pdPASS)
    {
        PRINTF("\r\nFailed to create application task\r\n");
        for (;;)
            ;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize standard SDK demo application pins */
    BOARD_InitHardware();

#ifdef MCMGR_USED
    /* Initialize MCMGR before calling its API */
    (void)MCMGR_Init();
#endif /* MCMGR_USED */

    app_create_task();
    vTaskStartScheduler();

    (void)PRINTF("Failed to start FreeRTOS on core0.\r\n");
    for (;;)
    {
    }
}
