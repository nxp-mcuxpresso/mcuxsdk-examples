/* Copyright 2024-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MPP_CONFIG_H
#define _MPP_CONFIG_H

/* This header configures the MPP HAL and the application according to the board model */

/*
 * 0: MATCH_FORMAT_ANY
 * 1: MATCH_FORMAT_MJPEG
 * 2: MATCH_FORMAT_UNCOMPRESSED
 *
 */
#ifdef USE_USB_CAMERA
#include "host_video.h"
#define MATCH_FORMAT MATCH_FORMAT_MJPEG
#endif

/*******************************************************************************
 * HAL configuration (Mandatory)
 ******************************************************************************/

/**
 * This is the frdmimxrt700 board configuration
 * Disabling HAL of unused/missing devices saves memory
 */

#define HAL_ENABLE_CAMERA
#ifdef USE_USB_CAMERA
#define HAL_ENABLE_CAMERA_DEV_EzhV_Ov7670     0
#define HAL_ENABLE_CAMERA_DEV_USB             1
#else /* Use Flexio camera */
#define HAL_ENABLE_CAMERA_DEV_EzhV_Ov7670     1
#define HAL_ENABLE_CAMERA_DEV_USB             0
#endif
#define HAL_ENABLE_DISPLAY
#define HAL_ENABLE_DISPLAY_DEV_LcdifWaveshare5Inch   1
#define HAL_ENABLE_2D_IMGPROC

/**
 * This is the image decoder HAL configuration
 */

/* enable JPEG SW decoder */
#if (MATCH_FORMAT != MATCH_FORMAT_UNCOMPRESSED)
#define HAL_ENABLE_JPEG_CPU                   0
#define HAL_ENABLE_JPEG_HW                    1
#endif

/* use GPU backend */
#define HAL_ENABLE_GFX_DEV_Pxp                0
#define HAL_ENABLE_GFX_DEV_Cpu                0
#define HAL_ENABLE_GFX_DEV_GPU                1

/* use TFlite micro inference engine for this application */
#define HAL_ENABLE_INFERENCE_TFLITE           1

/**
 * This is the inference HAL configuration
 */

/* The size of Tensor Arena buffer for TensorFlowLite-Micro */
/* minimum required arena size for persondetect converted for NPU */
#define HAL_TFLM_TENSOR_ARENA_SIZE_KB         550

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
 * This is HAL debug configuration
 */

/*
 * Log level configuration
 * ERR:   0
 * INFO:  1
 * DEBUG: 2
 */
#ifndef HAL_LOG_LEVEL
#define HAL_LOG_LEVEL 0
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

/* camera parameters */
#ifdef USE_USB_CAMERA
#define APP_CAMERA_NAME    "USB_cam"
#if (MATCH_FORMAT == MATCH_FORMAT_UNCOMPRESSED)
#define APP_CAMERA_WIDTH   320   //320 //1280 //640 //352
#define APP_CAMERA_HEIGHT  240   //240 // 720 //480 //288
#define APP_CAMERA_FORMAT  MPP_PIXEL_YUYV
#else
#define APP_CAMERA_WIDTH   640   //320 //1280 //640 //352
#define APP_CAMERA_HEIGHT  480   //240 // 720 //480 //288
#define APP_CAMERA_FORMAT  MPP_PIXEL_JPEG
#endif
#else /* Flexio camera parameters */
#define APP_CAMERA_NAME    "EzhV_Ov7670"
#define APP_CAMERA_WIDTH   640
#define APP_CAMERA_HEIGHT  480
#define APP_CAMERA_FORMAT  MPP_PIXEL_RGB565
#endif

/* display parameters */
#define APP_DISPLAY_NAME                      "LcdifWaveshare5Inch"
#define APP_DISPLAY_WIDTH                     800
#define APP_DISPLAY_HEIGHT                    480
#define APP_DISPLAY_FORMAT                    MPP_PIXEL_RGB565

/* Activate scaled view parameters */
#define SCALED_VIEW

#define APP_GFX_BACKEND_NAME    "gfx_GPU"
#define APP_DECODE_BACKEND_NAME "jpeg_HW"

/* 30fps capture */
#define APP_RC_CYCLE_INC 3
#ifdef USE_USB_CAMERA
#define APP_RC_CYCLE_MIN 66
#else
#define APP_RC_CYCLE_MIN 33
#endif

/* other parameters */
/* Waveshare 5inch DSI is natively landscape (800x480), no rotation needed */
#ifdef USE_USB_CAMERA
#define APP_DISPLAY_LANDSCAPE_ROTATE ROTATE_0
#define APP_SRC_DISPLAY_FLIP         FLIP_HORIZONTAL
#else /* OV7670 */
#define APP_DISPLAY_LANDSCAPE_ROTATE ROTATE_0
#define APP_SRC_DISPLAY_FLIP         FLIP_NONE
#endif

/* select inference model converted for NPU */
#define APP_USE_NEUTRON64_MODEL

/* Tensorflow lite Model data */
#ifdef USE_NAS_OPTIMIZED_MODEL
#define APP_TFLITE_PERSONDETECT_DATA "persondetect_220_220_npu64_tflite.h"
#define APP_TFLITE_PERSONDETECT_INFO "persondetect_220_220_npu64_tflite_info.h"
#else
#define APP_TFLITE_PERSONDETECT_DATA "persondetect_160_128_npu64_tflite.h"
#define APP_TFLITE_PERSONDETECT_INFO "persondetect_160_128_npu64_tflite_info.h"
#endif


#endif /* _MPP_CONFIG_H */
