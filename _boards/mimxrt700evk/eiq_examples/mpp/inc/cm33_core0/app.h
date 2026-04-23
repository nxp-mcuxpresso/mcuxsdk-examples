/*
 * Copyright 2024-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __APP_H__
#define __APP_H__

#include "fsl_device_registers.h"

#ifdef USE_USB_CAMERA
#include "usb_host_config.h"
#include "usb_host.h"
#endif

#ifdef MCMGR_USED
#include "mcmgr.h"
#endif /* MCMGR_USED */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* @TEST_ANCHOR */
#ifdef USE_USB_CAMERA
#if ((defined USB_HOST_CONFIG_KHCI) && (USB_HOST_CONFIG_KHCI))
#ifndef CONTROLLER_ID
#define CONTROLLER_ID kUSB_ControllerKhci0
#endif
#endif /* USB_HOST_CONFIG_KHCI */
#if ((defined USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI))
#ifndef CONTROLLER_ID
#define CONTROLLER_ID kUSB_ControllerEhci0
#endif
#endif /* USB_HOST_CONFIG_EHCI */
#if ((defined USB_HOST_CONFIG_OHCI) && (USB_HOST_CONFIG_OHCI))
#ifndef CONTROLLER_ID
#define CONTROLLER_ID kUSB_ControllerOhci0
#endif
#endif /* USB_HOST_CONFIG_OHCI */
#if ((defined USB_HOST_CONFIG_IP3516HS) && (USB_HOST_CONFIG_IP3516HS))
#ifndef CONTROLLER_ID
#define CONTROLLER_ID kUSB_ControllerIp3516Hs0
#endif
#endif /* USB_HOST_CONFIG_IP3516HS */

#if defined(__GIC_PRIO_BITS)
#define USB_HOST_INTERRUPT_PRIORITY (25U)
#else
#define USB_HOST_INTERRUPT_PRIORITY (6U)
#endif

/*! @brief host app device attach/detach status */
typedef enum _usb_host_app_state
{
    kStatus_DEV_Idle = 0, /*!< there is no device attach/detach */
    kStatus_DEV_Attached, /*!< device is attached */
    kStatus_DEV_Detached, /*!< device is detached */
} usb_host_app_state_t;

enum{
	USB_CAMERA_FRAME_READY,
	USB_CAMERA_FRAME_DONE,
	USB_CAMERA_LAST = 0xFF
};

typedef struct usb_camera_msg_{
	uint8_t cmd;
	uint8_t reserved[3]; // for alignment
	void* parameter;
}usb_camera_msg_t;

#define MATCH_RESOLUTION_FIXED 1
#define NUM_FRAMES_TO_PROCESS -1 // -1 all frames, >0 process as many frames as requested

#define USB_FRAME_INTERVAL_OVERRIDE 1
//#define USB_FRAME_INTERVAL_VALUE 666666; /* 15FPS */
#define USB_FRAME_INTERVAL_VALUE 1000000; /* 10FPS */
//#define USB_FRAME_INTERVAL_VALUE 2000000; /* 5FPS */

#ifdef USE_PSRAM_JPG_BUFFERS
#define JPEG_BUFF0_ADDR 0x60400000U
#define JPEG_BUFF1_ADDR 0x60600000U
#endif /* USE_PSRAM_JPG_BUFFERS */

#endif /* USE_USB_CAMERA */

#ifdef RPMSG_USED
#define RPMSG_LITE_LINK_ID (RL_PLATFORM_IMXRT700_M33_0_M33_1_LINK_ID)
#endif /* RPMSG_USED */

#ifdef BOOT_SECONDARY_CORE
/* Address of memory, from which the secondary core will boot */
#ifndef CORE1_BOOT_ADDRESS
#define CORE1_BOOT_ADDRESS 0x20600000
#endif
#define APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY

#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t Image$$CORE1_REGION$$Base;
extern uint32_t Image$$CORE1_REGION$$Length;
#define CORE1_IMAGE_START &Image$$CORE1_REGION$$Base
#elif defined(__ICCARM__)
#pragma section = "__core1_image"
#define CORE1_IMAGE_START __section_begin("__core1_image")
#elif (defined(__GNUC__)) && (!defined(__MCUXPRESSO))
extern const char core1_image_start[];
extern const char *core1_image_end;
extern uint32_t core1_image_size;
#define CORE1_IMAGE_START ((void *)core1_image_start)
#define CORE1_IMAGE_SIZE  ((void *)core1_image_size)
#endif
#endif /* BOOT_SECONDARY_CORE */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*${prototype:start}*/
void BOARD_Init(void);

#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void);
#endif /* CORE1_IMAGE_COPY_TO_RAM */

#ifdef APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY
void invalidate_cache_for_core1_image_memory(uint32_t address, uint32_t size_byte);
#endif /* APP_INVALIDATE_CACHE_FOR_SECONDARY_CORE_IMAGE_MEMORY */
/*${prototype:end}*/

#endif /* __APP_H__ */
