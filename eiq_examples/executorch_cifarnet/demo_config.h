/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _DEMO_CONFIG_H_
#define _DEMO_CONFIG_H_

#include<cstdint>

#define EXAMPLE_NAME       "CIFARNET"
#define FRAMEWORK_NAME     "ExecuTorch"
#define DETECTION_TRESHOLD 60
#define NUM_RESULTS        1
#define DEMO_VERBOSE       false
#define OUTPUT_MIN_RANGE   INT8_MIN        // Min value of output data type
#define CONFIDENCE_SCALE   (256.0f / 100)  // Scale factor to convert uint8 [0-255] to percentage [0-100)

#endif // _DEMO_CONFIG_H_
