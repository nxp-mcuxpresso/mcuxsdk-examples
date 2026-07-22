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
 * This is the frdmimx1152 board configuration
 * Disabling HAL of unused/missing devices saves memory
 */

#define HAL_ENABLE_2D_IMGPROC
#define HAL_ENABLE_GFX_DEV_Pxp                0
#define HAL_ENABLE_GFX_DEV_Cpu                0
#define HAL_ENABLE_GFX_DEV_GPU                0

/**
 * This is the inference HAL configuration
 */

/* enable TFlite by default */
#define HAL_ENABLE_INFERENCE_TFLITE              1

/* The size of Tensor Arena buffer for TensorFlowLite-Micro */
/* minimum required arena size for mobilefacenet */
#define HAL_TFLM_TENSOR_ARENA_SIZE_KB            686

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
/* define a Tensorflow lite model data input */
#define APP_TFLITE_MOBILEFACENET_DATA "models/mobilefacenet/mobilefacenet_96_96_tflite.h"
#define APP_TFLITE_MOBILEFACENET_INFO "models/mobilefacenet/mobilefacenet_96_96_tflite_info.h"

/* define persons database */
#define APP_DATABASE_NAME "models/mobilefacenet/embeddings_database.h"
#define APP_DATABASE_INFOS "models/mobilefacenet/mobilefacenet_database_infos.h"

/* define a static image that will be included if needed */
#define APP_STATIC_IMAGE_NAME "images/thispersondoesnotexist_4_96_rgb.h"

#endif /* _MPP_CONFIG_H */
