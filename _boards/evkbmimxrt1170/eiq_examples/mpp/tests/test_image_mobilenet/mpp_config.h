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
#define HAL_ENABLE_GFX_DEV_Pxp                1
#define HAL_ENABLE_GFX_DEV_Cpu                0
#define HAL_ENABLE_GFX_DEV_GPU                0

/**
 * This is the inference HAL configuration
 */

/* enable TFlite by default */
#if !defined(INFERENCE_ENGINE_GLOW) && !defined(INFERENCE_ENGINE_DeepViewRT)
#define HAL_ENABLE_INFERENCE_TFLITE           1
#define HAL_ENABLE_INFERENCE_GLOW             0
#define HAL_ENABLE_INFERENCE_DVRT             0
#elif defined(INFERENCE_ENGINE_GLOW)
#define HAL_ENABLE_INFERENCE_TFLITE           0
#define HAL_ENABLE_INFERENCE_GLOW             1
#define HAL_ENABLE_INFERENCE_DVRT             0
#elif defined(INFERENCE_ENGINE_DeepViewRT)
#define HAL_ENABLE_INFERENCE_TFLITE           0
#define HAL_ENABLE_INFERENCE_GLOW             0
#define HAL_ENABLE_INFERENCE_DVRT             1
#endif

/* The size of Tensor Arena buffer for TensorFlowLite-Micro */
/* minimum required arena size for MobileNetv1 */
#define HAL_TFLM_TENSOR_ARENA_SIZE_KB         512

/* The memory size used for weights and activations when using glow inference with Mobilenet v1,
 * these macros should be adjusted when using another model*/
#define HAL_GLOW_CONSTANT_WEIGHTS_MAX_MEMSIZE 479168
#define HAL_GLOW_MUTABLE_WEIGHTS_MAX_MEMSIZE  53184
#define HAL_GLOW_ACTIVATIONS_MAX_MEMSIZE      98368

/* The mempool holds DeepViewRT intermediate buffers that can be multiple megabytes in size and therefore
 * should be stored in the SDRAM. It's allocated in the heap (not FreeRTOS's heap) */
#define HAL_DVRT_MEMPOOL_SIZE_MB              5

/* Cache is optionally used by DeepViewRT to optimize certain internal loops.
 * If  HAL_DVRT_USE_CACHE is set to 0 then it will be allocated on the heap using the provided HAL_DVRT_CACHE_SIZE_KB.
 * If  HAL_DVRT_USE_CACHE is set to 1 it will be placed in the fastest available memory defined by HAL_DVRT_CACHE_MEMORY
 * for maximum performance. */
#define HAL_DVRT_USE_CACHE                    1
#define HAL_DVRT_CACHE_SIZE_KB                256
#define HAL_BOARD_SRAM_DTC_CM7                0x20000000              /* Board SRAM_DTC address */
#define HAL_DVRT_CACHE_MEMORY                 HAL_BOARD_SRAM_DTC_CM7  /* DVRT cache will be allocated in the SRAM_DTC_CM7 */

/**
 * This is the PXP HAL configuration
 */

/* Workaround for the PXP bug where BGR888 is output instead of RGB888 [MPP-97] */
#define HAL_PXP_WORKAROUND_OUT_RGB            1


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


/* Tensorflow lite Model data */
#ifdef APP_USE_NEUTRON64_MODEL
#define APP_TFLITE_MOBILENET_DATA "models/mobilenet_v1_0.25_128_quant_int8/mobilenetv1_model_data_tflite_npu64.h"
#define APP_TFLITE_MOBILENET_INFO "models/mobilenet_v1_0.25_128_quant_int8/mobilenetv1_model_data_tflite_npu64_info.h"
#elif defined(APP_USE_NEUTRON16_MODEL)
#define APP_TFLITE_MOBILENET_DATA "models/mobilenet_v1_0.25_128_quant_int8/mobilenetv1_model_data_tflite_npu16.h"
#define APP_TFLITE_MOBILENET_INFO "models/mobilenet_v1_0.25_128_quant_int8/mobilenetv1_model_data_tflite_npu16_info.h"
#else
#define APP_TFLITE_MOBILENET_DATA "models/mobilenet_v1_0.25_128_quant_int8/mobilenetv1_model_data_tflite.h"
#define APP_TFLITE_MOBILENET_INFO "models/mobilenet_v1_0.25_128_quant_int8/mobilenetv1_model_data_tflite_info.h"
#endif  // APP_USE_NEUTRON16_MODEL

#endif /* _MPP_CONFIG_H */
