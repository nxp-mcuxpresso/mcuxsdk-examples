/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MPP_CONFIG_H
#define _MPP_CONFIG_H

#include "test_config.h"

/* This header configures the MPP HAL and the application according to the board model */

/*******************************************************************************
 * HAL configuration (Mandatory)
 ******************************************************************************/
/* Set here all the static configuration of the Media Processing Pipeline HAL */

/**
 * This is the mimxrt700evk board configuration
 * Disabling HAL of unused/missing devices saves memory
 */

#define HAL_ENABLE_DISPLAY
#define HAL_ENABLE_DISPLAY_DEV_Lcdifv2Rk055   0
#define HAL_ENABLE_2D_IMGPROC
#define HAL_ENABLE_GFX_DEV_Pxp                0
#define HAL_ENABLE_GFX_DEV_Cpu                0
#define HAL_ENABLE_GFX_DEV_GPU                0

/**
 * This is the inference HAL configuration
 */

/* enable TFlite */
#define HAL_ENABLE_INFERENCE_TFLITE 1

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

/* select inference model converted for NPU */
#define APP_USE_NEUTRON64_MODEL

/* detection boxes params */
/* maximum number of boxes stored in RAM by APP (1box ~= 16B) */
#define APP_MAX_BOXES 100

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

/*
 * Enable this flag to define TFlite tensor arena non-cacheable.
 * Allocating tensor arena in non-cacheable memory may improve performance of
 * operators executed on NPU, but decreases performance on CPU.
 */
#ifdef APP_USE_NEUTRON64_MODEL
#define HAL_TENSOR_ARENA_NCACHE    1
#else
#define HAL_TENSOR_ARENA_NCACHE    0
#endif

/* Tensorflow lite Model data */
#ifdef APP_USE_NEUTRON64_MODEL
#if defined(USE_SCRFD_320_256_MODEL)
#define APP_TFLITE_SCRFD_KPS_DATA "models/scrfd_kps_500m_full_integer_quant/scrfd_kps_500m_full_integer_quant_320x256_npu64_tflite.h"
#define APP_TFLITE_SCRFD_KPS_INFO "models/scrfd_kps_500m_full_integer_quant/scrfd_kps_500m_full_integer_quant_320x256_tflite_info.h"
#elif defined(USE_SCRFD_256_256_MODEL)
#define APP_TFLITE_SCRFD_KPS_DATA "models/scrfd_kps_500m_full_integer_quant/scrfd_kps_500m_full_integer_quant_256x256_npu64_tflite.h"
#define APP_TFLITE_SCRFD_KPS_INFO "models/scrfd_kps_500m_full_integer_quant/scrfd_kps_500m_full_integer_quant_256x256_tflite_info.h"
#else
#define APP_TFLITE_SCRFD_KPS_DATA "models/scrfd_kps_500m_full_integer_quant/scrfd_kps_500m_full_integer_quant_128x128_npu64_tflite.h"
#define APP_TFLITE_SCRFD_KPS_INFO "models/scrfd_kps_500m_full_integer_quant/scrfd_kps_500m_full_integer_quant_128x128_tflite_info.h"
#endif /* USE_SCRFD_320_256_MODEL */
#else
#if defined(USE_SCRFD_320_256_MODEL)
#define APP_TFLITE_SCRFD_KPS_DATA "models/scrfd_kps_500m_full_integer_quant/scrfd_kps_500m_full_integer_quant_320x256_tflite.h"
#define APP_TFLITE_SCRFD_KPS_INFO "models/scrfd_kps_500m_full_integer_quant/scrfd_kps_500m_full_integer_quant_320x256_tflite_info.h"
#elif defined(USE_SCRFD_256_256_MODEL)
#define APP_TFLITE_SCRFD_KPS_DATA "models/scrfd_kps_500m_full_integer_quant/scrfd_kps_500m_full_integer_quant_256x256_tflite.h"
#define APP_TFLITE_SCRFD_KPS_INFO "models/scrfd_kps_500m_full_integer_quant/scrfd_kps_500m_full_integer_quant_256x256_tflite_info.h"
#else
#define APP_TFLITE_SCRFD_KPS_DATA "models/scrfd_kps_500m_full_integer_quant/scrfd_kps_500m_full_integer_quant_128x128_tflite.h"
#define APP_TFLITE_SCRFD_KPS_INFO "models/scrfd_kps_500m_full_integer_quant/scrfd_kps_500m_full_integer_quant_128x128_tflite_info.h"
#endif /* USE_SCRFD_320_256_MODEL */
#endif /* APP_USE_NEUTRON64_MODEL */

#include APP_TFLITE_SCRFD_KPS_INFO
#define MODEL_NUM_LANDMARKS SCRFD_NUM_LANDMARKS

#endif /* _MPP_CONFIG_H */
