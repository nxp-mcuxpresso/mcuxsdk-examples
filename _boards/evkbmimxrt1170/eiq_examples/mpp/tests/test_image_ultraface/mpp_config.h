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
 * This is the evkbmimxrt1170 board configuration
 * Disabling HAL of unused/missing devices saves memory
 */
#define HAL_ENABLE_2D_IMGPROC
#define HAL_ENABLE_GFX_DEV_Pxp                1
#define HAL_ENABLE_GFX_DEV_Cpu                0
#define HAL_ENABLE_GFX_DEV_GPU                0

/**
 * This is the inference HAL configuration
 */

/* This example uses TFlite inference engine */
#if !defined(INFERENCE_ENGINE_GLOW) && !defined(INFERENCE_ENGINE_DeepViewRT)
#define HAL_ENABLE_INFERENCE_TFLITE 1
#define HAL_ENABLE_INFERENCE_GLOW 0
#define HAL_ENABLE_INFERENCE_DVRT 0
#elif defined(INFERENCE_ENGINE_GLOW)
#error "Glow inference not supported in this app for this board"
#elif defined(INFERENCE_ENGINE_DeepViewRT)
#error "DeepViewRT inference not supported in this app for this board"
#endif

/* The size of Tensor Arena buffer for TensorFlowLite-Micro */
/* minimum required arena size for Ultraface is 1536 */
#define HAL_TFLM_TENSOR_ARENA_SIZE_KB         1536

/**
 * This is the PXP HAL configuration
 */

/* Workaround for the PXP bug where BGR888 is output instead of RGB888 [MPP-97] */
#define HAL_PXP_WORKAROUND_OUT_RGB            1

/**
 * This is the display HAL configuration
 */

/* The display max byte per pixel */
#define HAL_DISPLAY_MAX_BPP                   2

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

/* detection boxes params */
/* maximum number of boxes stored in RAM by APP (1box ~= 16B) */
#define APP_MAX_BOXES 10000


/* Tensorflow lite Model data */
#ifndef APP_ULTRAFACE_ULTRASLIM
#define APP_TFLITE_ULTRAFACE_DATA "models/ultraface_slim_quant_int8/ultraface_slim_tflite.h"
#define APP_TFLITE_ULTRAFACE_INFO "models/ultraface_slim_quant_int8/ultraface_slim_tflite_info.h"
#else
#ifdef APP_USE_NEUTRON16_MODEL
#define APP_TFLITE_ULTRAFACE_DATA "models/ultraface_slim_quant_int8/ultraface_slim_ultraslim_npu16_tflite.h"
#define APP_TFLITE_ULTRAFACE_INFO "models/ultraface_slim_quant_int8/ultraface_slim_ultraslim_npu16_tflite_info.h"
#elif defined(APP_USE_NEUTRON64_MODEL)
#define APP_TFLITE_ULTRAFACE_DATA "models/ultraface_slim_quant_int8/ultraface_slim_ultraslim_npu64_tflite.h"
#define APP_TFLITE_ULTRAFACE_INFO "models/ultraface_slim_quant_int8/ultraface_slim_ultraslim_npu64_tflite_info.h"
#else
#define APP_TFLITE_ULTRAFACE_DATA "models/ultraface_slim_quant_int8/ultraface_slim_ultraslim_tflite.h"
#define APP_TFLITE_ULTRAFACE_INFO "models/ultraface_slim_quant_int8/ultraface_slim_ultraslim_tflite_info.h"
#endif
#endif

#endif /* _MPP_CONFIG_H */

