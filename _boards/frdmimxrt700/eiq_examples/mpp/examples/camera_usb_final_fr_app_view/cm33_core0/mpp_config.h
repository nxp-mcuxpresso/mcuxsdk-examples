/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MPP_CONFIG_H
#define _MPP_CONFIG_H

#include "hal_camera_shared.h"

/* This header configures the MPP HAL and the application according to the board model */

/*******************************************************************************
 * HAL configuration (Mandatory)
 ******************************************************************************/
/* Set here all the static configuration of the Media Processing Pipeline HAL */

/**
 * This is the frdmimxrt700 board configuration
 * Disabling HAL of unused/missing devices saves memory
 */

#define HAL_ENABLE_CAMERA
#define HAL_ENABLE_CAMERA_DEV_EzhV_Ov7670     0
#define HAL_ENABLE_CAMERA_DEV_USB             0
#define HAL_ENABLE_VIRTUAL_CAMERA             1

#define HAL_ENABLE_DISPLAY
#define HAL_ENABLE_DISPLAY_DEV_LcdifWaveshare5Inch   1
#define HAL_ENABLE_DISPLAY_DEV_Fbdev          0
#define HAL_ENABLE_2D_IMGPROC

/* use GPU backend */
#define HAL_ENABLE_GFX_DEV_Pxp                0
#define HAL_ENABLE_GFX_DEV_Cpu                0
#define HAL_ENABLE_GFX_DEV_GPU                1

/* Enable JPEG CPU decoder */
#define HAL_ENABLE_JPEG_CPU                   0
#define HAL_ENABLE_JPEG_HW                    1

/**
 * This is the inference HAL configuration
 */

/* enable TFlite */
#define HAL_ENABLE_INFERENCE_TFLITE           1

/**
 * This is the inference HAL configuration
 */

/* The size of Tensor Arena buffer for TensorFlowLite-Micro */
/* minimum required arena size for mobilefacenet > scrfd */
#define HAL_TFLM_TENSOR_ARENA_SIZE_KB         895

/*
 * TFLite tensor arena buffer alignment requirement:
 * TFLite input buffer allocation is not dynamically controlled by the pipeline.
 * Thus, set HAL_TFLITE_BUFFER_ALIGN to 64B when using GPU backend as its output buffer
 * is 64B aligned.
 * Default value is 16Bytes.
 */
#define HAL_TFLITE_BUFFER_ALIGN               64

/*
 * Enable this flag to define TFlite tensor arena non-cacheable.
 * Allocating tensor arena in non-cacheable memory may improve performance of
 * operators executed on NPU, but decreases performance on CPU.
 */
#define HAL_TENSOR_ARENA_NCACHE               1

/**
 * VGLite heap size for MIMXRT700 EVK.
 */
#define HAL_VGLITE_HEAP_SZ                    0x100000 /* 1 MB */
#define HAL_VGLITE_BUFFER_ALIGN               64

/* GPU chip ID for MIMXRT700EVK board.*/
#define HAL_GPU_CHIPID                        0x555

/**
 * This is the display HAL configuration
 */

/* The display max byte per pixel */
#define HAL_DISPLAY_MAX_BPP                   4

/* Log level configuration
 * ERR:   0
 * INFO:  1
 * DEBUG: 2
 */
#ifndef HAL_LOG_LEVEL
#define HAL_LOG_LEVEL                         0
#endif

/**
 *  Mutex lock timeout definition
 *  An arbitrary default value is defined to 5 seconds
 *  value unit should be milliseconds
 * */
#define HAL_MUTEX_TIMEOUT_MS   (5000)

/*******************************************************************************
 * Application configuration (Optional)
 ******************************************************************************/
/* Set here all the static configuration of the Application */

