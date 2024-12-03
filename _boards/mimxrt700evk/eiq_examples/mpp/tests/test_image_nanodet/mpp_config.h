/*
 * Copyright 2024 NXP
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
#define HAL_ENABLE_INFERENCE_TFLITE           1

/* The size of Tensor Arena buffer for TensorFlowLite-Micro */
/* minimum required arena size for Nanodet-m converted for NPU */
#define HAL_TFLM_TENSOR_ARENA_SIZE_KB        1153

/*
 * Enable this flag to define TFlite tensor arena non-cacheable.
 * Allocating tensor arena in non-cacheable memory may improve performance of
 * operators executed on NPU, but decreases performance on CPU.
 */
#define HAL_TENSOR_ARENA_NCACHE               1

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
#define HAL_MUTEX_TIMEOUT_MS                  (5000)

/*******************************************************************************
 * Application configuration (Optional)
 ******************************************************************************/
/* Set here all the static configuration of the Application */

/* select inference model converted for NPU */
#define APP_USE_NEUTRON64_MODEL

/* detection boxes params */
/* maximum number of boxes stored in RAM by APP (1box ~= 16B) */
#define APP_MAX_BOXES                           100


/* Tensorflow lite Model data */
#ifdef APP_USE_NEUTRON64_MODEL
#define APP_TFLITE_NANODET_DATA "models/nanodet_m_320_quant_int8/nanodet_m_0.5x_nhwc_nopermute_npu64_tflite.h"
#define APP_TFLITE_NANODET_INFO "models/nanodet_m_320_quant_int8/nanodet_m_0.5x_nhwc_nopermute_npu64_tflite_info.h"
#else
#define APP_TFLITE_NANODET_DATA "models/nanodet_m_320_quant_int8/nanodet_m_0.5x_nhwc_tflite.h"
#define APP_TFLITE_NANODET_INFO "models/nanodet_m_320_quant_int8/nanodet_m_0.5x_nhwc_tflite_info.h"
#endif  // APP_USE_NEUTRON64_MODEL

#endif /* _MPP_CONFIG_H */
