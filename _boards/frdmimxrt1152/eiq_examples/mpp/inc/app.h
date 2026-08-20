/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#ifdef USE_USB_CAMERA
#include <stdint.h>
#include "usb_host_config.h"
#if ((defined USB_HOST_CONFIG_KHCI) && (USB_HOST_CONFIG_KHCI))
#ifndef CONTROLLER_ID
#define CONTROLLER_ID kUSB_ControllerKhci0
#endif
#endif
#if ((defined USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI))
#ifndef CONTROLLER_ID
#define CONTROLLER_ID kUSB_ControllerEhci0
#endif
#endif
#if ((defined USB_HOST_CONFIG_OHCI) && (USB_HOST_CONFIG_OHCI))
#ifndef CONTROLLER_ID
#define CONTROLLER_ID kUSB_ControllerOhci0
#endif
#endif
#if ((defined USB_HOST_CONFIG_IP3516HS) && (USB_HOST_CONFIG_IP3516HS))
#ifndef CONTROLLER_ID
#define CONTROLLER_ID kUSB_ControllerIp3516Hs0
#endif
#endif
#ifndef USB_HOST_INTERRUPT_PRIORITY
#define USB_HOST_INTERRUPT_PRIORITY (3U)
#endif

/*! @brief USB device attach/detach status */
typedef enum _usb_host_app_state
{
    kStatus_DEV_Idle = 0, /*!< there is no device attach/detach */
    kStatus_DEV_Attached, /*!< device is attached */
    kStatus_DEV_Detached, /*!< device is detached */
} usb_host_app_state_t;

enum {
    USB_CAMERA_FRAME_READY,
    USB_CAMERA_FRAME_DONE,
    USB_CAMERA_LAST = 0xFF
};

typedef struct usb_camera_msg_ {
    uint8_t cmd;
    uint8_t reserved[3];
    void *parameter;
    uint32_t size;
} usb_camera_msg_t;

#define NUM_FRAMES_TO_PROCESS -1

#endif /* USE_USB_CAMERA */

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_Init(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