/* Virtual usb dual camera parameters */
#define APP_CAMERA_NAME    "Virtual_USB_cam"
#define RGB_CAMERA_WIDTH  TARGET_CAMERA0_WIDTH
#define RGB_CAMERA_HEIGHT TARGET_CAMERA0_HEIGHT
#define IR_CAMERA_WIDTH   TARGET_CAMERA1_WIDTH
#define IR_CAMERA_HEIGHT  TARGET_CAMERA1_HEIGHT
#define APP_CAMERA_FORMAT  MPP_PIXEL_JPEG

/* display parameters */
#define APP_DISPLAY_NAME   "LcdifWaveshare5Inch" /* alternative: FBdev*/
#define APP_DISPLAY_WIDTH                     800
#define APP_DISPLAY_HEIGHT                    480
#define APP_DISPLAY_FORMAT MPP_PIXEL_RGB565

#define APP_GFX_BACKEND_NAME "gfx_GPU"
#define APP_GFX_BACKEND_INFER_NAME "gfx_GPU"

#define IMG_DECODE_DEV_NAME "jpeg_HW"

/* 30fps capture */
#define APP_RC_CYCLE_INC 10
#ifdef USE_USB_CAMERA
#define APP_RC_CYCLE_MIN 200
#else
#define APP_RC_CYCLE_MIN 33
#endif

/* Enable or not the antispoofing*/
#ifndef USE_ANTISPOOFING
#define USE_ANTISPOOFING 0
#endif
/* other parameters */
/* Waveshare 5inch DSI is natively landscape (800x480), no rotation needed */
#define APP_DISPLAY_LANDSCAPE_ROTATE ROTATE_270

#define APP_PIPELINE_TASK_MAX_PRIO            5
#define APP_RC_TASK_MAX_PRIO                  4
#define APP_PR_TASK_MAX_PRIO                  3

/* select inference model converted for NPU */
#define APP_USE_NEUTRON64_MODEL

/* enable rotation for display */
#define APP_SKIP_CONVERT_FOR_DISPLAY 0

#ifdef APP_USE_NEUTRON64_MODEL
#define APP_TFLITE_SCRFD_KPS_DATA "scrfd_kps_500m_full_integer_quant_256x256_npu64_tflite.h"
#define APP_TFLITE_SCRFD_KPS_INFO "scrfd_kps_500m_full_integer_quant_256x256_tflite_info.h"
#define APP_TFLITE_MOBILEFACENET_DATA "mobilefacenet_96_96_npu64_tflite.h"
#define APP_TFLITE_MOBILEFACENET_INFO "mobilefacenet_96_96_npu64_tflite_info.h"
#define APP_TFLITE_ANTISPOOFING_DATA "antispoofing_npu64_tflite.h"
#define APP_TFLITE_ANTISPOOFING_INFO "antispoofing_npu64_tflite_info.h"
#else
#define APP_TFLITE_SCRFD_KPS_DATA "scrfd_kps_500m_full_integer_quant_128x128_tflite.h"
#define APP_TFLITE_SCRFD_KPS_INFO "scrfd_kps_500m_full_integer_quant_128x128_tflite_info.h"
#define APP_TFLITE_MOBILEFACENET_DATA "mobilefacenet_96_96_tflite.h"
#define APP_TFLITE_MOBILEFACENET_INFO "mobilefacenet_96_96_tflite_info.h"
#define APP_TFLITE_ANTISPOOFING_DATA "antispoofing_tflite.h"
#define APP_TFLITE_ANTISPOOFING_INFO "antispoofing_tflite_info.h"
#endif  // APP_USE_NEUTRON64_MODEL

/* detection boxes params */
/* maximum number of boxes stored in RAM by APP (1box ~= 16B) */
#define APP_MAX_BOXES 100

#include APP_TFLITE_SCRFD_KPS_INFO
#define MODEL_NUM_LANDMARKS SCRFD_NUM_LANDMARKS
/* define persons database */
#define APP_DATABASE_NAME "face_database.h"
#define APP_DATABASE_INFOS "mobilefacenet_database_infos.h"

#endif /* _MPP_CONFIG_H */
