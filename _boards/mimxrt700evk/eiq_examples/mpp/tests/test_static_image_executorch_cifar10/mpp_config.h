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

/* enable ExecuTorch */
#define HAL_ENABLE_INFERENCE_EXECUTORCH        1

/* Use Neutron backend for ExecuTorch inference */
#define HAL_EXECUTORCH_BACKEND_NEUTRON 1

/* Memory arena sizes for ExecuTorch runtime (in KB) */
#define HAL_EXECUTORCH_METHOD_ARENA_SIZE_KB     256
#define HAL_EXECUTORCH_TEMP_ARENA_SIZE_KB       256

/*
 * Enable this flag to place ExecuTorch arenas in non-cacheable memory.
 * Allocating arenas in non-cacheable memory may improve performance of
 * operators executed on NPU, but decreases performance on CPU.
 */
#define HAL_EXECUTORCH_ARENA_NCACHE            1

/* ExecuTorch buffer alignment (bytes) */
#define HAL_EXECUTORCH_BUFFER_ALIGN            16

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

/* ExecuTorch CIFAR-10 model data */
#define APP_EXECUTORCH_CIFAR_DATA              "cifar10_model_data_executorch.h"
#define APP_EXECUTORCH_CIFAR_INFO              "cifar10_model_data_executorch_info.h"

#endif /* _MPP_CONFIG_H */
