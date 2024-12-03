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
 * This is the frdmmcxn947 board configuration
 * Disabling HAL of unused/missing devices saves memory
 */
#define HAL_ENABLE_DISPLAY_DEV_McuLcdST7796S 0
#define HAL_ENABLE_DISPLAY_DEV_McuLcdSsd1963 0
#define HAL_ENABLE_CAMERA_DEV_EzhOv7670 	 0
#define HAL_ENABLE_GFX_DEV_Pxp 				 0
#define HAL_ENABLE_GFX_DEV_Cpu 				 1

/* enable TFlite by default */
#define HAL_ENABLE_INFERENCE_TFLITE 1

/**
 * This is the inference HAL configuration
 */

/* The size of Tensor Arena buffer for TensorFlowLite-Micro */
/* minimum required arena size for persondetect converted for NPU */
#define HAL_TFLM_TENSOR_ARENA_SIZE_KB 247


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

/* select inference model converted for NPU */
#define APP_USE_NEUTRON16_MODEL

/* define a Tensorflow lite model data input */
/* NAS optimized model can't be used on MCXN because it requires large tensor arena size */
#define APP_TFLITE_PERSONDETECT_DATA "models/persondetect/persondetect_160_128_npu16_tflite.h"
#define APP_TFLITE_PERSONDETECT_INFO "models/persondetect/persondetect_160_128_npu16_tflite_info.h"

/* define a static image that will be included if needed */
#define APP_STATIC_IMAGE_NAME "images/skigirl_COCO_160_128_rgb.h"

/* detection boxes params */
/* maximum number of boxes stored in RAM by APP (1box ~= 16B) */
#define APP_MAX_BOXES 100

#endif /* _MPP_CONFIG_H */
