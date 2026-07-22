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

#define HAL_ENABLE_CAMERA
#define HAL_ENABLE_CAMERA_DEV_MipiOv5640      1
#define HAL_ENABLE_DISPLAY
#define HAL_ENABLE_DISPLAY_DEV_Lcdifv2Rk055   1
#define HAL_ENABLE_2D_IMGPROC
#define HAL_ENABLE_GFX_DEV_Pxp                1
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

/* maximum allowed number of faces in the database */
#define MAX_FACES								 1000

/* maximum string size for names */
#define MAX_STR_SIZE							 32

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

/* Workaround for the PXP bug where BGR888 is output instead of RGB888 [MPP-97] */
#define HAL_PXP_WORKAROUND_OUT_RGB            1
/*******************************************************************************
 * Application configuration (Optional)
 ******************************************************************************/

/* Set here all the static configuration of the Application */
/* camera parameters */
#define APP_CAMERA_NAME                       "MipiOv5640"
#define APP_CAMERA_WIDTH                      1280
#define APP_CAMERA_HEIGHT                     720
#define APP_CAMERA_FORMAT                     MPP_PIXEL_YUV1P444

/* display parameters */
#define APP_DISPLAY_NAME                      "Lcdifv2Rk055"
#define APP_DISPLAY_WIDTH                     720
#define APP_DISPLAY_HEIGHT                    1280
#define APP_DISPLAY_FORMAT                    MPP_PIXEL_RGB565

/* other parameters */
/* rotation is needed to display in landscape because display RK055 is portrait */
#define APP_DISPLAY_LANDSCAPE_ROTATE          ROTATE_90

/* enable checksum computation on display framebuffer [TESTS ONLY] */
#ifndef ENABLE_FB_CHEKSUM
#define ENABLE_FB_CHEKSUM                     0
#endif

#define APP_PIPELINE_TASK_MAX_PRIO            5

/* define a Tensorflow lite model data input */
#define APP_TFLITE_MOBILEFACENET_DATA "models/mobilefacenet/mobilefacenet_96_96_tflite.h"
#define APP_TFLITE_MOBILEFACENET_INFO "models/mobilefacenet/mobilefacenet_96_96_tflite_info.h"

/* define a static image that will be included if needed */
#define APP_STATIC_IMAGE_NAME "images/thispersondoesnotexist_11_bgra.h"

/* define persons database */
#define APP_DATABASE_NAME "models/mobilefacenet/embeddings_database.h"
#define APP_DATABASE_INFOS "models/mobilefacenet/mobilefacenet_database_infos.h"

#endif /* _MPP_CONFIG_H */
