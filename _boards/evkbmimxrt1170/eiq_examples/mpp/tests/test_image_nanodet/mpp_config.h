/*
 * Copyright 2021-2024 NXP
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
 * This is the evkbmimxrt1170 board configuration
 * Disabling HAL of unused/missing devices saves memory
 */

#define HAL_ENABLE_2D_IMGPROC
#define HAL_ENABLE_GFX_DEV_Pxp                  1
#define HAL_ENABLE_GFX_DEV_Cpu                  0
#define HAL_ENABLE_GFX_DEV_GPU                  0

/**
 * This is the inference HAL configuration
 */

/* enable TFlite by default */
#if !defined(INFERENCE_ENGINE_GLOW) && !defined(INFERENCE_ENGINE_DeepViewRT)
#define HAL_ENABLE_INFERENCE_TFLITE              1
#define HAL_ENABLE_INFERENCE_GLOW                0
#define HAL_ENABLE_INFERENCE_DVRT                0
#elif defined(INFERENCE_ENGINE_GLOW)
#define HAL_ENABLE_INFERENCE_TFLITE              0
#define HAL_ENABLE_INFERENCE_GLOW                1
#define HAL_ENABLE_INFERENCE_DVRT                0
#elif defined(INFERENCE_ENGINE_DeepViewRT)
#error "DeepViewRT inference is not supported in this application"
#endif


/* The size of Tensor Arena buffer for TensorFlowLite-Micro */
/* minimum required arena size for Nanodet-M */
#define HAL_TFLM_TENSOR_ARENA_SIZE_KB            2048

/* The memory size used for weights and activations when using glow inference with Nanodet-M,
 * these macros should be adjusted when using another model*/
#define HAL_GLOW_CONSTANT_WEIGHTS_MAX_MEMSIZE    216704
#define HAL_GLOW_MUTABLE_WEIGHTS_MAX_MEMSIZE     352000
#define HAL_GLOW_ACTIVATIONS_MAX_MEMSIZE         1075200

/**
 * This is the PXP HAL configuration
 */

/* Workaround for the PXP bug where BGR888 is output instead of RGB888 [MPP-97] */
#define HAL_PXP_WORKAROUND_OUT_RGB               1

/* Log level configuration
 * ERR:   0
 * INFO:  1
 * DEBUG: 2
 */
#ifndef HAL_LOG_LEVEL
#define HAL_LOG_LEVEL                            0
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

/* detection boxes params */
/* maximum number of boxes stored in RAM by APP (1box ~= 16B) */
#define APP_MAX_BOXES                           10000


/* Tensorflow lite Model data */
#ifdef APP_USE_NEUTRON64_MODEL
#define APP_TFLITE_NANODET_DATA "models/nanodet_m_320_quant_int8/nanodet_m_0.5x_nhwc_nopermute_npu64_tflite.h"
#define APP_TFLITE_NANODET_INFO "models/nanodet_m_320_quant_int8/nanodet_m_0.5x_nhwc_nopermute_npu64_tflite_info.h"
#else
#define APP_TFLITE_NANODET_DATA "models/nanodet_m_320_quant_int8/nanodet_m_0.5x_nhwc_tflite.h"
#define APP_TFLITE_NANODET_INFO "models/nanodet_m_320_quant_int8/nanodet_m_0.5x_nhwc_tflite_info.h"
#endif  // APP_USE_NEUTRON64_MODEL

#endif /* _MPP_CONFIG_H */
