/*
 * Copyright 2025 NXP
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
 * This is the mimxrt700evk board configuration
 * Disabling HAL of unused/missing devices saves memory
 */

#define HAL_ENABLE_CAMERA
#define HAL_ENABLE_CAMERA_DEV_EzhV_Ov7670     1
#define HAL_ENABLE_DISPLAY
#define HAL_ENABLE_DISPLAY_DEV_Lcdifv2Rk055   1
#define HAL_ENABLE_2D_IMGPROC
#define HAL_ENABLE_GFX_DEV_Pxp                0
#define HAL_ENABLE_GFX_DEV_Cpu                0
#define HAL_ENABLE_GFX_DEV_GPU                1

/**
 * This is the inference HAL configuration
 */

/* enable TFlite */
#define HAL_ENABLE_INFERENCE_TFLITE 1

/*
 * Enable this flag to define TFlite tensor arena non-cacheable.
 * Allocating tensor arena in non-cacheable memory may improve performance of
 * operators executed on NPU, but decreases performance on CPU.
 */
#define HAL_TENSOR_ARENA_NCACHE    1

/*
 * TFLite tensor arena buffer alignment requirement:
 * TFLite input buffer allocation is not dynamically controlled by the pipeline.
 * Thus, set HAL_TFLITE_BUFFER_ALIGN to 64B when using GPU backend as its output buffer
 * is 64B aligned.
 * Default value is 16Bytes.
 */
#define HAL_TFLITE_BUFFER_ALIGN               64

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
#define APP_CAMERA_NAME                      "EzhV_Ov7670"
#define APP_CAMERA_WIDTH                     640
#define APP_CAMERA_HEIGHT                    480
#define APP_CAMERA_FORMAT                    MPP_PIXEL_RGB565

/* display parameters */
#define APP_DISPLAY_NAME                      "Lcdifv2Rk055"
#define APP_DISPLAY_WIDTH                     720
#define APP_DISPLAY_HEIGHT                    1280
#define APP_DISPLAY_FORMAT                    MPP_PIXEL_RGB565

/* gfx parameters */
#define APP_GFX_BACKEND_NAME "gfx_GPU"
#define APP_GFX_BACKEND_INFER_NAME "gfx_GPU"

/* pipeline tasks parameters */
#define APP_RC_CYCLE_MIN 33
#define APP_RC_CYCLE_INC 10

/* other parameters */
/* rotation is needed to display in landscape because display RK055 is portrait */
#define APP_DISPLAY_LANDSCAPE_ROTATE          ROTATE_90

/* select inference model converted for NPU */
#define APP_USE_NEUTRON64_MODEL
// #define USE_SCRFD_320_256_MODEL
// #define USE_SCRFD_256_256_MODEL

/* The size of Tensor Arena buffer for TensorFlowLite-Micro */
/* minimum required arena size for scrfd_kps_500m model on CPU or NPU */
#ifdef APP_USE_NEUTRON64_MODEL
#if defined(USE_SCRFD_320_256_MODEL) || defined(USE_SCRFD_256_256_MODEL)
#define HAL_TFLM_TENSOR_ARENA_SIZE_KB 932
#else
#define HAL_TFLM_TENSOR_ARENA_SIZE_KB 190
#endif /* USE_SCRFD_320_256_MODEL */
#else
#if defined(USE_SCRFD_320_256_MODEL) || defined(USE_SCRFD_256_256_MODEL)
#define HAL_TFLM_TENSOR_ARENA_SIZE_KB 1132
#else
#define HAL_TFLM_TENSOR_ARENA_SIZE_KB 256
#endif /* USE_SCRFD_320_256_MODEL */
#endif /* APP_USE_NEUTRON64_MODEL */

/* detection boxes params */
/* maximum number of boxes stored in RAM by APP (1box ~= 16B) */
#define APP_MAX_BOXES 100

#define SOURCE_STATIC_IMAGE 0

/* Tensorflow lite Model data */
#ifdef APP_USE_NEUTRON64_MODEL
#if defined(USE_SCRFD_320_256_MODEL)
#define APP_TFLITE_SCRFD_KPS_DATA "scrfd_kps_500m_full_integer_quant_320x256_npu64_tflite.h"
#define APP_TFLITE_SCRFD_KPS_INFO "scrfd_kps_500m_full_integer_quant_320x256_tflite_info.h"
#elif defined(USE_SCRFD_256_256_MODEL)
#define APP_TFLITE_SCRFD_KPS_DATA "scrfd_kps_500m_full_integer_quant_256x256_npu64_tflite.h"
#define APP_TFLITE_SCRFD_KPS_INFO "scrfd_kps_500m_full_integer_quant_256x256_tflite_info.h"
#else
#define APP_TFLITE_SCRFD_KPS_DATA "scrfd_kps_500m_full_integer_quant_128x128_npu64_tflite.h"
#define APP_TFLITE_SCRFD_KPS_INFO "scrfd_kps_500m_full_integer_quant_128x128_tflite_info.h"
#endif /* USE_SCRFD_320_256_MODEL */
#else
#if defined(USE_SCRFD_320_256_MODEL)
#define APP_TFLITE_SCRFD_KPS_DATA "scrfd_kps_500m_full_integer_quant_320x256_tflite.h"
#define APP_TFLITE_SCRFD_KPS_INFO "scrfd_kps_500m_full_integer_quant_320x256_tflite_info.h"
#elif defined(USE_SCRFD_256_256_MODEL)
#define APP_TFLITE_SCRFD_KPS_DATA "scrfd_kps_500m_full_integer_quant_256x256_tflite.h"
#define APP_TFLITE_SCRFD_KPS_INFO "scrfd_kps_500m_full_integer_quant_256x256_tflite_info.h"
#else
#define APP_TFLITE_SCRFD_KPS_DATA "scrfd_kps_500m_full_integer_quant_128x128_tflite.h"
#define APP_TFLITE_SCRFD_KPS_INFO "scrfd_kps_500m_full_integer_quant_128x128_tflite_info.h"
#endif /* USE_SCRFD_320_256_MODEL */
#endif /* APP_USE_NEUTRON64_MODEL */

#include APP_TFLITE_SCRFD_KPS_INFO
#define MODEL_NUM_LANDMARKS SCRFD_NUM_LANDMARKS

#if defined(USE_SCRFD_320_256_MODEL)
#define APP_STATIC_IMAGE_PATH "images/couple_COCO_320_256_rgb.h"
#elif defined(USE_SCRFD_256_256_MODEL)
#define APP_STATIC_IMAGE_PATH "images/couple_COCO_256_256_rgb.h"
#else
#define APP_STATIC_IMAGE_PATH "images/couple_COCO_128_128_rgb.h"
#endif /* USE_SCRFD_320_256_MODEL */

#endif /* _MPP_CONFIG_H */
