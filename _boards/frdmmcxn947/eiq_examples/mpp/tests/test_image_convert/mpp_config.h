/*
 * Copyright 2023-2024 NXP
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
 * This is the mcxn9xxevk board configuration
 * Disabling HAL of unused/missing devices saves memory
 */
#define HAL_ENABLE_DISPLAY_DEV_McuLcdST7796S 0
#define HAL_ENABLE_DISPLAY_DEV_McuLcdSsd1963 0
#define HAL_ENABLE_CAMERA_DEV_EzhOv7670 0
#define HAL_ENABLE_GFX_DEV_Pxp 0
#define HAL_ENABLE_2D_IMGPROC
#define HAL_ENABLE_GFX_DEV_Cpu 1

/**
 * This is the inference HAL configuration
 */
#define HAL_ENABLE_INFERENCE_TFLITE 0
#define HAL_ENABLE_INFERENCE_GLOW 0
#define HAL_ENABLE_INFERENCE_DVRT 0

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

/* enable checksum computation using Pisano [TESTS ONLY] */
#ifndef ENABLE_PISANO_CHECKSUM
#define ENABLE_PISANO_CHECKSUM 1
#endif

/*******************************************************************************
 * Application configuration (Optional)
 ******************************************************************************/
/* Set here all the static configuration of the Application */

#endif /* _MPP_CONFIG_H */

