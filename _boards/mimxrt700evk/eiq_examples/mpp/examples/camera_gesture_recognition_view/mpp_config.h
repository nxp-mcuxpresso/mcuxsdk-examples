/* Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MPP_CONFIG_H
#define _MPP_CONFIG_H

/* This header configures the MPP HAL and the application according to the board model */

/*
 * 0: MATCH_FORMAT_ANY
 * 1: MATCH_FORMAT_MJPEG
 * 2: MATCH_FORMAT_UNCOMPRESSED
 *
 */
#ifdef USE_USB_CAMERA
#include "host_video.h"
#define MATCH_FORMAT MATCH_FORMAT_MJPEG
#endif

/*******************************************************************************
 * HAL configuration (Mandatory)
 ******************************************************************************/

/**
 * This is the mimxrt700evk board configuration
 * Disabling HAL of unused/missing devices saves memory
 */

#define HAL_ENABLE_CAMERA
#ifdef USE_USB_CAMERA
#define HAL_ENABLE_CAMERA_DEV_EzhV_Ov7670     0
#define HAL_ENABLE_CAMERA_DEV_USB             1
#else /* Use Flexio camera */
#define HAL_ENABLE_CAMERA_DEV_EzhV_Ov7670     1
#define HAL_ENABLE_CAMERA_DEV_USB             0
#endif
#define HAL_ENABLE_DISPLAY
#define HAL_ENABLE_DISPLAY_DEV_Lcdifv2Rk055   1
#define HAL_ENABLE_2D_IMGPROC

/**
 * This is the image decoder HAL configuration
 */

/* enable JPEG SW decoder */
#if (MATCH_FORMAT != MATCH_FORMAT_UNCOMPRESSED)
#define HAL_ENABLE_JPEG_CPU                   0
#define HAL_ENABLE_JPEG_HW                    1
#endif

/* use GPU backend */
#define HAL_ENABLE_GFX_DEV_Pxp                0
#define HAL_ENABLE_GFX_DEV_Cpu                0
#define HAL_ENABLE_GFX_DEV_GPU                1

/* use TFlite micro inference engine for this application */
#define HAL_ENABLE_INFERENCE_TFLITE           1

/**
 * This is the inference HAL configuration
 */

 /* select inference model converted for NPU */
#define APP_USE_NEUTRON64_MODEL

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
 * This is HAL debug configuration
 */

/*
 * Log level configuration
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

/* camera parameters */
#ifdef USE_USB_CAMERA
#define APP_CAMERA_NAME    "USB_cam"
#if (MATCH_FORMAT == MATCH_FORMAT_UNCOMPRESSED)
#define APP_CAMERA_WIDTH   320   //320 //1280 //640 //352
#define APP_CAMERA_HEIGHT  240   //240 // 720 //480 //288
#define APP_CAMERA_FORMAT  MPP_PIXEL_YUYV
#else
#define APP_CAMERA_WIDTH   640   //320 //1280 //640 //352
#define APP_CAMERA_HEIGHT  480   //240 // 720 //480 //288
#define APP_CAMERA_FORMAT  MPP_PIXEL_JPEG
#endif
#else /* Flexio camera parameters */
#define APP_CAMERA_NAME    "EzhV_Ov7670"
#define APP_CAMERA_WIDTH   320
#define APP_CAMERA_HEIGHT  240
#define APP_CAMERA_FORMAT  MPP_PIXEL_RGB565
#endif

/* display parameters */
#define APP_DISPLAY_NAME   "Lcdifv2Rk055"
#define APP_DISPLAY_WIDTH  720
#define APP_DISPLAY_HEIGHT 1280
#define APP_DISPLAY_FORMAT MPP_PIXEL_RGB565

/* Activate scaled view parameters */
#define SCALED_VIEW

#define APP_GFX_BACKEND_NAME    "gfx_GPU"
#define APP_DECODE_BACKEND_NAME "jpeg_HW"

/* 30fps capture */
#define APP_RC_CYCLE_INC 3
#ifdef USE_USB_CAMERA
#define APP_RC_CYCLE_MIN 66
#else
#define APP_RC_CYCLE_MIN 33
#endif

/* other parameters */
/* rotation is needed to display in landscape because display RK055 is portrait */
#ifdef USE_USB_CAMERA
#define APP_DISPLAY_LANDSCAPE_ROTATE ROTATE_270
#define APP_SRC_DISPLAY_FLIP         FLIP_HORIZONTAL
#else /* OV7670 */
#define APP_DISPLAY_LANDSCAPE_ROTATE ROTATE_90
#define APP_SRC_DISPLAY_FLIP         FLIP_NONE
#endif

/* detection boxes params */
/* maximum number of boxes stored in RAM by APP (1box ~= 16B) */
#define APP_MAX_BOXES 100

/* The size of Tensor Arena buffer for TensorFlowLite-Micro */
#ifdef APP_USE_NEUTRON64_MODEL
#define HAL_TFLM_TENSOR_ARENA_SIZE_KB         1200
#else
#define HAL_TFLM_TENSOR_ARENA_SIZE_KB         1300
#endif

/* Tensorflow lite Model data */
#ifdef APP_USE_NEUTRON64_MODEL
#define APP_TFLITE_BLAZE_DETECTOR_PTQ_DATA  "blaze_detector_ptq_int8_npu64_tflite.h"
#define APP_TFLITE_HAND_LANDMARK_DATA       "hand_landmark_quant_int8_npu64_tflite.h"
#else
#define APP_TFLITE_BLAZE_DETECTOR_PTQ_DATA  "blaze_detector_ptq_int8_tflite.h"
#define APP_TFLITE_HAND_LANDMARK_DATA       "hand_landmark_quant_int8_tflite.h"
#endif
#define APP_TFLITE_GESTURE_EMBEDDER_DATA    "gesture_embedder1_i8_opt_tflite.h"
#define APP_TFLITE_GESTURE_CLASSIFIER_DATA  "canned_gesture_classifier_i8_opt_tflite.h"

/* Model info */
#define APP_TFLITE_BLAZE_DETECTOR_PTQ_INFO "blaze_detector_ptq_int8_tflite_info.h"
#define APP_TFLITE_HAND_LANDMARK_INFO       "hand_landmark_quant_int8_tflite_info.h"
#define APP_TFLITE_GESTURE_EMBEDDER_INFO    "gesture_embedder1_i8_opt_tflite_info.h"
#define APP_TFLITE_GESTURE_CLASSIFIER_INFO  "canned_gesture_classifier_i8_opt_tflite_info.h"

#include APP_TFLITE_BLAZE_DETECTOR_PTQ_INFO
#include APP_TFLITE_HAND_LANDMARK_INFO
#include APP_TFLITE_GESTURE_EMBEDDER_INFO
#include APP_TFLITE_GESTURE_CLASSIFIER_INFO

#define MODEL_NUM_LANDMARKS         BLAZE_DETECTOR_NUM_LANDMARKS
#define MODEL_NUM_3D_LANDMARKS      HAND_LANDMARK_NUM_LANDMARKS

#define HAND_LANDMARK_CONVERT_ELEM_ID   2

#define BLAZE_DETECTOR_MODEL_ID         1
#define HANDLANDMARK_MODEL_ID           2
#define GESTURE_EMBEDDER_MODEL_ID       3
#define GESTURE_CLASSIFIER_MODEL_ID     4

#endif /* _MPP_CONFIG_H */
