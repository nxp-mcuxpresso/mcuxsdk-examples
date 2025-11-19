/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_MSG_H_
#define _APP_MSG_H_

#include <stdint.h>

#define NAME_SIZE       32

enum{
    /* Msg used by control task on core0 */
    CTRL_TASK_WIFI_INIT_DONE,
    CTRL_TASK_WIFI_CONNECTED,

    /* Simulate face recognized/unrecoginized event from MPP by Shell cmd */
    CTRL_TASK_SIMU_FACE_NOT_RECOGNIZED,
    CTRL_TASK_SIMU_FACE_RECOGNIZED,

    /* Presence detection or tracking events from UWB task to control task */
    CTRL_TASK_UWB_PRESENCE_DETECTED,

    /* multi core task. */
    CTRL_TASK_REMOTE_EP_READY, //5

    /* MPP task msg */
    MPP_TASK_CONFIG,
    MPP_TASK_FR_ENABLE_LCD_ON,
    MPP_TASK_FR_ENABLE_LCD_OFF,  //reserved for future face recognition LCD control
    MPP_TASK_FR_DISABLE_LCD_OFF,
    MPP_TASK_FR_DISABLE_LCD_ON,
    MPP_TASK_FACE_REG_REQ,  //userName and timeout, 10
    CTRL_TASK_FACE_DETECTED,
    CTRL_TASK_FACE_RECOGNIZED, //user name
    CTRL_TASK_FACE_NOT_RECOGNIZED,
    CTRL_TASK_FACE_REG_COMPLETE,
    CTRL_TASK_FACE_REG_TIMEOUT,   //14

    USB_HOST_VIDEO_TASK_CMD_LAST = 0xFF
};

typedef struct{
    uint16_t distance[4];
}presence_det_msg_payload_t;


typedef struct{
    char name[NAME_SIZE];
    uint32_t similarity;
}face_recognized_msg_payload_t;

typedef struct{
    char name[NAME_SIZE];
    uint32_t timeout;  //unit: seconds
}face_reg_req_msg_payload_t;

typedef struct{
    uint32_t msgID;
    union
    {
        /* data */
        presence_det_msg_payload_t pd_payload;
        face_recognized_msg_payload_t fr_payload;
        face_reg_req_msg_payload_t reg_payload;
    };
} msg_t;

#endif /* _APP_MSG_H_ */