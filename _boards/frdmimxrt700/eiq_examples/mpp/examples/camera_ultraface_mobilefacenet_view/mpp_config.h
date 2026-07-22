/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MPP_CONFIG_H
#define _MPP_CONFIG_H

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
#define HAL_ENABLE_CAMERA_DEV_EzhV_Ov7670     1
#define HAL_ENABLE_DISPLAY
#define HAL_ENABLE_DISPLAY_DEV_LcdifWaveshare5Inch   1
#define HAL_ENABLE_2D_IMGPROC

/* use GPU backend */
#define HAL_ENABLE_GFX_DEV_Pxp                0
#define HAL_ENABLE_GFX_DEV_Cpu                1
#define HAL_ENABLE_GFX_DEV_GPU                1

/**
 * This is the inference HAL configuration
 */

/* enable TFlite */
#define HAL_ENABLE_INFERENCE_TFLITE           1

/**
 * This is the inference HAL configuration
 */

/* The size of Tensor Arena buffer for TensorFlowLite-Micro */
/* minimum required arena size for mobilefacenet > ultraface */
#define HAL_TFLM_TENSOR_ARENA_SIZE_KB         1390

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
/* camera parameters */
#define APP_CAMERA_NAME    "EzhV_Ov7670"
#define APP_CAMERA_WIDTH   640
#define APP_CAMERA_HEIGHT  480
#define APP_CAMERA_FORMAT  MPP_PIXEL_RGB565

/* display parameters */
#define APP_DISPLAY_NAME   "LcdifWaveshare5Inch"
#define APP_DISPLAY_WIDTH                     800
#define APP_DISPLAY_HEIGHT                    480
#define APP_DISPLAY_FORMAT MPP_PIXEL_RGB565

#define APP_GFX_BACKEND_NAME "gfx_GPU"
#define APP_GFX_BACKEND_INFER_NAME "gfx_GPU"

#define APP_RC_CYCLE_MIN 30
#define APP_RC_CYCLE_INC 10

/* other parameters */
/* Waveshare 5inch DSI is natively landscape (800x480), no rotation needed */
#define APP_DISPLAY_LANDSCAPE_ROTATE ROTATE_0

#define APP_PIPELINE_TASK_MAX_PRIO            5

/* select inference model converted for NPU */
#define APP_USE_NEUTRON64_MODEL

/* enable rotation for display */
#define APP_SKIP_CONVERT_FOR_DISPLAY 0

/* enable dynamic recognition box matching detected face */
#define APP_DYNAMIC_RECO_ZONE

/* Tensorflow lite Model data */
#define APP_ULTRAFACE_ULTRASLIM

#ifdef APP_USE_NEUTRON64_MODEL
#define APP_TFLITE_ULTRAFACE_DATA "ultraface_slim_ultraslim_npu64_tflite.h"
#define APP_TFLITE_ULTRAFACE_INFO "ultraface_slim_ultraslim_npu64_tflite_info.h"
#define APP_TFLITE_MOBILEFACENET_DATA "mobilefacenet_96_96_npu64_tflite.h"
#define APP_TFLITE_MOBILEFACENET_INFO "mobilefacenet_96_96_npu64_tflite_info.h"
#else
#define APP_TFLITE_ULTRAFACE_DATA "ultraface_slim_ultraslim_tflite.h"
#define APP_TFLITE_ULTRAFACE_INFO "ultraface_slim_ultraslim_tflite_info.h"
#define APP_TFLITE_MOBILEFACENET_DATA "mobilefacenet_96_96_tflite.h"
#define APP_TFLITE_MOBILEFACENET_INFO "mobilefacenet_96_96_tflite_info.h"
#endif  // APP_USE_NEUTRON64_MODEL

/* define persons database */
#define APP_DATABASE_NAME "face_database.h"
#define APP_DATABASE_INFOS "mobilefacenet_database_infos.h"

/* define a static image that will be included if needed */
#define APP_STATIC_IMAGE_NAME "thispersondoesnotexist_4_96_bgra.h"

#endif /* _MPP_CONFIG_H */
